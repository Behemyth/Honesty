export module synodic.honesty.test.types:tag;

import std;
import fixed_string;
import inplace_vector;
import synodic.honesty.utility;

namespace synodic::honesty::test
{
	/**
	 * @brief TODO: Add description
	 */
	export class Tag
	{
		// NOTE: Tags must not be templated. The type must be shared between all instances such that conditional
		//	logic can be applied to all tags regardless of their stored internal state.

		static constexpr int MAX_TAGS	   = 8;
		static constexpr int MAX_NAME_SIZE = 13;

	public:
		using value_type			 = utility::InplaceString<MAX_NAME_SIZE>;
		using pointer				 = value_type*;
		using const_pointer			 = const value_type*;
		using reference				 = value_type&;
		using const_reference		 = const value_type&;
		using const_iterator		 = const value_type*;
		using iterator				 = const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using reverse_iterator		 = const_reverse_iterator;
		using size_type				 = std::size_t;
		using difference_type		 = std::ptrdiff_t;

		template<std::convertible_to<char>... Chars>
			requires(... && !std::is_pointer_v<Chars>)
		consteval explicit Tag(Chars... chars) noexcept
		{
		}

		template<std::size_t N>
			requires(N - 1 <= MAX_NAME_SIZE)
		explicit consteval Tag(const char (&tag)[N]) noexcept
		{
		}

		constexpr explicit Tag(const std::string_view view) noexcept 
		{
		}

		constexpr Tag(const Tag&) noexcept			  = default;
		constexpr Tag& operator=(const Tag&) noexcept = default;

		constexpr Tag operator/(const Tag& tag) const
		{
			return Tag();
		}

		constexpr std::size_t Size() const noexcept
		{
			return tags_.size();
		}

		constexpr std::span<const value_type> View() const noexcept
		{
			return tags_;
		}

		friend constexpr bool operator==(const Tag& first, const Tag& second)
		{
			for (auto& tag: second.tags_)
			{
				if (std::ranges::contains(first.tags_, tag))
				{
					return true;
				}
			}
			return false;
		}

		friend constexpr bool operator==(const Tag& first, const std::string_view second)
		{
			const bool result = std::ranges::contains(first.tags_, second);
			return result;
		}

	private:
		std::inplace_vector<value_type, MAX_TAGS> tags_;
	};

}
