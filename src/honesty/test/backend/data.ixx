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
			std::variant<std::function_ref<Generator()>, std::function_ref<Generator(Fixture&)>> generator) :
			nameView_(name),
			generator_(generator)
		{
		}

		// TODO: Make consteval when MSVC supports it
		constexpr std::string_view Name() const
		{
			return nameView_;
		}

		constexpr VariantType Variant() const
		{
			return generator_;
		}

	private:
		std::string_view nameView_;
		VariantType generator_;
	};
}
