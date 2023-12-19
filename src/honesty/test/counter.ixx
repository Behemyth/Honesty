export module counter;

import std;

export namespace std
{

	template<typename Context, int Start = 0, int Step = 1>
	class counter
	{
	public:
		template<typename Unique>
		static constexpr int next()
		{
			return next<Unique>(0) * Step + Start;
		}

		template<auto>
		static constexpr int next()
		{
			struct Unique
			{
			};

			return next<Unique>(0) * Step + Start;
		}

		template<typename Unique>
		static constexpr int current()
		{
			return current<Unique>(0) * Step + Start;
		}

		template<auto>
		static constexpr int current()
		{
			struct Unique
			{
			};

			return current<Unique>(0) * Step + Start;
		}

	private:
		template<int I>
		struct slot
		{
			friend constexpr auto slot_allocated(slot<I>);
		};

		template<int I>
		struct allocate_slot
		{
			friend constexpr auto slot_allocated(slot<I>)
			{
				return true;
			}

			enum
			{
				value = I
			};
		};

		// If slot_allocated(slot<I>) has NOT been defined, then SFINAE will keep this function out of the overload
		// set...
		template<typename Unique, int I = 0, bool = slot_allocated(slot<I>())>
		static constexpr int next(int)
		{
			return next<Unique, I + 1>(0);
		}

		// ...And this function will be used, instead, which will define slot_allocated(slot<I>) via allocate_slot<I>.
		template<typename Unique, int I = 0>
		static constexpr int next(double)
		{
			return allocate_slot<I>::value;
		}

		// If slot_allocated(slot<I>) has NOT been defined, then SFINAE will keep this function out of the overload
		// set...
		template<typename Unique, int I = Start, bool = slot_allocated(slot<I>())>
		static constexpr int current(int)
		{
			return current<Unique, I + 1>(0);
		}

		// ...And this function will be used, instead, which will return the current counter, or assert in case next()
		// hasn't been called yet.
		template<typename Unique, int I = Start>
		static constexpr int current(double)
		{
			static_assert(I != 0, "You must invoke next() first");

			return I - 1;
		}
	};

}
