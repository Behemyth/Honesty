export module synodic.honesty.test:set;

import std;

import :generator;
import :test;

export namespace synodic::honesty
{
	class [[nodiscard]] Set
	{
	public:
		Set(std::string_view name);

		Set(std::string_view name, std::move_only_function<Generator()>&& generator);

		Set(const Set& other)	  = delete;
		Set(Set&& other) noexcept = default;

		Set& operator=(const Set& other)	 = delete;
		Set& operator=(Set&& other) noexcept = default;

		Set& operator=(std::move_only_function<Generator()>&& generator) noexcept;

		explicit operator Generator();

	protected:
		std::string_view name_;
		std::move_only_function<Generator()> callable_;
	};

	
}
