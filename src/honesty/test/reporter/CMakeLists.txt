target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			reporter.ixx
	PRIVATE
		FILE_SET implementations
		FILES
			console.ixx
			console.cpp
)
