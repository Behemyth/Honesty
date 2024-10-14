# Provide the profile library
add_library(profile STATIC)
add_library(synodic::profile ALIAS profile)

# profile module definition
target_sources(profile
	PUBLIC # For building the library and dependents
		FILE_SET profile_public
		TYPE CXX_MODULES
		FILES
			profile.ixx
	PRIVATE # For building the target
		FILE_SET profile_private
		TYPE CXX_MODULES
)

target_link_libraries(profile
	PUBLIC
		properties_public
	PRIVATE
		properties_private
)
