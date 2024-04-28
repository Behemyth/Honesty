export module synodic.honesty.test:reporter.list;

import synodic.honesty.log;
import :reporter;
import std;

namespace synodic::honesty::test
{
	export class ListReporter final : public CumulativeAdapter
	{
	public:
		explicit(false) constexpr ListReporter(const std::string_view name) :
			CumulativeAdapter(name)
		{
		}

		~ListReporter() override = default;

		void Finalize() override
		{
		}
	};
}
