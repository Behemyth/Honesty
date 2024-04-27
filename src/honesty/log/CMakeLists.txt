target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"colour.ixx"
			"logger.ixx"
			"types.ixx"
)

add_subdirectory(sink)
