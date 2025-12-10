/*
 *	This module provides a simple api for reading and writing JSON files. The implementation is not meant to
 *		compete with the ergonomics or the efficiency of other libraries. Instead, it is meant to provide the small
 *		utility that are need for JSON interfaces while not depending on other libraries.
 */

module;

#include <cassert>

export module synodic.honesty.utility:json;

import std;

namespace
{
	// A helper for variant overload deduction
	template<typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};
}

namespace honesty::utility
{

	// TODO: Use c++26 reflection for a simple lexer without pulling in a library
	export class JSON
	{
		using Array	 = std::vector<JSON>;
		using Object = std::map<std::string, JSON>;
		using Value =
			std::variant<std::monostate, bool, double, std::int32_t, std::uint32_t, std::string, Array, Object>;

	public:
		using size_type = Array::size_type;

		JSON() :
			data_(std::monostate())
		{
		}

		template<typename T>
			requires std::is_same_v<std::remove_cvref_t<T>, std::nullptr_t>
		explicit(false) JSON(const T&) :
			data_(std::monostate())
		{
		}

		JSON(const JSON& other)		= default;
		JSON(JSON&& other) noexcept = default;

		template<typename T>
			requires std::constructible_from<Value, T>
		explicit(false) JSON(T&& other) noexcept :
			data_(std::move(other))
		{
		}

		// NOTE: Using Array::value_type causes a compile error
		// Using std::from_range_t to avoid ambiguity with variant passing
		template<typename R>
			requires std::ranges::input_range<R> && std::convertible_to<std::ranges::range_reference_t<R>, JSON>
		JSON(std::from_range_t, R&& range) :
			data_(std::in_place_type<Array>, std::from_range, std::forward<R>(range))
		{
		}

		// NOTE: Using Object::value_type causes a compile error
		// Using std::from_range_t to avoid ambiguity with variant passing
		template<typename R>
			requires std::ranges::input_range<R> &&
					 std::convertible_to<std::ranges::range_reference_t<R>, std::pair<const std::string, JSON>>
		JSON(std::from_range_t, R&& range) :
			data_(std::in_place_type<Object>, std::from_range, std::forward<R>(range))
		{
		}

		~JSON() = default;

		JSON& operator=(const JSON& other)	   = default;
		JSON& operator=(JSON&& other) noexcept = default;

		template<typename T>
			requires std::constructible_from<JSON, T> && std::assignable_from<JSON, T>
		auto operator=(T&& other) noexcept -> JSON&
		{
			data_ = std::move(other);
			return *this;
		}

		/**
		 * @brief Extracts a value from the JSON array with a given index. If the key does not exist, a new JSON object
		 *	is returned
		 * @param index The index to extract
		 * @return A new or existing JSON object
		 */
		JSON& operator[](const size_type index)
		{
			if (Null())
			{
				data_.emplace<Array>();
			}

			if (Array* arrayPointer = std::get_if<Array>(&data_)) [[likely]]
			{
				Array& jsonArray = *arrayPointer;

				// If the given index is out of bounds we will resize the array
				if (index >= jsonArray.size())
				{
					jsonArray.resize(index + 1);
				}

				return jsonArray[index];
			}

			throw std::runtime_error("Invalid type found for '[]' operator access");
		}

		/***
		 * @brief Extracts a value from the JSON array with a given index
		 * @param index The index to extract
		 * @return An existing JSON object
		 */
		const JSON& operator[](const size_type index) const
		{
			if (const Array* arrayPointer = std::get_if<Array>(&data_)) [[likely]]
			{
				const Array& jsonArray = *arrayPointer;
				return jsonArray[index];
			}

			throw std::runtime_error("Invalid type found for '[]' operator access");
		}

		/**
		 * @brief Extracts a value from the JSON object with a given key. If the key does not exist, a new JSON object
		 *	is returned
		 * @param key The key to extract
		 * @return A new or existing JSON object
		 */
		JSON& operator[](std::string key)
		{
			if (Null())
			{
				Object& object = data_.emplace<Object>();
				return object[std::move(key)];
			}

			if (Object* objectPointer = std::get_if<Object>(&data_)) [[likely]]
			{
				Object& object = *objectPointer;
				return object[std::move(key)];
			}

			throw std::runtime_error("Invalid type found for '[]' operator access");
		}

		/***
		 * @brief Extracts a value from the JSON object with a given key
		 * @param key The key to extract
		 * @return An existing JSON object
		 */
		const JSON& operator[](const std::string& key) const
		{
			if (const Object* objectPointer = std::get_if<Object>(&data_)) [[likely]]
			{
				const Object& object = *objectPointer;
				const auto iterator	 = object.find(key);

				assert(iterator != object.end());

				return iterator->second;
			}

			throw std::runtime_error("Invalid type found for '[]' operator access");
		}

