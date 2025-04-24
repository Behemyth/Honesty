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
		explicit consteval Provider(const std::zstring_view rootName, Names... names)
			requires std::conjunction_v<std::is_same<Names, std::zstring_view>...>:
			tracers_{Tracer(rootName) ,Tracer(names)...}
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

namespace honesty
{
	//export
	//using Tracer = 
}
