target_sources(tests
PRIVATE
	test_compile_time_config.cpp
)

add_subdirectory(
	metric
)

add_subdirectory(
	test
)

add_subdirectory(
	log
)

add_subdirectory(
	trace
)

add_subdirectory(
	utility
)