		// NOTE: Using Array::value_type causes a compile error
		// Using std::from_range_t to avoid ambiguity with variant passing
		template<typename R>
			requires std::ranges::input_range<R> && std::convertible_to<std::ranges::range_reference_t<R>, JSON>
		void AssignRange(R&& range)
		{
			data_.emplace<Array>(std::from_range, std::forward<R>(range));
		}

		// NOTE: Using Object::value_type causes a compile error
		// Using std::from_range_t to avoid ambiguity with variant passing
		template<typename R>
			requires std::ranges::input_range<R> &&
					 std::convertible_to<std::ranges::range_reference_t<R>, std::pair<const std::string, JSON>>
		void AssignRange(R&& range)
		{
			data_.emplace<Object>(std::from_range, std::forward<R>(range));
		}

		bool Null() const

		{
			return std::holds_alternative<std::monostate>(data_);
		}

		std::string Dump() const
		{
			return Dump(*this, 0);
		}

		/**
		 * @brief Parse JSON from an input stream
		 * @param stream The input stream to parse from
		 * @return A JSON object representing the parsed content
		 * @throws std::runtime_error on parse errors
		 */
		static JSON Parse(std::istream& stream)
		{
			return ParseValue(stream);
		}

		/**
		 * @brief Parse JSON from a string
		 * @param input The string to parse
		 * @return A JSON object representing the parsed content
		 * @throws std::runtime_error on parse errors
		 */
		static JSON Parse(std::string_view input)
		{
			std::istringstream stream {std::string(input)};
			return Parse(stream);
		}

		/**
		 * @brief Check if this JSON value is an object
		 */
		bool IsObject() const
		{
			return std::holds_alternative<Object>(data_);
		}

		/**
		 * @brief Check if this JSON value is an array
		 */
		bool IsArray() const
		{
			return std::holds_alternative<Array>(data_);
		}

		/**
		 * @brief Check if this JSON value is a string
		 */
		bool IsString() const
		{
			return std::holds_alternative<std::string>(data_);
		}

		/**
		 * @brief Check if this JSON value is a number (double, int32, or uint32)
		 */
		bool IsNumber() const
		{
			return std::holds_alternative<double>(data_) || std::holds_alternative<std::int32_t>(data_) ||
				   std::holds_alternative<std::uint32_t>(data_);
		}

		/**
		 * @brief Check if this JSON value is a boolean
		 */
		bool IsBool() const
		{
			return std::holds_alternative<bool>(data_);
		}

		/**
		 * @brief Get the string value. Throws if not a string.
		 */
		const std::string& GetString() const
		{
			if (const std::string* str = std::get_if<std::string>(&data_))
			{
				return *str;
			}
			throw std::runtime_error("JSON value is not a string");
		}

		/**
		 * @brief Get the numeric value as double. Throws if not a number.
		 */
		double GetNumber() const
		{
			if (const double* d = std::get_if<double>(&data_))
			{
				return *d;
			}
			if (const std::int32_t* i = std::get_if<std::int32_t>(&data_))
			{
				return static_cast<double>(*i);
			}
			if (const std::uint32_t* u = std::get_if<std::uint32_t>(&data_))
			{
				return static_cast<double>(*u);
			}
			throw std::runtime_error("JSON value is not a number");
		}

		/**
		 * @brief Get the boolean value. Throws if not a boolean.
		 */
		bool GetBool() const
		{
			if (const bool* b = std::get_if<bool>(&data_))
			{
				return *b;
			}
			throw std::runtime_error("JSON value is not a boolean");
		}

		/**
		 * @brief Get the size of an array or object. Throws if neither.
		 */
		size_type Size() const
		{
			if (const Array* arr = std::get_if<Array>(&data_))
			{
				return arr->size();
			}
			if (const Object* obj = std::get_if<Object>(&data_))
			{
				return obj->size();
			}
			throw std::runtime_error("JSON value is not an array or object");
		}

		/**
		 * @brief Check if an object contains a key. Throws if not an object.
		 */
		bool Contains(const std::string& key) const
		{
			if (const Object* obj = std::get_if<Object>(&data_))
			{
				return obj->contains(key);
			}
			throw std::runtime_error("JSON value is not an object");
		}

