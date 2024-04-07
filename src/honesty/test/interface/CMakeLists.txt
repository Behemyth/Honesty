target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	interface.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	interface.cpp
)