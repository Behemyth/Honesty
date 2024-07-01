module synodic.honesty.test:command.types;

import std;

namespace synodic::honesty::test::command
{
	class Command
	{
	public:
		constexpr Command() = default;
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) noexcept = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) noexcept = delete;

		virtual void Parse(std::span<std::string_view> arguments) = 0;
		virtual void Process() = 0;

	private:
	};

	template<typename T>
	concept command = requires()
	{
		// The command must have a name, and it must be a string_view
		{
			T::NAME
		} -> std::same_as<std::string_view>;

		// The name must be constant
		{ std::bool_constant<T::NAME>() } -> std::same_as<std::true_type>;

		// Must be derived from the Command class
		std::derived_from<T, Command>;
	};
}
