export module synodic.honesty.test:suite;

import std;

import :test;

export namespace synodic::honesty
{
	class suite;

	/**
	 * \brief The management of all suites. Useful only if implementing your own test entrypoint
	 */
	class suite_registrar
	{
	public:

		void execute();

	protected:
		static constinit std::vector<suite> suites_;

	};

	/**
	 * \brief Type to create a suite with
	 */
	class [[nodiscard]] suite final : protected suite_registrar
	{
	public:
		suite(std::string_view name, std::move_only_function<Generator()> generator);

		suite(const suite& other)	  = delete;
		suite(suite&& other) noexcept = default;

		suite& operator=(const suite& other)	 = delete;
		suite& operator=(suite&& other) noexcept = default;

	private:
		friend suite_registrar;

		std::string_view name_;
		std::move_only_function<Generator()> generator_;
	};

}
