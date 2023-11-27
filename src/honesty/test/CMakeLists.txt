target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"suite.ixx"
			"test.ixx"
			"entry.ixx"
			"runner.ixx"
			"expression.ixx"
			"literals.ixx"
			"reporter.ixx"
			"logger.ixx"
			"generator.ixx"
			"implementation.ixx"
			"test.cpp"
			"set.ixx"

	PRIVATE
		FILE_SET implementations
		FILES
			"runner.cpp"
			"suite.cpp"
			"set.cpp"
			"implementation.cpp"

)

target_sources(honesty_main
	PUBLIC
		FILE_SET interfaces
		FILES
			"main.cpp"
)

add_subdirectory(
	expression
)

add_subdirectory(
	reporter
)

add_subdirectory(
	logger
)