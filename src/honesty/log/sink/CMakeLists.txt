target_sources(log
	PUBLIC # For building the library and dependents
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
	PRIVATE # For building the target
		FILE_SET log_private
		TYPE CXX_MODULES
)
