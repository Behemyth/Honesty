target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	compact.ixx
	default.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	compact.cpp
	default.cpp
)
