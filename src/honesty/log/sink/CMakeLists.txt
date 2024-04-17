target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"sink.ixx"
			"console.ixx"
	PRIVATE
		FILE_SET implementations
		FILES
			"console.cpp"
			"sink.cpp"
)

