# Provide the library definition
add_library(honesty STATIC)
add_library(synodic::honesty ALIAS honesty)

# Provide a convenience variant with a main function
add_library(honesty_main STATIC)
add_library(synodic::honesty::main ALIAS honesty_main)

set_target_properties(honesty honesty_main
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
		FOLDER "honesty"
)

target_link_libraries(honesty
PUBLIC
	synodic::honesty::test
)

# Link the library to the convenience variant
target_link_libraries(honesty_main
PUBLIC
	synodic::honesty
)

add_subdirectory(
	honesty
)
