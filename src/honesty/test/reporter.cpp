module synodic.honesty.test;

import :reporter.compact;
import :reporter.default;

namespace
{
	constinit synodic::honesty::test::CompactReporter COMPACT_REPORTER;
	constinit synodic::honesty::test::DefaultReporter DEFAULT_REPORTER;

	constinit std::array<synodic::honesty::test::Reporter*, 2> REPORTERS = {&COMPACT_REPORTER, &DEFAULT_REPORTER};
}

namespace synodic::honesty::test
{
	std::string_view Reporter::Name() const
	{
		return name_;
	}

	bool RegisterReporter(Reporter& reporter)
	{
		GetRegistry().AddReporter(reporter);
		return true;
	}
}

std::span<synodic::honesty::test::Reporter*> GetBuiltinReporters()
{
	return REPORTERS;
}

const synodic::honesty::test::Reporter& GetDefaultReporter()
{
	return DEFAULT_REPORTER;
}
