export module synodic.honesty.test.backend:data;

import std;

import function_ref;

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

	export class SuiteData
	{
		using VariantType = std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>>;

	public:
		consteval SuiteData(
			const std::string_view name,
			std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>> testGenerator) :
			nameView(name),
			testGenerator(testGenerator)
		{
		}

		// TODO: Make consteval when MSVC supports it
		constexpr std::string_view Name() const
		{
			return nameView;
		}

		constexpr VariantType Variant() const
		{
			return testGenerator;
		}

	private:
		std::string_view nameView;
		VariantType testGenerator;
	};
}
