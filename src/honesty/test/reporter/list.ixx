export module synodic.honesty.test:reporter.list;

import synodic.honesty.log;
import :reporter;
import std;
import :types;

namespace synodic::honesty::test
{

	struct ListReporterParameters
	{
		ListReporterParameters() = default;

		ListOutputType outputType;
	};

	class ListReporter final : public CumulativeAdapter
	{
	public:
		explicit(false) ListReporter(const ListReporterParameters& parameters) :
			CumulativeAdapter("list"),
			parameters_(parameters)
		{
		}

		~ListReporter() override = default;

		void Finalize(CumulativeData data) override
		{
			switch (parameters_.outputType)
			{
				case ListOutputType::LOG :
				{
					log::Console sink;
					log::Logger& logger = log::GetLogger("reporter");

					break;
				}
				case ListOutputType::JSON :
				{
				}
				break;
			}
		}

	private:
		ListReporterParameters parameters_;
	};
}

