cmake_minimum_required(VERSION 3.29 FATAL_ERROR)

project(honesty
	VERSION 0.1.0
	DESCRIPTION "Honesty"
	LANGUAGES CXX
)

option(BUILD_TESTING "Build the tests verifying Honesty functionality" ON)

# Register our custom module location with CMake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/tool/cmake/modules")

# Provide a helper interface library that all other libraries should link against
add_library(properties_public INTERFACE)
add_library(properties_private INTERFACE)

set_target_properties(properties_public
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 26
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
		FOLDER "honesty"
)

set_target_properties(properties_private
	PROPERTIES
)

# Provide the main library
add_library(honesty STATIC)
add_library(synodic::honesty ALIAS honesty)

# Provide a convenience variant with a main function
add_library(honesty_entry STATIC)
add_library(synodic::honesty::entry ALIAS honesty_entry)

target_link_libraries(honesty
	PUBLIC
		properties_public
	PRIVATE
		properties_private
)

# Link the main library to the convenience variant
target_link_libraries(honesty_entry
	PUBLIC
		synodic::honesty
		properties_public
	PRIVATE
		properties_private
)

# Gather and define all the project and subproject sources
add_subdirectory(
	src
)

# NOTE: For IDE setups that do not use CMake directly, we setup all IDE directories to mimic
#	their system directory structure. See https://gitlab.kitware.com/cmake/cmake/-/issues/25992

file(GLOB_RECURSE file_list ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

# Must be called from the target definition directory. Only matched against 'target_sources'
#	files (No nested CMakeList.txt, docs, etc).
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${file_list})

if(BUILD_TESTING)
	# Do not include CTest because it adds targets and integrations we do not want to support
	enable_testing()

	add_subdirectory(
		tests
	)
endif()
