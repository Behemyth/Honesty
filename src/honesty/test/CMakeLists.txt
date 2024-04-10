target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"test.ixx"
			"types.ixx"
			"logger.ixx"
			"reporter.ixx"
			"runner.ixx"
			"function_ref.ixx"
			"generator.ixx"
			"interface.ixx"
	PRIVATE
		FILE_SET implementations
		FILES
			"registry.cpp"
			"test.cpp"
			"runner.cpp"
			"interface.cpp"
)

target_sources(honesty_main
	PUBLIC
		"entry.cpp"
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
