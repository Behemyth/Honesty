export module synodic.honesty.test:suite;

import :types;
import :test;

namespace synodic::honesty::test
{

	class SuiteData
	{
	public:
		consteval SuiteData(const std::string_view name, const std::function_ref<Generator()> generator) :
			name(name),
			generator(generator)
		{
		}

		std::string_view name;
		std::function_ref<Generator()> generator;
	};

	/**
	 * @brief Allows the static registration of tests in the global scope
	 */
	export template<size_t Size>
	class Suite final : public SuiteData
	{
	public:
		consteval Suite(const char (&name)[Size], std::function_ref<Generator()> generator) :
			SuiteData({name_.data(), name_.size()}, generator),
			name_ {0}
		{
			std::copy_n(name, Size, name_.begin());
		}

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = delete;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = delete;

	private:
		std::array<char, Size> name_;
	};
}
