target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		TYPE CXX_MODULES
		FILES
			"backend.ixx"
			"test.ixx"
			"types.ixx"
			"logger.ixx"
			"reporter.ixx"
			"runner.ixx"
			"function_ref.ixx"
			"counter.ixx"
			"generator.ixx"
	PRIVATE
		"backend.cpp"
		"test.cpp"
		"reporter.cpp"
		"runner.cpp"
		"logger.cpp"
)

target_sources(honesty_main
	PUBLIC
		"interface.cpp"
)

add_subdirectory(
	interface
)

add_subdirectory(
	backend
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)

add_subdirectory(
	logger
)