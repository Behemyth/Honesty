
target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
	FILES
	assert.ixx
	types.ixx
	reporter.ixx
	runner.ixx
	fixture.ixx)
