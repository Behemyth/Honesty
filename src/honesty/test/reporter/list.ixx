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
		explicit(false) ListReporter(const ListReporterParameters& parameters, log::Logger logger) :
			CumulativeAdapter(std::move(logger)),
			parameters_(parameters)
		{
		}

		~ListReporter() override = default;

		consteval std::string_view Name() const
		{
			return "list";
		}

		void Finalize(CumulativeData data) override
		{
			switch (parameters_.outputType)
			{
				case ListOutputType::LOG :
				{
					log::Console sink;
					const log::Logger& logger = Logger();

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

