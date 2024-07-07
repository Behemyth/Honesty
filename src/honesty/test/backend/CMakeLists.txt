# Provide the library definition
add_library(honesty_test_backend STATIC)
add_library(synodic::honesty::test::backend ALIAS honesty_test_backend)

set_target_properties(honesty_test_backend
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
		FOLDER "honesty"
)


# Define file sets for the test library
target_sources(honesty_test_backend
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
)

target_sources(honesty_test_backend
	PRIVATE
	FILE_SET implementations
	TYPE CXX_MODULES
)

target_sources(honesty_test_backend
	PUBLIC
	FILE_SET interfaces
	FILES
	backend.ixx
)
