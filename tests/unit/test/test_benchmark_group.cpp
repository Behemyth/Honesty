import std;
import synodic.honesty.test;
import synodic.honesty.metric;

using namespace honesty::test;
using namespace honesty::test::literals;
using namespace honesty::metric;

namespace
{
	Suite SUITE(
		"benchmark_group",
		[]() -> Generator
		{
			// ============================================================
			// Basic group with one baseline
			// ============================================================

			co_yield SILENT / "single_baseline"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "baseline_impl"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						int sum = 0;
						for (int i = 0; i < 100; ++i)
						{
							sum += i;
						}
						DoNotOptimize(sum);
					});
				};

				co_yield "optimized_impl"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						// Simulated faster implementation
						int sum = 0;
						for (int i = 0; i < 50; ++i)
						{
							sum += i * 2;
						}
						DoNotOptimize(sum);
					});
				};

				co_yield "slower_impl"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						// Simulated slower implementation
						int sum = 0;
						for (int i = 0; i < 200; ++i)
						{
							sum += i;
						}
						DoNotOptimize(sum);
					});
				};
			};

			// ============================================================
			// Group with no baseline - should show absolute values only
			// ============================================================

			co_yield SILENT / "no_baseline"_bench = []() -> BenchmarkGenerator
			{
				co_yield "impl_a"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						int x = 42;
						DoNotOptimize(x);
					});
				};

				co_yield "impl_b"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						int x = 42;
						int y = x * 2;
						DoNotOptimize(y);
					});
				};
			};

			// ============================================================
			// Group with multiple baselines - should use first, warn about extras
			// ============================================================

			co_yield SILENT / "multiple_baselines"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "first_baseline"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						int sum = 0;
						for (int i = 0; i < 100; ++i)
						{
							sum += i;
						}
						DoNotOptimize(sum);
					});
				};

				co_yield BASELINE / "second_baseline"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						int sum = 0;
						for (int i = 0; i < 80; ++i)
						{
							sum += i;
						}
						DoNotOptimize(sum);
					});
				};

				co_yield "regular_impl"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						int sum = 0;
						for (int i = 0; i < 120; ++i)
						{
							sum += i;
						}
						DoNotOptimize(sum);
					});
				};
			};

			// ============================================================
			// Nested groups - each level reports independently
			// ============================================================

			co_yield SILENT / "nested_groups"_bench = []() -> BenchmarkGenerator
			{
				co_yield BASELINE / "outer_baseline"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						int x = 1;
						DoNotOptimize(x);
					});
				};

				co_yield "outer_impl"_bench = [](RegressionContext& ctx)
				{
					ctx.Measure([]()
					{
						int x = 2;
						DoNotOptimize(x);
					});
				};

				// Nested group within the outer group
				co_yield "inner_group"_bench = []() -> BenchmarkGenerator
				{
					co_yield BASELINE / "inner_baseline"_bench = [](RegressionContext& ctx)
					{
						ctx.Measure([]()
						{
							int sum = 0;
							for (int i = 0; i < 10; ++i)
							{
								sum += i;
							}
							DoNotOptimize(sum);
						});
					};

					co_yield "inner_impl"_bench = [](RegressionContext& ctx)
					{
						ctx.Measure([]()
						{
							int sum = 0;
							for (int i = 0; i < 20; ++i)
							{
								sum += i;
							}
							DoNotOptimize(sum);
						});
					};
				};
			};

			// ============================================================
			// Empty group - should skip or warn
			// ============================================================

			co_yield SILENT / "empty_group"_bench = []() -> BenchmarkGenerator
			{
				// No children yielded
				co_return;
			};

			// ============================================================
			// Ungrouped benchmark - standalone, not nested in a group
			// ============================================================

			co_yield SILENT / "ungrouped_standalone"_bench = [](RegressionContext& ctx)
			{
				ctx.Measure([]() {
					int sum = 0;
					for (int i = 0; i < 50; ++i)
					{
						sum += i;
					}
					DoNotOptimize(sum);
				});
			};
		});

	SuiteRegistrar _(SUITE);
}
