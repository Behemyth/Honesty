target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"benchmark.ixx"
			"platform.ixx"
			"profile.ixx"
			"terminal.ixx"
			"test.ixx"
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
	terminal
)

add_subdirectory(
	test
)