target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"benchmark.ixx"
			"profile.ixx"
			"test.ixx"
			"log.ixx"
)

add_subdirectory(
	benchmark
)

add_subdirectory(
	platform
)

add_subdirectory(
	profile
)

add_subdirectory(
	log
)

add_subdirectory(
	test
)
