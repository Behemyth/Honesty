target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			standard.ixx
	PRIVATE
		FILE_SET test_commands_runner_private
		TYPE CXX_MODULES
)
