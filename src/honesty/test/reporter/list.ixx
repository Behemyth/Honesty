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
	};

	class ListReporter final : public CumulativeAdapter
	{
	public:
		explicit(false) ListReporter(const ListReporterParameters& parameters, const log::Logger& logger) :
			CumulativeAdapter(logger),
			parameters_(parameters)
		{
		}

		~ListReporter() override = default;

		static consteval std::string_view Name()
		{
			return "list";
		}

		void Finalize(CumulativeData data) override
		{
			data_ = std::move(data);
		}

		const CumulativeData& Data() const
		{
			return data_;
		}

	private:
		ListReporterParameters parameters_;
		CumulativeData data_;
	};
}

