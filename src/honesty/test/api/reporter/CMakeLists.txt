target_sources(honesty
	PUBLIC
		FILE_SET test_commands_reporter_public
		TYPE CXX_MODULES
		FILES
			compact.ixx
			standard.ixx
	PRIVATE
		FILE_SET test_commands_reporter_private
		TYPE CXX_MODULES
)
