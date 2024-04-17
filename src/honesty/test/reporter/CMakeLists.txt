target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	compact.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	compact.cpp
)
target_sources(honesty
	INTERFACE
	export.cpp
)
