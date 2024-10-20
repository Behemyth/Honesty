export module synodic.honesty.test.backend:test;

import std;

import generator;
import function_ref;
import inplace_vector;

import :requirements;
import :tag;

namespace synodic::honesty::test
{
	export class Test;
	export using Generator = std::generator<Test>;

	// TODO: Make this a compile-time check
	export constexpr void VerifyTestName(const std::string_view name)
	{
		// Empty check
		if (name.empty())
		{
			throw std::runtime_error(std::format(
				"Empty suite name - {}, {}",
				std::source_location::current().file_name(),
				std::source_location::current().line()));
		}

		// Uppercase check
		std::ranges::for_each(
			name,
			[](const char character)
			{
				// Uppercase check
				if (std::isupper(character))
				{
					throw std::runtime_error(std::format(
						"Uppercase suite name - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}

				// Space check
				if (std::isspace(character))
				{
					throw std::runtime_error(std::format(
						"Suite name contains spaces - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}

				// Check for non-alphanumeric characters, excluding '_'
				if (!std::isalnum(character) && character != '_')
				{
					throw std::runtime_error(std::format(
						"Non-alphanumeric suite name - {}, {}",
						std::source_location::current().file_name(),
						std::source_location::current().line()));
				}
			});
	}

	// Forward declaration
	export class TestData;

	class Test final
	{
		using VariantType = std::
			variant<std::function_ref<void(const Requirements&)>, std::function_ref<Generator(const Requirements&)>>;

	public:
		constexpr Test(
			const std::string_view name,
			const Tag& tag,
			const std::function_ref<void(const Requirements&)>& test) :
			name_(name),
			test_(test),
			tag_(tag)
		{
			VerifyTestName(name);
		}

		constexpr Test(
			const std::string_view name,
			const Tag& tag,
			const std::function_ref<Generator(const Requirements&)>& test) :
			name_(name),
			test_(test),
			tag_(tag)
		{
			VerifyTestName(name);
		}

		Test(const Test& other)				   = delete;
		Test(Test&& other) noexcept			   = delete;
		Test& operator=(const Test& other)	   = delete;
		Test& operator=(Test&& other) noexcept = delete;

		std::string_view Name() const
		{
			return name_;
		}

		std::span<const Tag::value_type> Tags() const
		{
			return tag_.View();
		}

	private:
		friend TestData;

		std::string_view name_;
		VariantType test_;

		Tag tag_;
	};

	/**
	 * @brief Data that can be extracted from a test object
	 */
	class TestData
	{
	public:
		explicit constexpr TestData(const Test& test) :
			test_(test)
		{
		}

		std::string_view Name() const
		{
			return test_.name_;
		}

		Test::VariantType Variant() const
		{
			return test_.test_;
		}

		const Tag& Tag() const
		{
			return test_.tag_;
		}

	private:
		const Test& test_;
	};

}
