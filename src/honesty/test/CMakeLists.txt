target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
	FILES
	assert.ixx
	fixture.ixx
	test.ixx
	types.ixx
	reporter.ixx
	runner.ixx
	function_ref.ixx
	frontend.ixx
	inplace_vector.ixx
	generator.ixx
	suite.ixx
)

add_subdirectory(
	api
)

add_subdirectory(
	entry
)

add_subdirectory(
	context
)

add_subdirectory(
	commands
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)