		/**
		 * @brief Get keys of an object. Throws if not an object.
		 */
		std::vector<std::string> Keys() const
		{
			if (const Object* obj = std::get_if<Object>(&data_))
			{
				std::vector<std::string> keys;
				keys.reserve(obj->size());
				for (const auto& [key, _]: *obj)
				{
					keys.push_back(key);
				}
				return keys;
			}
			throw std::runtime_error("JSON value is not an object");
		}

	private:
		static void SkipWhitespace(std::istream& stream)
		{
			while (stream && std::isspace(stream.peek()))
			{
				stream.get();
			}
		}

		static JSON ParseValue(std::istream& stream)
		{
			SkipWhitespace(stream);

			if (!stream)
			{
				throw std::runtime_error("Unexpected end of input");
			}

			const int ch = stream.peek();

			if (ch == '{')
			{
				return ParseObject(stream);
			}
			if (ch == '[')
			{
				return ParseArray(stream);
			}
			if (ch == '"')
			{
				return ParseString(stream);
			}
			if (ch == 't' || ch == 'f')
			{
				return ParseBool(stream);
			}
			if (ch == 'n')
			{
				return ParseNull(stream);
			}
			if (ch == '-' || std::isdigit(ch))
			{
				return ParseNumber(stream);
			}

			throw std::runtime_error(std::format("Unexpected character: '{}'", static_cast<char>(ch)));
		}

		static JSON ParseObject(std::istream& stream)
		{
			stream.get(); // consume '{'
			SkipWhitespace(stream);

			JSON json;
			json.data_.emplace<Object>();
			Object& obj = std::get<Object>(json.data_);

			if (stream.peek() == '}')
			{
				stream.get();
				return json;
			}

			while (stream)
			{
				SkipWhitespace(stream);

				if (stream.peek() != '"')
				{
					throw std::runtime_error("Expected string key in object");
				}

				std::string key = ParseString(stream).GetString();

				SkipWhitespace(stream);

				if (stream.get() != ':')
				{
					throw std::runtime_error("Expected ':' after object key");
				}

				obj[std::move(key)] = ParseValue(stream);

				SkipWhitespace(stream);

				const int ch = stream.get();
				if (ch == '}')
				{
					break;
				}
				if (ch != ',')
				{
					throw std::runtime_error("Expected ',' or '}' in object");
				}
			}

			return json;
		}

		static JSON ParseArray(std::istream& stream)
		{
			stream.get(); // consume '['
			SkipWhitespace(stream);

			JSON json;
			json.data_.emplace<Array>();
			Array& arr = std::get<Array>(json.data_);

			if (stream.peek() == ']')
			{
				stream.get();
				return json;
			}

			while (stream)
			{
				arr.push_back(ParseValue(stream));

				SkipWhitespace(stream);

				const int ch = stream.get();
				if (ch == ']')
				{
					break;
				}
				if (ch != ',')
				{
					throw std::runtime_error("Expected ',' or ']' in array");
				}
			}

			return json;
		}

		static JSON ParseString(std::istream& stream)
		{
			stream.get(); // consume opening '"'

			std::string result;

			while (stream)
			{
				const int ch = stream.get();

				if (ch == '"')
				{
					return JSON(std::move(result));
				}

				if (ch == '\\')
				{
					const int escaped = stream.get();
					switch (escaped)
					{
						case '"':
							result += '"';
							break;
						case '\\':
							result += '\\';
							break;
						case '/':
							result += '/';
							break;
						case 'b':
							result += '\b';
							break;
						case 'f':
							result += '\f';
							break;
						case 'n':
							result += '\n';
							break;
						case 'r':
							result += '\r';
							break;
						case 't':
							result += '\t';
							break;
						case 'u':
						{
							// Parse 4 hex digits
							char hex[5] = {};
							stream.read(hex, 4);
							const auto codepoint = std::stoul(hex, nullptr, 16);
							// Simple ASCII range handling
							if (codepoint < 128)
							{
								result += static_cast<char>(codepoint);
							}
							else
							{
								// UTF-8 encoding for non-ASCII
								if (codepoint < 0x800)
								{
									result += static_cast<char>(0xC0 | (codepoint >> 6));
									result += static_cast<char>(0x80 | (codepoint & 0x3F));
								}
								else
								{
									result += static_cast<char>(0xE0 | (codepoint >> 12));
									result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
									result += static_cast<char>(0x80 | (codepoint & 0x3F));
								}
							}
							break;
						}
						default:
							throw std::runtime_error(
								std::format("Invalid escape sequence: \\{}", static_cast<char>(escaped)));
					}
				}
				else
				{
					result += static_cast<char>(ch);
				}
			}

			throw std::runtime_error("Unterminated string");
		}

