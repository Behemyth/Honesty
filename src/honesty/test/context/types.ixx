export module synodic.honesty.test.context:types;

import std;

import function_ref;

namespace synodic::honesty::test
{
	// Forward declarations that will be defined in the `synodic.honesty.test` module
	class Fixture;
	class Generator;
	class Requirements;

	export struct TestData
	{
		using VariantType = std::
			variant<std::function_ref<void(const Requirements&)>, std::function_ref<Generator(const Requirements&)>>;

		constexpr TestData(const std::string_view name, VariantType test) :
			name_(name),
			test_(test)
		{
		}

		std::string_view name_;
		VariantType test_;
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
