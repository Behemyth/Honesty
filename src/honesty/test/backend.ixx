export module synodic.honesty.test:backend;

import std;

import :generator;

export namespace synodic::honesty
{
	class TestBase
	{
	public:
		virtual ~TestBase() = default;
		virtual void Run()	= 0;

	private:
	};

	using Generator = std::generator<TestBase>;
}
