target_sources(honesty_test
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
	inplace_vector.ixx
	generator.ixx
	suite.ixx
	PRIVATE
	FILE_SET implementations
	TYPE CXX_MODULES
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
	backend
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
