target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			json.ixx
			utility.ixx
	PRIVATE
		FILE_SET utility_private
		TYPE CXX_MODULES
)
