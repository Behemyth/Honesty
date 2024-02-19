target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		TYPE CXX_MODULES
		FILES
			"terminal.ixx"
	PRIVATE
		"terminal.cpp"
)