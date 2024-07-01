target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	assert.ixx
	fixture.ixx
	test.ixx
	types.ixx
	reporter.ixx
	runner.ixx
	function_ref.ixx
	inplace_vector.ixx
	generator.ixx
	command.ixx
	api.ixx
	suite.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	context.cpp
)

target_sources(honesty_main
PRIVATE
	entry.cpp
)

add_subdirectory(
	api
)

add_subdirectory(
	command
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)
