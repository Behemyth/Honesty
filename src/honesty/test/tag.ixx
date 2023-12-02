export module synodic.honesty.test:tag;

import std;

export namespace synodic::honesty
{
	class tag
	{
	public:
		explicit consteval tag(std::string_view)
		{
		}

	private:
	};

	tag skip("skip");
}
