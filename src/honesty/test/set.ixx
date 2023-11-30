export module synodic.honesty.test:set;

import std;

import :generator;
import :test;

export namespace synodic::honesty
{

	// Note, not constexpr, as a coroutine cant be constexpr

	template<std::invocable Fn>
	auto Set(std::string_view name, Fn&& generator)
	{
		return std::ranges::elements_of(generator());
	}

	/**
	 * @brief Strongly typed definition around string_view with construction 
	 */
	class [[nodiscard]] SetName
	{
	public:
		consteval SetName(std::string_view name) :
			name_(name) {

			};

		SetName(const SetName& other)				 = delete;
		SetName(SetName&& other) noexcept			 = delete;
		SetName& operator=(const SetName& other)	 = delete;
		SetName& operator=(SetName&& other) noexcept = delete;

		template<std::invocable Fn>
		auto operator=(Fn&& generator)
		{
			return Set(name_, generator);
		}

	protected:
		std::string_view name_;
	};

}
