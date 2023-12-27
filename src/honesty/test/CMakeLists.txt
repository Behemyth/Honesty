target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		TYPE CXX_MODULES
		FILES
			"test.ixx"
			"runner.ixx"
			"reporter.ixx"
			"logger.ixx"
			"generator.ixx"
			"function_ref.ixx"
	PRIVATE
		FILE_SET implementations
		TYPE CXX_MODULES
		FILES
			"backend.ixx"
	PRIVATE
		"test.cpp"
		"runner.cpp"
		"logger.cpp"
		"reporter.cpp"
		"backend.cpp"

)

target_sources(honesty_main
	PUBLIC
		"main.cpp"
)

add_subdirectory(
	entry
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