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

			const log::Logger& logger = Logger();

			std::string styledResult = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");
			logger.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());

			if (not event.reason.empty())
			{
				logger.Info("    {}", event.reason);
			}
		}

		void Signal(const event::Summary& event) override
		{
			std::string passedStyled = format(log::TextStyle(log::Colour24(0, 255, 0)), "Passed");
			std::string failedStyled = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");

			Logger().Info("{} Assertions {}", passCount_, passedStyled);

			if (failCount_)
			{
				Logger().Info("{} Assertions {}", failCount_, failedStyled);
			}
		}

	private:
		std::size_t passCount_ = 0;
		std::size_t failCount_ = 0;
	};

}
