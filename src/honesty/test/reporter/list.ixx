export module synodic.honesty.test:reporter.list;

import synodic.honesty.log;
import :reporter;
import std;
import :types;

namespace synodic::honesty::test
{
	class ListReporter final : public CumulativeAdapter
	{
	public:
		explicit(false) ListReporter(const log::Logger& logger) :
			CumulativeAdapter(logger)
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
		CumulativeData data_;
	};
}
