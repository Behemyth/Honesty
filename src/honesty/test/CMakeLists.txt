# Provide the library definition
add_library(honesty_test STATIC)
add_library(synodic::honesty_test ALIAS honesty_test)

set_target_properties(honesty_test
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
		FOLDER "honesty"
)


# Define file sets for the test library
target_sources(honesty_test
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
)

target_sources(honesty_test
	PRIVATE
	FILE_SET implementations
	TYPE CXX_MODULES
)

target_sources(honesty_test
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
	commands.ixx
	suite.ixx
	api.ixx
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
	commands
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)

# Link the library to library
target_link_libraries(honesty_test
PUBLIC
	synodic::honesty_test_api
)
