export module synodic.honesty.trace:provider;

import std;

import :tracer;
import zstring_view;

namespace honesty::trace
{
	/**
	 * @brief Entrypoint for creating Tracers
	 */
	export
	template<std::uint8_t N>
	class Provider
	{
	public:
		template<typename... Names>
		explicit consteval Provider(Names... names)
			requires ((std::same_as<Names, std::zstring_view> && ...)) :
			tracers_{Tracer(names)...}
		{
		}

	private:
		std::array<Tracer, N> tracers_;
	};

	// Deduction guide to infer N from the number of names
	export
	template<typename... Names>
	Provider(Names...) -> Provider<sizeof...(Names)>;
}