		static JSON ParseNumber(std::istream& stream)
		{
			std::string numStr;
			bool isFloat = false;

			if (stream.peek() == '-')
			{
				numStr += static_cast<char>(stream.get());
			}

			while (stream && std::isdigit(stream.peek()))
			{
				numStr += static_cast<char>(stream.get());
			}

			if (stream.peek() == '.')
			{
				isFloat = true;
				numStr += static_cast<char>(stream.get());
				while (stream && std::isdigit(stream.peek()))
				{
					numStr += static_cast<char>(stream.get());
				}
			}

			if (stream.peek() == 'e' || stream.peek() == 'E')
			{
				isFloat = true;
				numStr += static_cast<char>(stream.get());
				if (stream.peek() == '+' || stream.peek() == '-')
				{
					numStr += static_cast<char>(stream.get());
				}
				while (stream && std::isdigit(stream.peek()))
				{
					numStr += static_cast<char>(stream.get());
				}
			}

			if (isFloat)
			{
				return JSON(std::stod(numStr));
			}

			// Try to parse as integer
			const long long value = std::stoll(numStr);
			if (value >= 0 && value <= std::numeric_limits<std::uint32_t>::max())
			{
				return JSON(static_cast<std::uint32_t>(value));
			}
			if (value >= std::numeric_limits<std::int32_t>::min() &&
				value <= std::numeric_limits<std::int32_t>::max())
			{
				return JSON(static_cast<std::int32_t>(value));
			}
			// Fall back to double for large integers
			return JSON(static_cast<double>(value));
		}

		static JSON ParseBool(std::istream& stream)
		{
			char buf[6] = {};
			if (stream.peek() == 't')
			{
				stream.read(buf, 4);
				if (std::string_view(buf, 4) == "true")
				{
					return JSON(true);
				}
			}
			else
			{
				stream.read(buf, 5);
				if (std::string_view(buf, 5) == "false")
				{
					return JSON(false);
				}
			}
			throw std::runtime_error("Invalid boolean literal");
		}

		static JSON ParseNull(std::istream& stream)
		{
			char buf[5] = {};
			stream.read(buf, 4);
			if (std::string_view(buf, 4) == "null")
			{
				return JSON(nullptr);
			}
			throw std::runtime_error("Invalid null literal");
		}
		static std::string Dump(const JSON& json, std::size_t indentCount)
		{
			auto dataVisitor = Overload {
				[&](const std::monostate&)
				{
					return std::format("null");
				},
				[&](const bool& value)
				{
					return std::format("{}", value ? "true" : "false");
				},
				[&](const double& value)
				{
					return std::format("{}", value);
				},
				[&](const std::int32_t& value)
				{
					return std::format("{}", value);
				},
				[&](const std::uint32_t& value)
				{
					return std::format("{}", value);
				},
				[&](const std::string& value)
				{
					return std::format("\"{}\"", value);
				},
				[&](const Array& data)
				{
					if (data.empty())
					{
						return std::format("[]");
					}

					std::string result = std::format("[\n");

					const std::size_t newIndentCount = indentCount + 1;

					for (const auto& element: data | std::views::take(data.size() - 1))
					{
						result += std::format("{:\t>{}}", "", newIndentCount);
						result += Dump(element, newIndentCount);
						result += std::format(",\n");
					}

					// Last element
					{
						result += std::format("{:\t>{}}", "", newIndentCount);
						result += Dump(data.back(), newIndentCount);
					}

					result += std::format("\n");
					result += std::format("{:\t>{}}", "", indentCount);
					result += std::format("]");

					return result;
				},
				[&](const Object& data)
				{
					if (data.empty())
					{
						return std::format("{{}}");
					}

					std::string result = std::format("{{\n");

					const std::size_t newIndentCount = indentCount + 1;

					for (const auto& [key, value]: data | std::views::take(data.size() - 1))
					{
						result += std::format("{:\t>{}}", "", newIndentCount);

						result += std::format("\"{}\": ", key);
						result += Dump(value, newIndentCount);

						result += std::format(",\n");
					}

					// Last element
					{
						result += std::format("{:\t>{}}", "", newIndentCount);

						result += std::format("\"{}\": ", data.rbegin()->first);
						result += Dump(data.rbegin()->second, newIndentCount);
					}

					result += std::format("\n");
					result += std::format("{:\t>{}}", "", indentCount);
					result += std::format("}}");

					return result;
				}};

			return std::visit(dataVisitor, json.data_);
		}

		Value data_;
	};

	export std::ostream& operator<<(std::ostream& stream, const JSON& json)
	{
		stream << json.Dump();
		return stream;
	}
}
