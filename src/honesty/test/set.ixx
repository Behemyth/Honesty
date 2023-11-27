export module synodic.honesty.test:set;

import std;

import :generator;
import :test;

export namespace synodic::honesty
{
	class [[nodiscard]] Set
	{
	public:
		Set() = default;

		Set(const Set& other)	  = delete;
		Set(Set&& other) noexcept = default;

		Set& operator=(const Set& other)	 = delete;
		Set& operator=(Set&& other) noexcept = default;

	protected:
		std::string_view name_;
	};

}
