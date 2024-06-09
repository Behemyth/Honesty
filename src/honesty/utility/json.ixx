/*
 *	This module provides a simple interface for reading and writing JSON files. The implementation is not meant to
 *		compete with the ergonomics or the efficiency of other libraries. Instead, it is meant to provide the small
 *		utility that are need for JSON interfaces while not depending on other libraries.
 */

export module synodic.honesty.utility:json;

import std;
import <cassert>;

namespace
{
	// A helper for variant overload deduction
	template<typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};
}

namespace synodic::honesty::utility
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

		JSON()	= default;
		~JSON() = default;

		JSON(const JSON& other)				   = default;
		JSON(JSON&& other) noexcept			   = default;
		JSON& operator=(const JSON& other)	   = default;
		JSON& operator=(JSON&& other) noexcept = default;

		template<typename T>
			requires std::constructible_from<Value, T>
		explicit(false) JSON(const T& other) :
			value_(other)
		{
		}

		template<typename T>
			requires std::constructible_from<Value, T>
		explicit(false) JSON(T&& other) noexcept :
			value_(std::move(other))
		{
		}

		/**
		 * @brief Extracts a value from the JSON array with a given index. If the key does not exist, a new JSON object
		 *	is returned
		 * @param index The index to extract
		 * @return A new or existing JSON object
		 */
		JSON& operator[](const size_type index)
		{
			if (std::holds_alternative<std::monostate>(value_))
			{
				value_.emplace<Array>();
			}

			if (Array* arrayPointer = std::get_if<Array>(&value_)) [[likely]]
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
			if (const Array* arrayPointer = std::get_if<Array>(&value_)) [[likely]]
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
			if (std::holds_alternative<std::monostate>(value_))
			{
				Object& object = value_.emplace<Object>();
				return object[std::move(key)];
			}

			if (Object* objectPointer = std::get_if<Object>(&value_)) [[likely]]
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
			if (const Object* objectPointer = std::get_if<Object>(&value_)) [[likely]]
			{
				const Object& object = *objectPointer;
				const auto iterator	 = object.find(key);

				assert(iterator != object.end());

				return iterator->second;
			}

			throw std::runtime_error("Invalid type found for '[]' operator access");
		}

		template<typename T>
			requires std::constructible_from<Value, T>
		explicit operator T() const
		{
			return std::get<T>(value_);
		}

		template<typename T>
			requires std::convertible_to<Value, T>
		explicit operator const T&() const
		{
			return std::get<T>(value_);
		}

		std::string Dump() const
		{
			return Dump(*this, 0);
		}

	private:
		static std::string Dump(const JSON& json, std::size_t indentCount)
		{
			auto executor = Overload {
				[&](const std::monostate& value)
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
				[&](const Array& value)
				{
					if (value.empty())
					{
						return std::format("[]");
					}

					std::string result = std::format("[\n");

					const std::size_t newIndentCount = indentCount + 1;

					for (const auto& element: value | std::views::take(value.size() - 1))
					{
						result += std::format("{:\t>{}}", "", newIndentCount);
						result += Dump(element, newIndentCount);
						result += std::format(",\n");
					}

					// Last element
					{
						result += std::format("{:\t>{}}", "", newIndentCount);
						result += Dump(value.back(), newIndentCount);
					}

					result += std::format("\n");
					result += std::format("{:\t>{}}", "", indentCount);
					result += std::format("]");

					return result;
				},
				[&](const Object& value)
				{
					if (value.empty())
					{
						return std::format("{{}}");
					}

					std::string result = std::format("{{\n");

					const std::size_t newIndentCount = indentCount + 1;

					for (const auto& element: value | std::views::take(value.size() - 1))
					{
						result += std::format("{:\t>{}}", "", newIndentCount);

						result += std::format("\"{}\": ", element.first);
						result += Dump(element.second, newIndentCount);

						result += std::format(",\n}}");
					}

					// Last element
					{
						result += std::format("{:\t>{}}", "", newIndentCount);

						result += std::format("\"{}\": ", value.rbegin()->first);
						result += Dump(value.rbegin()->second, newIndentCount);
					}

					result += std::format("\n");
					result += std::format("{:\t>{}}", "", indentCount);
					result += std::format("}}");

					return result;
				}};

			return std::visit(executor, json.value_);
		}

		Value value_;
	};

	export std::ostream& operator<<(std::ostream& stream, const JSON& json)
	{
		stream << json.Dump();
		return stream;
	}
}
