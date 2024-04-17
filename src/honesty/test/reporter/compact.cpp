module synodic.honesty.test;

namespace synodic::honesty::test
{
	CompactReporter::CompactReporter(std::string_view name) :
		StreamingAdapter(name),
		logger_("reporter")
	{
	}
}
