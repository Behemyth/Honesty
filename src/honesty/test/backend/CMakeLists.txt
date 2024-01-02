target_sources(honesty
	PRIVATE
		FILE_SET implementations
		TYPE CXX_MODULES
		FILES
			"backend.ixx"
			"generator.ixx"
			"test.ixx"
			"suite.ixx"
			"runner.ixx"
			"reporter.ixx"
			"logger.ixx"

	PRIVATE
		"backend.cpp"
		"runner.cpp"
		"suite.cpp"
		"test.cpp"
		"reporter.cpp"
		"logger.cpp"
)