target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			console.ixx
	PRIVATE
		FILE_SET implementations
		FILES
			console.cpp
)
