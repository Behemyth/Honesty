target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	assert.ixx
	test.ixx
	types.ixx
	reporter.ixx
	runner.ixx
	function_ref.ixx
	generator.ixx
	instance.ixx
	interface.ixx
	suite.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	context.cpp
)

target_sources(honesty_main
	PUBLIC
	entry.cpp
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)
