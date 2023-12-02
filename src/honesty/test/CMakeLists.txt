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
			"tag.ixx"
			"exception.ixx"

	PRIVATE
		FILE_SET implementations
		FILES
			"test.cpp"
			"runner.cpp"
			"suite.cpp"
			"tag.cpp"
			"exception.cpp"

)

target_sources(honesty_main
	PUBLIC
		FILE_SET interfaces
		FILES
			"main.cpp"
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