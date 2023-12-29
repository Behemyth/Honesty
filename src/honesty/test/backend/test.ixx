export module synodic.honesty.test.backend:test;

import std;

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
