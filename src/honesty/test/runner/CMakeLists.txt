target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			runner.ixx
	PRIVATE
		FILE_SET implementations
		FILES
			single.ixx
			single.cpp
)
