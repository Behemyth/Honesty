target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			runner.ixx
			local.ixx
	PRIVATE
		FILE_SET implementations
		FILES
			local.cpp
			runner.cpp
)
