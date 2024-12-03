cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

if(NOT SKBUILD)
	message(FATAL_ERROR
		"Please use 'skbuild' to build this project."
	)
endif()

project(honesty
	VERSION ${SKBUILD_PROJECT_VERSION}
	DESCRIPTION "Honesty"
	LANGUAGES CXX
)

# Register our custom module location with CMake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/tool/cmake/modules")

option(BUILD_TESTING "Build the tests verifying Honesty functionality" ON)

# Provide the main library
add_library(honesty STATIC)
add_library(synodic::honesty ALIAS honesty)

# Provide a convenience variant with a main function
add_library(honesty_main STATIC)
add_library(synodic::honesty::main ALIAS honesty_main)

# Search for python and enable the modules required for the build backend
# NOTE: CMake will only find our virtual interpreter if we are in its virtual environment
find_package(Python 3.13 REQUIRED COMPONENTS Interpreter Development.Module)

find_package(nanobind CONFIG REQUIRED)

find_package(Pytest REQUIRED)

# Provide the Python bindings module
nanobind_add_module(honesty_bindings src/bindings/bindings.cpp)

# Link our library to the bindings
target_link_libraries(honesty_bindings
	PRIVATE
	synodic::honesty
)

set_target_properties(honesty honesty_main
	PROPERTIES
	LINKER_LANGUAGE CXX
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS NO
	FOLDER "honesty"
)

# Link the main library to the convenience variant
target_link_libraries(honesty_main
	PUBLIC
	synodic::honesty
)

# Gather and define all the project and sub-project sources
add_subdirectory(
	src
)

# NOTE: For IDE setups that do not use CMake directly, we setup all IDE directories to mimic
# their system directory structure. See https://gitlab.kitware.com/cmake/cmake/-/issues/25992
file(GLOB_RECURSE file_list ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

# Must be called from the target definition directory. Only matched against 'target_sources'
# files (No nested CMakeList.txt, docs, etc).
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${file_list})

if(BUILD_TESTING)

	# Do not include CTest because it adds targets and integrations we do not want to support
	enable_testing()

	add_subdirectory(
		tests
	)

endif()
