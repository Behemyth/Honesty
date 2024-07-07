target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"log.ixx"
			"colour.ixx"
			"logger.ixx"
			"types.ixx"
)

add_subdirectory(sink)
