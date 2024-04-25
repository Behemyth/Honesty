module synodic.honesty.test;

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
