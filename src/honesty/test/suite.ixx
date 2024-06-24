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
	export template<size_t NameSize>
	class Suite final
	{
	public:
		consteval Suite(const char (&name)[NameSize], const std::function_ref<Generator()> generator) :
			name_(name),
			testGenerator_(generator)
		{
		}

		consteval Suite(const char (&name)[NameSize], const std::function_ref<Generator(Fixture&)> generator) :
			name_(name),
			testGenerator_(generator)
		{
		}

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = delete;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = delete;

		consteval std::string_view Name() const
		{
			return name_;
		}

	private:
		friend SuiteView;

		utility::FixedString<NameSize> name_;
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

	/**
	 * @brief Don't export. Keeps the SUITES global variable internally linked to the test module
	 */
	void AddSuite(SuiteView suite)
	{
		SUITES.push_back(std::move(suite));
	}

	export template<size_t... NameSizes>
	class SuiteRegistrar
	{
	public:
		explicit SuiteRegistrar(Suite<NameSizes>&... suites)
		{
			(AddSuite(SuiteView(suites)), ...);
		}
	};
}
