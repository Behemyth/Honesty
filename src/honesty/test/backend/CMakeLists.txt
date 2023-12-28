target_sources(honesty
	PRIVATE
		FILE_SET implementations
		TYPE CXX_MODULES
		FILES
			"backend.ixx"
			"generator.ixx"
			"test.ixx"

	PRIVATE
		"backend.cpp"

)