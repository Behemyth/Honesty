export module synodic.honesty.platform;

import std;

export namespace synodic::honesty::platform
{

	enum class PlatformID
	{
		WINDOWS,
		OSX,
		LINUX,
		ANDROID,
		UNKNOWN
	};

	class Platform
	{
	public:
		constexpr static PlatformID GetPlatform()
		{
			return PLATFORM;
		}

		constexpr static bool IsDesktop()
		{
			return PLATFORM == PlatformID::LINUX || PLATFORM == PlatformID::WINDOWS || PLATFORM == PlatformID::OSX;
		}

	private:
#if defined(__linux__) && !defined(__ANDROID__)

		constexpr static PlatformID PLATFORM = PlatformID::Linux;

#elif defined(_WIN64)

		constexpr static auto PLATFORM = PlatformID::WINDOWS;

#elif defined(__APPLE__)

		constexpr static PlatformID PLATFORM = PlatformID::OSX;

#elif defined(__ANDROID__)

		constexpr static PlatformID PLATFORM = PlatformID::Android;

#else

		constexpr static PlatformID PLATFORM = PlatformID::Unknown;

#endif
	};

	enum class CompilerID
	{
		VISUAL_STUDIO,
		GCC,
		CLANG,
		EMSCRIPTEN,
		UNKNOWN
	};

	class Compiler
	{
	public:
		constexpr static CompilerID GetCompiler()
		{
			return COMPILER;
		}

		constexpr static bool Debug()
		{
			return DEBUG;
		}

	private:
#if defined(_MSC_VER)

		constexpr static auto COMPILER = CompilerID::VISUAL_STUDIO;

#elif defined(__GNUC__)

		constexpr static auto COMPILER = CompilerID::GCC;

#elif defined(__clang__)

		constexpr static auto COMPILER = CompilerID::Clang;

#elif defined(__EMSCRIPTEN__)

		constexpr static auto COMPILER = CompilerID::Emscripten;

#else

		constexpr static auto COMPILER = CompilerID::Unknown;

#endif

// TODO: Update debug macro. C++23/26?
#if defined(NDEBUG)

		constexpr static bool DEBUG = false;

#else

		constexpr static bool DEBUG = true;

#endif
	};
}

module :private;

namespace synodic::honesty::platform
{

}
