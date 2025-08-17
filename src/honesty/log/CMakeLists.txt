target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			log.ixx
			colour.ixx
			logger.ixx
			types.ixx
	PRIVATE
		FILE_SET log_private
		TYPE CXX_MODULES
)

add_subdirectory(sink)
