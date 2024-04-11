
export module synodic.honesty.test:instance;

import std;
import :interface;

namespace synodic::honesty::test
{

	export class Instance
	{
	public:
		struct Configuration
		{
			Configuration() = default;
		};

		Instance(Configuration& configuration, std::span<std::string_view> arguments);

	private:
		Interface interface_;
	};

}
