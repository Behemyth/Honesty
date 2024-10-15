target_sources(honesty
	PUBLIC
		FILE_SET test_public
		TYPE CXX_MODULES
		FILES
			test.ixx
			reporter.ixx
			runner.ixx
			frontend.ixx
			suite.ixx
	PRIVATE
		FILE_SET test_private
		TYPE CXX_MODULES
)

add_subdirectory(
	api
)

add_subdirectory(
	core
)

add_subdirectory(
	entry
)

add_subdirectory(
	backend
)

add_subdirectory(
	commands
)

add_subdirectory(
	types
)
