export module synodic.honesty.test:suite;

import synodic.honesty.utility;
import :types;
import :test;
import :fixture;
import inplace_vector;

namespace synodic::honesty::test
{
	template<typename T, typename R, typename... Args>
	concept invocable_r = std::invocable<T, Args...> && requires(Args&&... args) {
		{
			invoke(forward<Args>(args...))
		} -> std::convertible_to<R>;
	};

	struct SuiteView;

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

	public:
		consteval Suite(const char (&name)[LiteralSize], const std::function_ref<Generator()> generator) :
			testGenerator_(generator)
		{
			std::copy(std::begin(name), std::end(name) - 1, std::begin(name_));
		}

		consteval Suite(const char (&name)[LiteralSize], const std::function_ref<Generator(Fixture&)> generator) :
			testGenerator_(generator)
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
		friend SuiteView;

		std::array<char, NAME_SIZE> name_;
		std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>> testGenerator_;
	};

	struct SuiteView
	{
		constexpr SuiteView() :
			testGenerator(
				[]() -> Generator
				{
					co_return;
				}) {};

		template<size_t NameSize>
		explicit(false) constexpr SuiteView(const Suite<NameSize>& suite) :
			name(suite.name_),
			testGenerator(suite.testGenerator_)
		{
		}

		std::string_view name;
		std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>> testGenerator;
	};
}

namespace
{
	constinit std::inplace_vector<synodic::honesty::test::SuiteView, 50> SUITES;
}

namespace synodic::honesty::test
{

	std::span<SuiteView> GetSuites()
	{
		return SUITES;
	}

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
				SUITES,
				name,
				[](const SuiteView& view)
				{
					return view.name;
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

	/**
	 * @brief Don't export. Keeps the SUITES global variable internally linked to the test module
	 */
	void AddSuite(SuiteView suite)
	{
		VerifySuiteName(suite.name);

		SUITES.push_back(std::move(suite));
	}

	export template<size_t... NameSizes>
	class SuiteRegistrar
	{
	public:
		explicit SuiteRegistrar(const Suite<NameSizes>&... suites)
		{
			(AddSuite(SuiteView(suites)), ...);
		}
	};
}
