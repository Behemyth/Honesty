# Provide the log library
add_library(log STATIC)
add_library(synodic::log ALIAS log)

# log module definition
target_sources(log
	PUBLIC # For building the library and dependents
		FILE_SET log_public
		TYPE CXX_MODULES
		FILES
			log.ixx
			colour.ixx
			logger.ixx
			types.ixx
	PRIVATE # For building the target
		FILE_SET log_private
		TYPE CXX_MODULES
)

target_link_libraries(log
	PUBLIC
		properties_public
	PRIVATE
		properties_private
)

add_subdirectory(sink)
