# Provide the benchmark library
add_library(benchmark STATIC)
add_library(synodic::benchmark ALIAS benchmark)

# benchmark module definition
target_sources(benchmark
	PUBLIC # For building the library and dependents
		FILE_SET benchmark_public
		TYPE CXX_MODULES
		FILES
			benchmark.ixx
			context.ixx
	PRIVATE # For building the target
		FILE_SET benchmark_private
		TYPE CXX_MODULES
)

target_link_libraries(benchmark
	PUBLIC
		properties_public
	PRIVATE
		properties_private
)
