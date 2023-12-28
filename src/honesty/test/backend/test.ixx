export module synodic.honesty.test.backend:test;

export namespace synodic::honesty
{
	class TestContext
	{
	};

	class TestBase
	{
	public:
		virtual ~TestBase()								   = default;
		virtual void Run() const = 0;

	private:
	};
}
