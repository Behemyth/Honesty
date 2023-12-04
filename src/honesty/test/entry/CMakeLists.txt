target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			entry.ixx
	PRIVATE
		FILE_SET implementations
		FILES
			entry.cpp
)
