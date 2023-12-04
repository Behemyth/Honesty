target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			logger.ixx
	PRIVATE
		FILE_SET implementations
		FILES
			stdout.ixx
			stdout.cpp
)
