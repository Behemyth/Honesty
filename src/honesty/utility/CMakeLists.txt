target_sources(honesty
	PUBLIC
		FILE_SET utility_public
		TYPE CXX_MODULES
		FILES
			exception.ixx
			hash.ixx
			json.ixx
			utility.ixx
			inplace_vector.ixx
			inplace_string.ixx
			generator.ixx
			function_ref.ixx
			fixed_string.ixx
			traits.ixx
	PRIVATE
		FILE_SET utility_private
		TYPE CXX_MODULES
)
