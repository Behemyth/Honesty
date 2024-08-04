target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			exception.ixx
			hash.ixx
			json.ixx
			utility.ixx
			inplace_vector.ixx
			generator.ixx
			function_ref.ixx
			fixed_string.ixx
)
