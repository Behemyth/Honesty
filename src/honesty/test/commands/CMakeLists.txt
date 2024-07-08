
target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
	FILES
	commands.ixx
	PRIVATE
	FILE_SET implementations
	TYPE CXX_MODULES
	FILES
	execute.cpp
	list.cpp
	types.cpp
)
