
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

		// Resolved all input into immediately executable state ready for the 'Execute' function
		Instance(const Configuration& configuration, std::span<std::string_view> arguments);

		void Execute() const;

	private:

		Interface::Command command_;
		Interface::Configuration configuration_;
	};

}
