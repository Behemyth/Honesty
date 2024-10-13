
target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
	FILES
	requirements.ixx
	test.ixx
	types.ixx
	reporter.ixx
	runner.ixx
	suite.ixx
	fixture.ixx
	tag.ixx
	suite_context.ixx
	test_context.ixx
)
