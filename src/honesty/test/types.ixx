export module synodic.honesty.test:types;

import std;
import function_ref;
import :generator;

namespace synodic::honesty::test
{
	export class TestBase
	{
	public:
		virtual ~TestBase()		 = default;
		virtual void Run() const = 0;

		virtual std::span<std::string_view> Tags() const = 0;

		virtual std::string_view Name() const = 0;

	private:
	};

	class SuiteData
	{
	public:
		consteval SuiteData(std::string_view name, std::function_ref<synodic::honesty::test::generator<TestBase>()> generator) noexcept;

		std::string_view name;
		std::function_ref<synodic::honesty::test::generator<TestBase>()> generatorWrapper;
	};

	consteval SuiteData::SuiteData(
		std::string_view name,
		std::function_ref<synodic::honesty::test::generator<TestBase>()> generator) noexcept :
		name(name),
		generatorWrapper(generator)
	{
	}
}
