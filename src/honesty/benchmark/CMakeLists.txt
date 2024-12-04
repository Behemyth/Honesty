target_sources(honesty
	PUBLIC
		FILE_SET benchmark_public
		TYPE CXX_MODULES
		FILES
			benchmark.ixx
			context.ixx
			types.ixx
			timer.ixx
	PRIVATE
		FILE_SET benchmark_private
		TYPE CXX_MODULES
)
