export module synodic.honesty.test.logger:stdout;

import std;

export namespace synodic::honesty::logger
{
	class Local
	{
	public:
		template<class... Args>
		void log(std::format_string<Args...> fmt, Args&&... args)
		{
			std::print(fmt, std::forward<Args>(args)...);
		}

	private:
	};
}
