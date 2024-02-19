target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		TYPE CXX_MODULES
		FILES
			"platform.ixx"
	PRIVATE
		"platform.cpp"
)