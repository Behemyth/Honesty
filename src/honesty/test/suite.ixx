export module synodic.honesty.test:suite;

import std;

import :test;

export namespace synodic::honesty
{
	class suite;

	class suite_registrar
	{
	public:
		static std::span<suite> suites();

	protected:
		inline static std::vector<suite> suites_;

	};

	class [[nodiscard]] suite final : protected suite_registrar
	{
	public:
		suite(std::string_view name, std::move_only_function<Generator()> generator);

		suite(const suite& other)	  = delete;
		suite(suite&& other) noexcept = default;

		suite& operator=(const suite& other)	 = delete;
		suite& operator=(suite&& other) noexcept = default;

	private:
		std::string_view name_;
		std::move_only_function<Generator()> generator_;
	};

}
