export module synodic.honesty.test:reporter.default;

import synodic.honesty.log;
import :reporter;
import std;

namespace synodic::honesty::test
{
	export class DefaultReporter final : public StreamingAdapter
	{
	public:
		explicit DefaultReporter(log::Logger logger) :
			StreamingAdapter(std::move(logger))
		{
		}

		~DefaultReporter() override = default;

		static consteval std::string_view Name()
		{
			return "default";
		}

		void Signal(const event::SuiteBegin& event) override
		{
		}

		void Signal(const event::TestBegin& event) override
		{
		}

		void Signal(const event::AssertionPass& event) override
		{
			++passCount_;
		}

		void Signal(const event::AssertionFail& event) override
		{
			++failCount_;

			std::string styledResult = format(log::TextStyle(log::color24_t(255, 0, 0)), "Failed");
			Logger()
				.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());
		}

		void Signal(const event::Summary& event) override
		{
			std::string passedStyled = format(log::TextStyle(log::color24_t(0, 255, 0)), "Passed");
			std::string failedStyled = format(log::TextStyle(log::color24_t(255, 0, 0)), "Failed");

			Logger().Info("{} Assertions {}\n{} Assertions {}", passCount_, passedStyled, failCount_, failedStyled);
		}

	private:
		std::size_t passCount_ = 0;
		std::size_t failCount_ = 0;
	};

}
