export module synodic.honesty.test.backend:test;


import std;
import function_ref;

export namespace synodic::honesty
{
	class TestContext
	{
	};

	class TestBase
	{
	public:
		virtual ~TestBase()		 = default;
		virtual void Run() const = 0;

		virtual std::span<std::string_view> Tags() const = 0;

	private:
	};

}
