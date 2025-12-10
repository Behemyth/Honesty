# Testing API

## Tags

Tags modify how tests are executed and reported. They can be combined with tests using the `/` operator.

### Predefined Tags

| Tag | Description |
|-----|-------------|
| `RUN` | Default test state (explicit) |
| `SKIP` | Skips test execution, records as skipped |
| `FAIL` | Expects test to fail, inverts pass/fail reporting |
| `TODO` | Marks test as incomplete, outcome is undefined |
| `BASELINE` | Marks a benchmark as the baseline for comparison within a group |
| `SILENT` | Runs test silently without reporter output or counting in totals |

### SILENT Tag

The `SILENT` tag is designed for **meta-tests** - tests that verify the test framework itself. When a test is tagged with `SILENT`:

- The test **executes normally** and can fail the overall test run
- **No events are signaled** to reporters (no output, no counting in totals)
- **All child tests** inherit the silent mode (propagates through nested tests)
- **Assertions still work** but don't report to external reporters

```cpp
#include <honesty/test.hpp>

using namespace honesty::test;
using namespace honesty::test::literals;

Suite SUITE("meta_tests", []() -> Generator
{
    // This test runs but produces no output
    co_yield SILENT / "verify_assertion_works"_test = [](const Requirements& requirements)
    {
        requirements.Assert(true);  // Passes silently
    };

    // SILENT propagates to all children
    co_yield SILENT / "nested_silent"_test = []() -> Generator
    {
        co_yield "child1"_test = [](const Requirements& requirements)
        {
            requirements.Expect(true);  // Also silent
        };

        co_yield "child2"_test = [](const Requirements& requirements)
        {
            requirements.Expect(true);  // Also silent
        };
    };
});
```

### When to Use SILENT

Use `SILENT` when:
- Testing the test framework's internal behavior
- Running benchmarks that measure framework overhead
- Integration tests that shouldn't pollute the main test output
