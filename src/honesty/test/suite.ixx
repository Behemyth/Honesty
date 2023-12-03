export module synodic.honesty.test:suite;

import std;

export namespace synodic::honesty
{
	class [[nodiscard]] suite
	{
	public:
		suite(std::string_view name, std::move_only_function<void()> generator);

		suite(const suite& other)	  = delete;
		suite(suite&& other) noexcept = default;

		suite& operator=(const suite& other)	 = delete;
		suite& operator=(suite&& other) noexcept = default;

	protected:

		inline static std::vector<suite> suites_;

		std::string_view name_;
		std::move_only_function<void()> generator_;
	};

}
