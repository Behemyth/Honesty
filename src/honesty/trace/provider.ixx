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
			requires std::conjunction_v<std::is_convertible<Names, std::zstring_view>...>:
			tracers_{Tracer(rootName), Tracer(names)...}
		{
		}

		consteval const Tracer& Get(const std::zstring_view name) const
		{
			for (const Tracer& tracer: tracers_)
			{
				// Assuming Tracer has a method or member to get its label for comparison
				if (tracer.Label() == name)
				{
					return tracer;
				}
			}
			throw std::runtime_error("Tracer with the given name not found");
		}

	private:
		std::array<Tracer, N> tracers_;
	};

	// Deduction guide to infer N from the number of names
	export
	template<typename... Names>
	Provider(std::zstring_view, Names...) -> Provider<1 + sizeof...(Names)>;
}

namespace honesty
{
	//export
	//using Tracer = 
}
