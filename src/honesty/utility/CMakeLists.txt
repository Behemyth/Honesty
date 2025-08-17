target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			exception.ixx
			hash.ixx
			json.ixx
			module.ixx
			inplace_vector.ixx
			inplace_string.ixx
			function_ref.ixx
			fixed_string.ixx
			traits.ixx
			zstring_view.ixx
	PRIVATE
		FILE_SET utility_private
		TYPE CXX_MODULES
)
