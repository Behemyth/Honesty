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
		JSON& operator[](const std::string_view& key)
		{
			return data_[std::string(key)];
		}

		JSON(const JSON& other) = default;

	private:
		std::variant<bool, int, std::uint32_t, float, std::map<std::string, JSON>> data_;
	};

}
