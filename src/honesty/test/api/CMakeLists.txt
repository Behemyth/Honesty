# Provide the library definition
add_library(honesty_test_api STATIC)
add_library(synodic::honesty_test_api ALIAS honesty_test)

set_target_properties(honesty_test_api
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
		FOLDER "honesty"
)

# Define file sets for the test library
target_sources(honesty_test_api
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
)

target_sources(honesty_test_api
	PRIVATE
	FILE_SET implementations
	TYPE CXX_MODULES
)


target_sources(honesty_test_api
	PUBLIC
	FILE_SET interfaces
	FILES
	execute.ixx
	list.ixx
	types.ixx
)
