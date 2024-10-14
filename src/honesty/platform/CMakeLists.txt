# Provide the platform library
add_library(platform STATIC)
add_library(synodic::platform ALIAS platform)

# platform module definition
target_sources(platform
	PUBLIC # For building the library and dependents
		FILE_SET platform_public
		TYPE CXX_MODULES
		FILES
			platform.ixx
	PRIVATE # For building the target
		FILE_SET platform_private
		TYPE CXX_MODULES
)

target_link_libraries(platform
	PUBLIC
		properties_public
	PRIVATE
		properties_private
)
