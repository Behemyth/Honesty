target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	default.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	default.cpp
)
