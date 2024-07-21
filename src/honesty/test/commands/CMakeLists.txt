
target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
	FILES
	commands.ixx
	list.ixx
	PRIVATE
	FILE_SET implementations
	TYPE CXX_MODULES
	FILES
	execute.cpp
	types.cpp
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)
