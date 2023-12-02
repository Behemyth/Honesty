target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			single.ixx
	PRIVATE
		FILE_SET implementations
		FILES
			single.cpp
)
