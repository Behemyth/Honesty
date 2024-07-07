# Provide the library definition
add_library(honesty_test_commands STATIC)
add_library(synodic::honesty::test::commands ALIAS honesty_test_commands)

set_target_properties(honesty_test_commands
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
		FOLDER "honesty"
)

target_link_libraries(honesty_test_commands
	PRIVATE
	synodic::honesty::test::api
)

# Define file sets for the test library
target_sources(honesty_test_commands
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
)

target_sources(honesty_test_commands
	PRIVATE
	FILE_SET implementations
	TYPE CXX_MODULES
)

target_sources(honesty_test_commands
	PUBLIC
	FILE_SET interfaces
	FILES
	commands.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	execute.cpp
	list.cpp
	types.cpp
)
