module synodic.honesty.test;

namespace synodic::honesty::test
{
	Reporter::Reporter(std::string_view name) :
		name_(name)
	{
	}

	std::string_view Reporter::Name() const
	{
		return name_;
	}

	StreamingAdapter::StreamingAdapter(std::string_view name) :
		Reporter(name)
	{
	}
}
