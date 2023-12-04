
export module synodic.honesty.test:runner;

import :test;
import std;

export namespace synodic::honesty
{

	template<typename T>
	concept is_runner = requires(T const type, std::span<std::move_only_function<Generator()>> generator) {
		{
			type.run(generator)
		} -> std::same_as<void>;
	};

}
