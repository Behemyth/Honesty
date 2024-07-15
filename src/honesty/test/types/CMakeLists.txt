
target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
	FILES
	assert.ixx
	exception.ixx
	test.ixx
	types.ixx
	reporter.ixx
	runner.ixx
	suite.ixx
	fixture.ixx)
