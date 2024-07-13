target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
	FILES
	test.ixx
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
	backend
)

add_subdirectory(
	commands
)

add_subdirectory(
	types
)
