export module synodic.honesty.test.commands:reporter.standard;

import synodic.honesty.log;
import synodic.honesty.test;
import synodic.honesty.utility;
import std;

// TODO: Make constexpr when
//	'https://developercommunity.visualstudio.com/t/Inexplicable-ICE-when-using-modules/1593396' is fixed
// static const inline auto
//	HIGHLIGHT_STYLE(synodic::honesty::log::TextStyle(synodic::honesty::log::Colour24(255, 255, 0)));

namespace synodic::honesty::test
{

	/**
	 * @brief The state required for the lifetime of a test
	 */
	class TestState
	{
	public:
		struct Output
		{
			std::size_t passedAssertions		  = 0;
			std::size_t failedAssertions		  = 0;
			std::size_t undefinedPassedAssertions = 0;
			std::size_t undefinedFailedAssertions = 0;
		};

		TestState(const event::TestBegin& event, const log::Logger& logger) :
			testOutcome_(event.outcome),
			logger_(logger)
		{
		}

		void Signal(const event::AssertionPass& event)
		{
			ReportSuccess(event);
		}

		void Signal(const event::AssertionFail& event)
		{
			ReportFailure(event);
		}

		void Signal(const event::EqualityFail& event)
		{
			ReportFailure(event);

			const std::string_view relation = event.equal ? "==" : "!=";

			ReportFailureSummary(relation, event.a, event.b);
		}

		void Signal(const event::ComparisonFail& event)
		{
			ReportFailure(event);

			const std::string_view relation = [](const std::strong_ordering ordering) -> std::string_view
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

		Output Summarize(const event::TestEnd& event)
		{
			return output_;
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
			constexpr std::size_t maxLineLength = 79;

			const log::Logger& logger = logger_.get();
			constexpr std::string_view indent("	");

			// Size of the inputs including the space between them.
			const std::size_t totalSize = a.size() + b.size() + 2 + relation.size();

			// Check to see if the compacted summary can fit on a single line
			if (a.contains("\n") or b.contains("\n") or totalSize >= maxLineLength)
			{
				throw utility::NotImplemented();
			}
			else
			{
				logger.Info("{}Expected:", indent);

				const auto HIGHLIGHT_STYLE(
					log::TextStyle(log::Colour24(255, 255, 0)));

				logger.Info(
					"Expected: {}\n{}\n{}",
					format(HIGHLIGHT_STYLE, "{}'{}'", indent, a),
					relation,
					format(HIGHLIGHT_STYLE, "{}'{}'", indent, b));
			}
		}

		/**
		 * @brief Reports the success of an assertion.
		 * @param event The assertion event.
		 */
		void ReportSuccess(const event::AssertionPass& event)
		{
			const log::Logger& logger = logger_.get();
			switch (testOutcome_)
			{
				case ExpectedAssertOutcome::PASS :
				{
					++output_.passedAssertions;
					break;
				}
				case ExpectedAssertOutcome::FAIL :
				{
					std::string styledResult = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");
					logger.Info(
						"Test {}: File({}), Line({})",
						styledResult,
						event.location.file_name(),
						event.location.line());

					++output_.failedAssertions;
					break;
				}
				case ExpectedAssertOutcome::TODO :
				{
					++output_.undefinedPassedAssertions;
					break;
				}
			}
		}

		/**
		 * @brief Reports the failure of an assertion.
		 * @param event The assertion event.
		 */
		void ReportFailure(const event::AssertionFail& event)
		{
			const log::Logger& logger = logger_.get();
			switch (testOutcome_)
			{
				case ExpectedAssertOutcome::PASS :
				{
					std::string styledResult = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");
					logger.Info(
						"Test {}: File({}), Line({})",
						styledResult,
						event.location.file_name(),
						event.location.line());

					++output_.failedAssertions;

					break;
				}
				case ExpectedAssertOutcome::FAIL :
				{
					++output_.passedAssertions;

					break;
				}
				case ExpectedAssertOutcome::TODO :
				{
					++output_.undefinedFailedAssertions;
					break;
				}
			}
		}

		ExpectedAssertOutcome testOutcome_;
		std::reference_wrapper<const log::Logger> logger_;

		Output output_;
	};

	class SuiteState
	{
	public:
		struct Output
		{
			std::size_t passedAssertions		  = 0;
			std::size_t failedAssertions		  = 0;
			std::size_t undefinedPassedAssertions = 0;
			std::size_t undefinedFailedAssertions = 0;
		};

		SuiteState(const event::SuiteBegin& event, const log::Logger& logger) :
			logger_(logger)
		{
		}

		void Signal(const event::TestBegin& event)
		{
			TestState& state = testStates_.emplace_back(event, logger_);
		}

		void Signal(const event::TestEnd& event)
		{
			TestState& state = testStates_.back();
			auto output		 = state.Summarize(event);

			testStates_.pop_back();
		}

		void Signal(const event::AssertionPass& event)
		{
			TestState& state = testStates_.back();
			state.Signal(event);
		}

		void Signal(const event::AssertionFail& event)
		{
			TestState& state = testStates_.back();
			state.Signal(event);
		}

		void Signal(const event::EqualityFail& event)
		{
			TestState& state = testStates_.back();
			state.Signal(event);
		}

		void Signal(const event::ComparisonFail& event)
		{
			TestState& state = testStates_.back();
			state.Signal(event);
		}

		Output Summarize(const event::TestEnd& event)
		{
			return output_;
		}

	private:
		// Tests are recursive, and we should keep track of state as such.
		std::list<TestState> testStates_;

		std::reference_wrapper<const log::Logger> logger_;

		Output output_;
	};

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

		void Signal(const event::SuiteBegin& event) override
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
			const log::Logger& logger = Logger();

			const auto SUCCESS_STYLE(log::TextStyle(log::Colour24(0, 255, 0)));
			const auto FAILURE_STYLE(log::TextStyle(log::Colour24(255, 0, 0)));

			std::string passedStyled = format(SUCCESS_STYLE, "Passed");
			std::string failedStyled = format(FAILURE_STYLE, "Failed");

			logger.Info("{} Assertions {}", assertionsPassedCount_, passedStyled);

			if (assertionFailedCount_)
			{
				logger.Info("{} Assertions {}", assertionFailedCount_, failedStyled);
			}
		}

	private:
		std::size_t assertionsPassedCount_ = 0;
		std::size_t assertionFailedCount_  = 0;

		std::optional<SuiteState> currentSuiteState_;
	};

}
