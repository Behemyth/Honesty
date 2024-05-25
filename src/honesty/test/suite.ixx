export module synodic.honesty.test:suite;

import :types;
import :test;
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
			name_ {0},
			testGenerator_(generator)
		{
			std::copy_n(name, NameSize, name_.begin());
		}

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = delete;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = delete;

	private:
		friend SuiteView;

		std::array<char, NameSize> name_;
		std::function_ref<Generator()> testGenerator_;
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
		std::function_ref<Generator()> testGenerator;
	};
}

namespace
{
	constinit std::inplace_vector<synodic::honesty::test::SuiteView, 50> SUITES;
}

namespace synodic::honesty::test
{
	export struct RegisterResult
	{
		RegisterResult() = default;
	};

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
