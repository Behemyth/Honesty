target_sources(honesty
	PUBLIC
		FILE_SET profile_public
		TYPE CXX_MODULES
		FILES
			profile.ixx
			timer.ixx
			types.ixx
	PRIVATE
		FILE_SET profile_private
		TYPE CXX_MODULES
)
