target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"colour.ixx"
			"logger.ixx"
			"types.ixx"
	PRIVATE
		FILE_SET implementations
		FILES
			"colour.cpp"
			"logger.cpp"
)

add_subdirectory(sink)
