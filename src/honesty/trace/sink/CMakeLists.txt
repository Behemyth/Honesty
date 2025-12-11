target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			sink.ixx
			otel.ixx
			log.ixx
			scatter.ixx
	PRIVATE
		FILE_SET trace_private
		TYPE CXX_MODULES
)

target_sources(honesty
	PRIVATE
		otel.cpp
)
