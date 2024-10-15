target_sources(honesty
	PUBLIC
		FILE_SET log_public
		TYPE CXX_MODULES
		FILES
			sink.ixx
			console.ixx
			ringbuffer.ixx
			synchronized.ixx
			ostream.ixx
			callback.ixx
			scatter.ixx
	PRIVATE
		FILE_SET log_private
		TYPE CXX_MODULES
)
