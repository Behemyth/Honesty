cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(honesty
	VERSION 0.1.0
	DESCRIPTION "Honesty"
	LANGUAGES CXX
)

option(BUILD_TESTING "Build the tests verifying Honesty functionality" ON)

# Register our custom module location with CMake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/tool/cmake/modules")

# TODO: Make this variables configurable only in CMakePresets.json
# Make sure we only search for the virtual environment if we are in one
set(Python3_FIND_VIRTUALENV ONLY)
set(Python3_FIND_STRATEGY LOCATION)

# Search for python and enable the modules required by 'nanobind'.
# NOTE: CMake will only find our virtual interpreter if we are in its virtual environment
find_package(Python 3.13 REQUIRED COMPONENTS Interpreter Development.Module)

# Register our venv location with CMake
list(APPEND CMAKE_PREFIX_PATH "$ENV{VIRTUAL_ENV}")

# Detect the installed nanobind package and import it into CMake
execute_process(
	COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
	OUTPUT_STRIP_TRAILING_WHITESPACE
	OUTPUT_VARIABLE nanobind_ROOT
	COMMAND_ERROR_IS_FATAL ANY
)

find_package(nanobind CONFIG REQUIRED)

find_package(Pytest REQUIRED)

# Provide the main library
add_library(honesty STATIC)
add_library(synodic::honesty ALIAS honesty)

# Provide a convenience variant with a main function
add_library(honesty_main STATIC)
add_library(synodic::honesty::main ALIAS honesty_main)

# Provide the Python bindings module
nanobind_add_module(honesty_bindings src/bindings/bindings.cpp)

set_target_properties(honesty honesty_main
	PROPERTIES
	LINKER_LANGUAGE CXX
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS NO
	FOLDER "honesty"
)

# Link our library to the bindings
target_link_libraries(honesty_bindings
	PRIVATE
	synodic::honesty
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
