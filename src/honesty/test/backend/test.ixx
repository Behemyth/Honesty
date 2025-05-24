export module synodic.honesty.test.backend:test;

import std;

import function_ref;
import inplace_vector;

import :requirements;
import :tag;

namespace honesty::test
{
	export class Test;
	export using Generator = std::generator<Test>;

	// TODO: Make this a compile-time check
	export constexpr void VerifyTestName(const std::string_view name)
	{
		// Empty check
		if (name.empty())
		{
			throw std::runtime_error(std::format("Empty suite name"));
		}

		// Uppercase check
		std::ranges::for_each(
			name,
			[name](const char character)
			{
				// Uppercase check
				if (std::isupper(character))
				{
					throw std::runtime_error(std::format("Uppercase suite name '{}'", name));
				}

				// Space check
				if (std::isspace(character))
				{
					throw std::runtime_error(std::format("Suite name '{}' contains spaces", name));
				}

				// Check for non-alphanumeric characters, excluding '_'
				if (!std::isalnum(character) && character != '_')
				{
					throw std::runtime_error(std::format("Non-alphanumeric suite name '{}'", name));
				}
			});
	}

	// Forward declaration
	export
	class TestData;

	class Test final
	{
		using VariantType = std::variant<std::function_ref<void(const Requirements&)>, std::function_ref<Generator()>>;

	public:
		constexpr Test(
			const std::string_view name,
			const Tag& tag,
			const std::function_ref<void(const Requirements&)>& test,
			std::string_view description = "") :
			name_(name),
			test_(test),
			tag_(tag)
		{
			VerifyTestName(name);
		}

		constexpr Test(
			const std::string_view name,
			const Tag& tag,
			const std::function_ref<Generator()>& test,
			std::string_view description = "") :
			name_(name),
			test_(test),
			tag_(tag)
		{
			VerifyTestName(name);
		}

		constexpr Test(const Test& other)                = delete;
		constexpr Test(Test&& other) noexcept            = delete;
		constexpr Test& operator=(const Test& other)     = delete;
		constexpr Test& operator=(Test&& other) noexcept = delete;

		std::string_view Name() const
		{
			return name_;
		}

		std::span<const Tag::value_type> Tags() const
		{
			return tag_.View();
		}


		constexpr Test operator<<(std::string_view description) const
		{
			return Test(name_, test_, tag_, description);
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
