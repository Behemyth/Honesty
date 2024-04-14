target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"colour.ixx"
	PRIVATE
		FILE_SET implementations
		FILES
			"colour.cpp"
)
