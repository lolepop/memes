#pragma once

template<int sz>
class O
{
private:
	mutable char out[sz];

	template<int sz> // took me half an hour to figure out how to write recursive constexpr functions (don't use the if statements to check for base cases)
	struct Enc
	{
		static constexpr void enc(const char* in, char* out, const char k)
		{
			out[sz] = in[sz] ^ k;
			Enc<sz - 1>::enc(in, out, k);
		}
	};

	template<>
	struct Enc<0>
	{
		static constexpr void enc(const char* in, char* out, const char k)
		{
			out[0] = in[0] ^ k;
		}
	};

public:
	const __declspec(noinline) char* dec(const char k)
	{
		for (int i = 0; i < sz; i++)
			out[i] ^= k;
		return out;
	}

	constexpr O(const char s[sz], const char k)
	{
		Enc<sz - 1>::enc(s, out, k);
	}
};

template <int sz>
static constexpr auto init(const char(&s)[sz], const char k)
{
	return O<sz>(s, k);
}
