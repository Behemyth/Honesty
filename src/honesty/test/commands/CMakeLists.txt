target_sources(honesty
	PUBLIC
		FILE_SET test_commands_public
		TYPE CXX_MODULES
		FILES
			commands.ixx
			list.ixx
	PRIVATE
		FILE_SET test_commands_private
		TYPE CXX_MODULES
		FILES
			execute.cpp
			types.cpp
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)
