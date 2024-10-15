target_sources(honesty
	PUBLIC
		FILE_SET platform_public
		TYPE CXX_MODULES
		FILES
			platform.ixx
	PRIVATE
		FILE_SET platform_private
		TYPE CXX_MODULES
)
