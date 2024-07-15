export module synodic.honesty.test.types:suite;

import std;

import :fixture;
import function_ref;
import :test;

namespace synodic::honesty::test
{
	/**
	 * @brief Allows the static registration of tests in the global scope. Constructed at compile-time so that test
	 * registration is constrained. We don't want a suite wrapper to be a generic fixture that can contain expensive
	 * calculations. All tests are known up-front and parameterized tests are expanded at run-time
	 */
	export template<size_t LiteralSize>
	class Suite final
	{
		static_assert(LiteralSize > 0, "LiteralSize must be greater than 0");

		static constexpr size_t NAME_SIZE = LiteralSize - 1;

		using VariantType = std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>>;

	public:
		consteval Suite(const char (&name)[LiteralSize], const std::function_ref<Generator()> generator) :
			generator_(generator)
		{
			std::copy(std::begin(name), std::end(name) - 1, std::begin(name_));
		}

		consteval Suite(const char (&name)[LiteralSize], const std::function_ref<Generator(Fixture&)> generator) :
			generator_(generator)
		{
			std::copy(std::begin(name), std::end(name) - 1, std::begin(name_));
		}

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = delete;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = delete;

		// TODO: Make consteval when MSVC supports it
		constexpr std::string_view Name() const
		{
			return {name_.data(), name_.size()};
		}

	private:
		friend class SuiteData;

		std::array<char, NAME_SIZE> name_;
		VariantType generator_;
	};

	export class SuiteData
	{
		using VariantType = std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>>;

	public:

		template<size_t LiteralSize>
		constexpr SuiteData(
			const Suite<LiteralSize>& suite) :
			nameView_(suite.name_),
			generator_(suite.generator_)
		{
		}

		// TODO: Make consteval when MSVC supports it
		constexpr std::string_view Name() const
		{
			return nameView_;
		}

		constexpr VariantType Variant() const
		{
			return generator_;
		}

	private:
		std::string_view nameView_;
		VariantType generator_;
	};
}
