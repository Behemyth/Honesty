cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(honesty
	VERSION 0.1.0
	DESCRIPTION "Honesty"
	LANGUAGES CXX
)

option(BUILD_TESTING "Build the tests verifying Honesty functionality" ON)

# Register our custom module location with CMake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/tool/cmake/modules")

# Find our dependencies for tools and functionality

# Python
set(Python_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.venv/Scripts")
find_package(Python MODULE REQUIRED COMPONENTS Interpreter Development.Module REQUIRED)

# nanobind
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

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

# Link the library to the convenience variant
target_link_libraries(honesty_main
PUBLIC
	synodic::honesty
)

# Gather all the project files
add_subdirectory(
	src
)

# Glob all files to add a source group tree. Includes all files
file(GLOB_RECURSE file_list ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

# Must be called from the target definition directory. Only matched against target source files (No nested CMakeList.txt, docs, etc).
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${file_list})

if(BUILD_TESTING)

	# Do not include CTest because it adds targets and integrations we do not want to support
	enable_testing()

    add_subdirectory(
		tests
	)

endif()