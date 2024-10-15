target_sources(honesty
	PUBLIC
		FILE_SET test_core_public
		TYPE CXX_MODULES
	PRIVATE
		FILE_SET test_core_private
		TYPE CXX_MODULES
		FILES
			core.ixx
			suite_context.ixx
			test_context.ixx
)
