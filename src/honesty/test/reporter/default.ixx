export module synodic.honesty.test:reporter.default;

import synodic.honesty.log;
import :reporter;
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

		const auto rangeA = a | std::views::split('\n') |
							std::views::transform(
								[](auto r)
								{
									return std::string_view(r);
								}) |
							std::views::join_with("\n" + indent);

		const auto rangeB = b | std::views::split('\n') |
							std::views::transform(
								[](auto r)
								{
									return std::string_view(r);
								}) |
							std::views::join_with("\n" + indent);

		// TODO: Using the ranges for formatting requires preview 3.
		const std::string indentedA = std::format("{}", a);
		const std::string indentedB = std::format("{}", b);

		return std::format(
			"{} {} {}",
			format(highlight, "{}", indentedA),
			relation,
			format(highlight, "{}", indentedB));
	}

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
		}

		void Signal(const event::EqualityFail& event) override
		{
			++failCount_;

			const log::Logger& logger = Logger();

			std::string styledResult = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");
			logger.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());
		}

		void Signal(const event::ComparisonFail& event) override
		{
			++failCount_;

			const log::Logger& logger = Logger();

			std::string styledResult = format(log::TextStyle(log::Colour24(255, 0, 0)), "Failed");
			logger.Info("Test {}: File({}), Line({})", styledResult, event.location.file_name(), event.location.line());
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
