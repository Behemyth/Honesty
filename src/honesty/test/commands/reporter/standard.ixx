export module synodic.honesty.test.commands:reporter.standard;

import synodic.honesty.log;
import synodic.honesty.test;
import std;

namespace
{
	constexpr auto SUCCESS	 = synodic::honesty::log::TextStyle(synodic::honesty::log::Colour24(0, 255, 0));
	constexpr auto FAILURE	 = synodic::honesty::log::TextStyle(synodic::honesty::log::Colour24(255, 0, 0));
	constexpr auto HIGHLIGHT = synodic::honesty::log::TextStyle(synodic::honesty::log::Colour24(255, 255, 0));
}

namespace synodic::honesty::test
{

	/**
	 * @brief The state required for the lifetime of a test
	 */
	class TestState
	{
	public:
		TestState(const ExpectedOutcome outcome, const log::Logger& logger) :
			testOutcome_(outcome),
			logger_(logger)
		{
		}

		void Signal(const event::AssertionPass& event)
		{
		}

		void Signal(const event::AssertionFail& event)
		{
			ReportFailure(event);
		}

		void Signal(const event::EqualityFail& event)
		{
			ReportFailure(event);

			const std::string relation = event.equal ? "==" : "!=";

			ReportFailureSummary(relation, event.a, event.b);
		}

		void Signal(const event::ComparisonFail& event)
		{
			ReportFailure(event);

			const std::string relation = [](const std::strong_ordering ordering) -> std::string
			{
				if (ordering == std::strong_ordering::less)
				{
					return "<";
				}
				if (ordering == std::strong_ordering::greater)
				{
					return ">";
				}
				return "==";
			}(event.ordering);

			ReportFailureSummary(relation, event.a, event.b);
		}

	private:
		/**
		 * @brief Provides consistent text styling for assertion failure summaries.
		 * @param relation The string representing the relationship between the two failed comparisons.
		 * @param a Comparison value a.
		 * @param b Comparison value b.
		 */
		void ReportFailureSummary(std::string_view relation, std::string_view a, std::string_view b) const
		{
			//const auto& logger = logger_.get();
			//const std::string indent(4, ' ');

			//auto makeView = [](auto range)
			//{
			//	return std::string_view(range);
			//};

			//// First line isn't indented
			//const std::string indentedA = a | std::views::split('\n') | std::views::transform(makeView) |
			//							  std::views::join_with("\n" + indent) | std::ranges::to<std::string>();

			//// First line isn't indented
			//const std::string indentedB = b | std::views::split('\n') | std::views::transform(makeView) |
			//							  std::views::join_with("\n" + indent) | std::ranges::to<std::string>();

			//logger.Info(
			//	"{}\n{}\n{}",
			//	format(HIGHLIGHT, "{}'{}'", indent, indentedA),
			//	relation,
			//	format(HIGHLIGHT, "{}'{}'", indent, indentedB));
		}

		/**
		 * @brief Reports the failure of an assertion.
		 * @param event The event containing the assertion failure.
		 * @param logger The logger to report the failure to.
		 */
		void ReportFailure(const event::AssertionFail& event)
		{
			//const auto& logger = logger_.get();
			//switch (testOutcome_)
			//{
			//	case ExpectedOutcome::PASS :
			//	{
			//		std::string styledResult = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");
			//		logger.Info(
			//			"Test {}: File({}), Line({})",
			//			styledResult,
			//			event.location.file_name(),
			//			event.location.line());

			//		//++assertionFailedCount_;

			//		break;
			//	}
			//	case ExpectedOutcome::FAIL :
			//	{
			//		std::string styledResult = format(log::TextStyle(log::Colour24(0, 255, 0)), "Passed");
			//		logger.Info(
			//			"Test {}: File({}), Line({})",
			//			styledResult,
			//			event.location.file_name(),
			//			event.location.line());

			//		//++expectedFailures_;

			//		break;
			//	}
			//	case ExpectedOutcome::SKIP :
			//	{
			//		std::string styledResult = format(log::TextStyle(log::Colour24(128, 128, 128)), "Skipped");
			//		logger.Info(
			//			"Test {}: File({}), Line({})",
			//			styledResult,
			//			event.location.file_name(),
			//			event.location.line());

			//		//++skippedCount_;

			//		break;
			//	}
			//}
		}

		ExpectedOutcome testOutcome_;
		std::reference_wrapper<const log::Logger> logger_;
	};

	// class SuiteState
	//{
	// public:
	//	SuiteState(const event::SuiteBegin& event, const log::Logger& logger) :
	//		logger_(logger)
	//	{
	//	}

	//	void Signal(const event::TestBegin& event)
	//	{
	//		currentTestState_.emplace(event.outcome, logger_);
	//	}

	//	void Signal(const event::TestEnd& event)
	//	{
	//		currentTestState_.reset();
	//	}

	//	void Signal(const event::AssertionPass& event)
	//	{
	//		currentTestState_->Signal(event);
	//	}

	//	void Signal(const event::AssertionFail& event)
	//	{
	//		currentTestState_->Signal(event);
	//	}

	//	void Signal(const event::EqualityFail& event)
	//	{
	//		currentTestState_->Signal(event);
	//	}

	//	void Signal(const event::ComparisonFail& event)
	//	{
	//		currentTestState_->Signal(event);
	//	}

	// private:
	//	std::optional<TestState> currentTestState_;

	//	std::reference_wrapper<const log::Logger> logger_;
	//};

	export class StandardReporter final : public StreamingAdapter
	{
	public:
		explicit StandardReporter(const log::Logger& logger) :
			StreamingAdapter(logger)
		{
		}

		~StandardReporter() override = default;

		static consteval std::string_view Name()
		{
			return "default";
		}

		/*void Signal(const event::SuiteBegin& event) override
		{
			currentSuiteState_.emplace(event, Logger());
		}

		void Signal(const event::SuiteEnd& event) override
		{
			currentSuiteState_.reset();
		}

		void Signal(const event::TestBegin& event) override
		{
			currentSuiteState_->Signal(event);
		}

		void Signal(const event::TestEnd& event) override
		{
			currentSuiteState_->Signal(event);
		}

		void Signal(const event::AssertionPass& event) override
		{
			currentSuiteState_->Signal(event);
		}

		void Signal(const event::AssertionFail& event) override
		{
			currentSuiteState_->Signal(event);
		}

		void Signal(const event::EqualityFail& event) override
		{
			currentSuiteState_->Signal(event);
		}

		void Signal(const event::ComparisonFail& event) override
		{
			currentSuiteState_->Signal(event);
		}

		void Signal(const event::Summary& event) override
		{
			std::string passedStyled = format(SUCCESS, "Passed");
			std::string failedStyled = format(FAILURE, "Failed");

			Logger().Info("{} Assertions {}", assertionsPassedCount_, passedStyled);

			if (assertionFailedCount_)
			{
				Logger().Info("{} Assertions {}", assertionFailedCount_, failedStyled);
			}
		}*/

	private:
		std::size_t assertionsPassedCount_ = 0;
		std::size_t assertionFailedCount_  = 0;

		// std::optional<SuiteState> currentSuiteState_;
	};

}
