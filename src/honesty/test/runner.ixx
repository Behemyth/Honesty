
export module synodic.honesty.test:runner;

import :test;
import std;

export namespace synodic::honesty
{

	template<typename T>
	concept is_runner = requires(T const type, std::span<suite_data> suites) {
		{
			type.run(suites)
		} -> std::same_as<void>;
	};

}
