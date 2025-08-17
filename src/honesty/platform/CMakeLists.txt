target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			build.ixx
			platform.ixx
	PRIVATE
		FILE_SET platform_private
		TYPE CXX_MODULES
)
