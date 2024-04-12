
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
			Configuration();

			std::string_view defaultRunner; // The default runner to use if none is specified
			std::string_view defaultReporter; // The default reporter to use if none is specified
		};

		// Resolve all input into immediately executable state ready for the 'Execute' function
		Instance(const Configuration& configuration, std::span<std::string_view> arguments);

		void Execute() const;

	private:
		// Our list of top level commands and the parameters that go with them
		std::variant<HelpParameters, ExecuteParameters, ListParameters> parameters_;
	};

}
