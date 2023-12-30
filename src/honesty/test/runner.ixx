
export module synodic.honesty.test:runner;

import :test;
import std;

export namespace synodic::honesty
{

	template<typename T>
	concept is_runner = requires(T type, const T constType, suite_data data) {
		{
			constType.run()
		} -> std::same_as<void>;

		{
			type.submit(data)
		} -> std::same_as<void>;
	};

}
