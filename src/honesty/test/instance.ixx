
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
		// Our list of top level commands and the parameters that go with them
		std::variant<ExecuteParameters, ListParameters> parameters_;
	};

}
