/*
 *	This module provides a simple interface for reading and writing JSON files. The implementation is not meant to
 *		compete with the ergonomics or the efficiency of other libraries. Instead, it is meant to provide the small
 *		utility functions that are need for JSON interfaces while not depending on other libraries.
 */

export module synodic.honesty.utility:json;

import std;

namespace synodic::honesty::utility
{
	// TODO: Use c++26 reflection for a simple writer without pulling in a library
	export class JSON
	{
	public:
		JSON()	= default;
		~JSON() = default;

		JSON(const JSON& other)				   = default;
		JSON(JSON&& other) noexcept			   = default;
		JSON& operator=(const JSON& other)	   = default;
		JSON& operator=(JSON&& other) noexcept = default;

		JSON& operator[](const std::string_view& key)
		{
			if (std::holds_alternative<std::monostate>(data_))
			{
			}

			return data_[std::string(key)];
		}

	private:
		std::variant<std::monostate, bool, int, std::uint32_t, float, std::unordered_set<JSON>> data_;
	};

}
