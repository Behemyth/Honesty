export module synodic.honesty.test.backend:data;

import std;

import function_ref;

import :assert;
import :fixture;

import synodic.honesty.test.types;

namespace synodic::honesty::test
{
	export class TestData
	{
	public:
		constexpr TestData(const Test& test) :
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

	private:
		const Test& test_;
	};

	export struct SuiteData
	{
		consteval SuiteData(
			const std::string_view name,
			std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>> testGenerator) :
			name(name),
			testGenerator(testGenerator)
		{
		}

		std::string_view name;
		std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>> testGenerator;
	};
}
