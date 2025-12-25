import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::metric;

namespace
{
	// Baseline implementation using std::vector<std::string>
	class BaselineTag
	{
	public:
		BaselineTag() = default;

		explicit BaselineTag(std::string_view tag) :
			tags_({std::string(tag)})
		{
		}

		BaselineTag operator/(const BaselineTag& other) const
		{
			BaselineTag result;
			result.tags_ = tags_;
			result.tags_.insert(result.tags_.end(), other.tags_.begin(), other.tags_.end());
			return result;
		}

		bool operator==(const BaselineTag& other) const
		{
			for (const auto& tag : other.tags_)
			{
				if (std::ranges::find(tags_, tag) != tags_.end())
				{
					return true;
				}
			}
			return false;
		}

		bool operator==(std::string_view tag) const
		{
			return std::ranges::find(tags_, tag) != tags_.end();
		}

		std::size_t Size() const { return tags_.size(); }

	private:
		std::vector<std::string> tags_;
	};

	Suite SUITE(
		"benchmark_tag",
		[]() -> Generator
		{
			// ============================================================
			// Tag Construction Benchmarks
			// ============================================================

			co_yield "construction"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "vector_string"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						auto tag = BaselineTag("benchmark");
						DoNotOptimize(tag);
					});
				};

				co_yield "tag"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						auto tag = Tag("benchmark");
						DoNotOptimize(tag);
					});
				};
			};

			// ============================================================
			// Tag Combination (operator/) Benchmarks
			// ============================================================

			co_yield "combination"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "vector_string"_bench = [](RegressionContext& ctx)
				{
					const BaselineTag tag1("first");
					const BaselineTag tag2("second");

					ctx.Measure([&]()
					{
						auto combined = tag1 / tag2;
						DoNotOptimize(combined);
					});
				};

				co_yield "tag"_bench = [](RegressionContext& ctx)
				{
					const Tag tag1("first");
					const Tag tag2("second");

					ctx.Measure([&]()
					{
						auto combined = tag1 / tag2;
						DoNotOptimize(combined);
					});
				};
			};

			co_yield "multi_combination"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "vector_string"_bench = [](RegressionContext& ctx)
				{
					const BaselineTag tag1("first");
					const BaselineTag tag2("second");
					const BaselineTag tag3("third");
					const BaselineTag tag4("fourth");

					ctx.Measure([&]()
					{
						auto combined = tag1 / tag2 / tag3 / tag4;
						DoNotOptimize(combined);
					});
				};

				co_yield "tag"_bench = [](RegressionContext& ctx)
				{
					const Tag tag1("first");
					const Tag tag2("second");
					const Tag tag3("third");
					const Tag tag4("fourth");

					ctx.Measure([&]()
					{
						auto combined = tag1 / tag2 / tag3 / tag4;
						DoNotOptimize(combined);
					});
				};
			};

			// ============================================================
			// Tag Equality Benchmarks
			// ============================================================

			co_yield "equality_string_view"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "vector_string"_bench = [](RegressionContext& ctx)
				{
					const BaselineTag tag = BaselineTag("skip") / BaselineTag("test") / BaselineTag("benchmark");

					ctx.Measure([&]()
					{
						bool found = (tag == "test");
						DoNotOptimize(found);
					});
				};

				co_yield "tag"_bench = [](RegressionContext& ctx)
				{
					const Tag tag = Tag("skip") / Tag("test") / Tag("benchmark");

					ctx.Measure([&]()
					{
						bool found = (tag == "test");
						DoNotOptimize(found);
					});
				};
			};

			co_yield "equality_tag"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "vector_string"_bench = [](RegressionContext& ctx)
				{
					const BaselineTag tag1 = BaselineTag("skip") / BaselineTag("test") / BaselineTag("benchmark");
					const BaselineTag tag2("test");

					ctx.Measure([&]()
					{
						bool found = (tag1 == tag2);
						DoNotOptimize(found);
					});
				};

				co_yield "tag"_bench = [](RegressionContext& ctx)
				{
					const Tag tag1 = Tag("skip") / Tag("test") / Tag("benchmark");
					const Tag tag2("test");

					ctx.Measure([&]()
					{
						bool found = (tag1 == tag2);
						DoNotOptimize(found);
					});
				};
			};

			// ============================================================
			// Tag Miss (Not Found) Benchmarks - Important for hot path
			// ============================================================

			co_yield "equality_miss"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "vector_string"_bench = [](RegressionContext& ctx)
				{
					const BaselineTag tag = BaselineTag("skip") / BaselineTag("test") / BaselineTag("benchmark");

					ctx.Measure([&]()
					{
						bool found = (tag == "nonexistent");
						DoNotOptimize(found);
					});
				};

				co_yield "tag"_bench = [](RegressionContext& ctx)
				{
					const Tag tag = Tag("skip") / Tag("test") / Tag("benchmark");

					ctx.Measure([&]()
					{
						bool found = (tag == "nonexistent");
						DoNotOptimize(found);
					});
				};
			};
		});

	SuiteRegistrar _(SUITE);
}
