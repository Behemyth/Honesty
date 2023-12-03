
export module synodic.honesty.test.runner;
export import :single;

import std;

export namespace synodic::honesty
{

	template<typename T>
	concept is_runner = requires(T type, std::span<std::move_only_function<void()>> generator) {
		{
			type.run(generator)
		} -> std::same_as<void>;
	};

}
