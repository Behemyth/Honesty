export module synodic.honesty.test.commands:reporter.default;

import synodic.honesty.log;
import synodic.honesty.test;
import std;

namespace synodic::honesty::test
{
	/**
	 * @brief Provides consistent text styling for assertion failures.
	 * @param relation The string representing the relationship between the two failed comparisons.
	 * @param a Comparison value a.
	 * @param b Comparison value b.
	 */
	std::string StyleAssertionReason(std::string_view relation, std::string_view a, std::string_view b)
	{
		constexpr log::TextStyle highlight(log::Colour24(255, 255, 0));
		const std::string indent(4, ' ');

		auto makeView = [](auto range)
		{
			return std::string_view(range);
		};

		// First line isn't indented
		const std::string indentedA = a | std::views::split('\n') | std::views::transform(makeView) |
									  std::views::join_with("\n" + indent) | std::ranges::to<std::string>();

		// First line isn't indented
		const std::string indentedB = b | std::views::split('\n') | std::views::transform(makeView) |
									  std::views::join_with("\n" + indent) | std::ranges::to<std::string>();

		return std::format(
			"{}\n{}\n{}",
			format(highlight, "{}'{}'", indent, indentedA),
			relation,
			format(highlight, "{}'{}'", indent, indentedB));
	}

	export class DefaultReporter final : public StreamingAdapter
	{
	public:
		explicit DefaultReporter(const log::Logger& logger) :
			StreamingAdapter(logger)
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
		}

		void Signal(const event::EqualityFail& event) override
		{
			++failCount_;

			const log::Logger& logger = Logger();

			std::string styledResult = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");
			logger.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());

			const std::string relation = event.equal ? "==" : "!=";

			logger.Info("{}", StyleAssertionReason(relation, event.a, event.b));
		}

		void Signal(const event::ComparisonFail& event) override
		{
			++failCount_;

			const log::Logger& logger = Logger();

			std::string styledResult = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");
			logger.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());

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

			logger.Info("{}", StyleAssertionReason(relation, event.a, event.b));
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
