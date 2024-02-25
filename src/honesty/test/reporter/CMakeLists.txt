target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			reporter.ixx
			console.ixx
	PRIVATE
		FILE_SET implementations
		FILES
			console.cpp
			reporter.cpp
)
