export module synodic.honesty.test:reporter;

import synodic.honesty.log;

import std;
import function_ref;
import :types;
import inplace_vector;

namespace synodic::honesty::test
{
	export class Reporter
	{
	public:
		/**
		 * @brief Constructs a Reporter object
		 * @param logger The logger to associate with this reporter
		 */
		explicit constexpr Reporter(log::Logger logger) :
			logger_(std::move(logger))
		{
		}

		virtual ~Reporter() = default;

		virtual void Signal(const event::SuiteBegin& event) = 0;
		virtual void Signal(const event::SuiteEnd& event)	= 0;
		virtual void Signal(const event::SuiteSkip& event)	= 0;

		virtual void Signal(const event::SuiteRun& event)  = 0;
		virtual void Signal(const event::SuiteFail& event) = 0;
		virtual void Signal(const event::SuitePass& event) = 0;

		virtual void Signal(const event::SuiteSummary& event) = 0;

		virtual void Signal(const event::TestBegin& event) = 0;
		virtual void Signal(const event::TestEnd& event)   = 0;
		virtual void Signal(const event::TestSkip& event)  = 0;

		virtual void Signal(const event::TestRun& event)  = 0;
		virtual void Signal(const event::TestFail& event) = 0;
		virtual void Signal(const event::TestPass& event) = 0;

		virtual void Signal(const event::AssertionFail& event) = 0;
		virtual void Signal(const event::EqualityFail& event) = 0;
		virtual void Signal(const event::ComparisonFail& event) = 0;
		virtual void Signal(const event::AssertionPass& event) = 0;
		virtual void Signal(const event::AssertionSkip& event) = 0;

		virtual void Signal(const event::Summary& event) = 0;

		/**
		 * @brief Get the logger associated with this reporter
		 * @return The logger given at construction, or the root logger if one wasn't specified
		 */
		const log::Logger& Logger() const
		{
			// We don't use value_or here because the const reference is not convertible to a value
			if (logger_)
			{
				return logger_.value();
			}

			return log::RootLogger();
		}

	private:
		std::string_view name_;
		std::optional<log::Logger> logger_;
	};

	/**
	 * @brief TODO: Comment
	 */
	export class StreamingAdapter : public Reporter
	{
	public:
		explicit(false) constexpr StreamingAdapter(log::Logger logger) :
			Reporter(std::move(logger))
		{
		}

		~StreamingAdapter() override = default;

		void Signal(const event::SuiteBegin& event) override
		{
		}

		void Signal(const event::SuiteEnd& event) override
		{
		}

		void Signal(const event::SuiteSkip& event) override
		{
		}

		void Signal(const event::SuiteRun& event) override
		{
		}

		void Signal(const event::SuiteFail& event) override
		{
		}

		void Signal(const event::SuitePass& event) override
		{
		}

		void Signal(const event::SuiteSummary& event) override
		{
		}

		void Signal(const event::TestBegin& event) override
		{
		}

		void Signal(const event::TestEnd& event) override
		{
		}

		void Signal(const event::TestSkip& event) override
		{
		}

		void Signal(const event::TestRun& event) override
		{
		}

		void Signal(const event::TestFail& event) override
		{
		}

		void Signal(const event::TestPass& event) override
		{
		}

		void Signal(const event::AssertionFail& event) override
		{
		}

		void Signal(const event::EqualityFail& event) override
		{
			// If the equality test is not overriden, we splice the input into a generic assertion fail
			Signal(static_cast<event::AssertionFail>(event));
		}

		void Signal(const event::ComparisonFail& event) override
		{
			// If the ordering test is not overriden, we splice the input into a generic assertion fail
			Signal(static_cast<event::AssertionFail>(event));
		}

		void Signal(const event::AssertionPass& event) override
		{
		}

		void Signal(const event::AssertionSkip& event) override
		{
		}

		void Signal(const event::Summary& event) override
		{
		}
	};

	/**
	 * @brief TODO: Comment
	 */
	export class CumulativeAdapter : public Reporter
	{
	public:

		struct TestData
		{
			event::TestBegin begin;
			event::TestEnd end;
		};

		struct SuiteData
		{
			event::SuiteBegin begin;
			event::SuiteEnd end;

			std::vector<TestData> tests;
		};

		struct CumulativeData
		{
			std::vector<SuiteData> suites;
		};

		explicit(false) CumulativeAdapter(log::Logger logger) :
			Reporter(std::move(logger))
		{
		}

		~CumulativeAdapter() override = default;

		void Signal(const event::SuiteBegin& event) final
		{
			auto& [begin, end, tests] = data_.suites.emplace_back();

			begin = event;
		}

		void Signal(const event::SuiteEnd& event) final
		{
			auto& [begin, end, tests] = data_.suites.back();

			end = event;
		}

		void Signal(const event::SuiteSkip& event) final
		{
		}

		void Signal(const event::SuiteRun& event) final
		{
		}

		void Signal(const event::SuiteFail& event) final
		{
		}

		void Signal(const event::SuitePass& event) final
		{
		}

		void Signal(const event::SuiteSummary& event) final
		{
		}

		void Signal(const event::TestBegin& event) final
		{
			SuiteData& suiteData = data_.suites.back();
			TestData& testData	 = suiteData.tests.emplace_back();
			testData.begin		 = event;
		}

		void Signal(const event::TestEnd& event) final
		{
			SuiteData& suiteData = data_.suites.back();
			TestData& testData	 = suiteData.tests.back();
			testData.end		 = event;
		}

		void Signal(const event::TestSkip& event) final
		{
		}

		void Signal(const event::TestRun& event) final
		{
		}

		void Signal(const event::TestFail& event) final
		{
		}

		void Signal(const event::TestPass& event) final
		{
		}

		void Signal(const event::AssertionFail& event) final
		{
		}

		void Signal(const event::EqualityFail& event) final
		{
		}

		void Signal(const event::ComparisonFail& event) final
		{
		}

		void Signal(const event::AssertionPass& event) final
		{
		}

		void Signal(const event::AssertionSkip& event) final
		{
		}

		void Signal(const event::Summary& event) final
		{
			Finalize(std::move(data_));
		}

		virtual void Finalize(CumulativeData data) = 0;

	private:
		CumulativeData data_;
	};

	template<typename T>
	concept reporter = requires {
		T::Name();
		{
			std::bool_constant<(T::Name(), true)>()
		} -> std::same_as<std::true_type>;
		std::derived_from<Reporter, T>;
	};

	class ReporterRegistry
	{
	public:
		ReporterRegistry()
		{
			registrars_.push_back(this);
		}

		virtual ~ReporterRegistry() = default;

		virtual std::string_view Name() const						 = 0;
		virtual std::unique_ptr<Reporter> Create(log::Logger logger) const = 0;

		static void Register(ReporterRegistry* registry)
		{
			registrars_.push_back(registry);
		}

		static std::span<ReporterRegistry*> Registrars()
		{
			return registrars_;
		}

	private:
		constinit static std::inplace_vector<ReporterRegistry*, 3> registrars_;
	};

	constinit std::inplace_vector<ReporterRegistry*, 3> ReporterRegistry::registrars_;

	export template<reporter T>
	class ReporterRegistrar final : ReporterRegistry
	{
	public:
		ReporterRegistrar()
		{
		}

		std::unique_ptr<Reporter> Create(log::Logger logger) const override
		{
			return std::make_unique<T>(std::move(logger));
		}

		std::string_view Name() const override
		{
			return T::Name();
		}
	};
}
