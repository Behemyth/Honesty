export module synodic.honesty.test:suite;

import :types;
import :test;

namespace synodic::honesty::test
{

	/**
	 * @brief Allows the static registration of tests in the global scope
	 */
	export template<size_t Size>
	class Suite final : SuiteData
	{
	public:
		consteval Suite(const char (&name)[Size], std::function_ref<void()> generator);

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = delete;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = delete;

		bool Register() &
		{
			SuiteData& data = *this;
			GetRegistry().AddSuite(data);

			return true;
		}

	private:
		std::array<char, Size> name_;
	};

	template<size_t Size>
	consteval Suite<Size>::Suite(const char (&name)[Size], std::function_ref<void()> generator) :
		name_ {0},
		SuiteData(std::string_view(name_.data(), Size), generator)
	{
		std::copy_n(name, Size, name_.begin());
	}

	/**
	 * @brief Registers a suite of tests with the global registry.
	 * @param ...suites Suites to be registered. Each are expected to be an l-value reference.
	 * @return TODO
	 */
	export template<size_t... Sizes>
	bool Register(Suite<Sizes>&... suites)
	{
		(suites.Register(), ...);
		return true;
	}

}
