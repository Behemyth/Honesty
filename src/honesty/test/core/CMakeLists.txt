target_sources(honesty
	PUBLIC
		FILE_SET test_core_public
		TYPE CXX_MODULES
		FILES
			core.ixx
	PRIVATE
		FILE_SET test_core_private
		TYPE CXX_MODULES
)
