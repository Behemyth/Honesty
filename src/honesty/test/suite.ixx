export module synodic.honesty.test:suite;

import synodic.honesty.utility;
import synodic.honesty.test.backend;

import :test;

namespace synodic::honesty::test
{
	// Forward declarations that will be defined elsewhere in the `synodic.honesty.test` module
	class Fixture;

	template<typename T, typename R, typename... Args>
	concept invocable_r = std::invocable<T, Args...> && requires(Args&&... args) {
		{
			invoke(forward<Args>(args...))
		} -> std::convertible_to<R>;
	};

	/**
	 * @brief Allows the static registration of tests in the global scope. Constructed at compile-time so that test
	 * registration is constrained. We don't want a suite wrapper to be a generic fixture that can contain expensive
	 * calculations. All tests are known up-front and parameterized tests are expanded at run-time
	 */
	export template<size_t LiteralSize>
	class Suite final : SuiteData
	{
		static_assert(LiteralSize > 0, "LiteralSize must be greater than 0");

		static constexpr size_t NAME_SIZE = LiteralSize - 1;

	public:
		consteval Suite(const char (&name)[LiteralSize], const std::function_ref<Generator()> generator) :
			SuiteData({name_.data(), name_.size()}, generator)
		{
			std::copy(std::begin(name), std::end(name) - 1, std::begin(name_));
		}

		consteval Suite(const char (&name)[LiteralSize], const std::function_ref<Generator(Fixture&)> generator) :
			SuiteData({name_.data(), name_.size()}, generator)
		{
			std::copy(std::begin(name), std::end(name) - 1, std::begin(name_));
		}

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = delete;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = delete;

	private:
		std::array<char, NAME_SIZE> name_;
	};

	// TODO: Make this a compile-time check
	export void VerifySuiteName(const std::string_view name)
	{
		// Empty check
		if (name.empty())
		{
			throw std::runtime_error(std::format(
				"Empty suite name - {}, {}",
				std::source_location::current().file_name(),
				std::source_location::current().line()));
		}

		// Duplicate check
		if (std::ranges::contains(
				GetSuites(),
				name,
				[](const SuiteData& view)
				{
					return view.Name();
				}))
		{
			throw std::runtime_error(std::format(
				"Duplicate suite name - {}, {}",
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

	export template<size_t... NameSizes>
	class SuiteRegistrar
	{
	public:
		explicit SuiteRegistrar(const Suite<NameSizes>&... suites)
		{
			(VerifySuiteName(suites.Name()), ...);
			(AddSuite(suites), ...);
		}
	};
}
