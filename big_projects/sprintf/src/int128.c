#include "../include/int128.h"

bool is_zero(int128_t value)
{
	return value.high == 0 && value.low == 0;
}

bool is_negative128(int128_t value)
{
	return (value.high & 0x8000000000000000ULL) != 0;
}

int128_t add128(int128_t a, int128_t b)
{
	int128_t result;
	result.low = a.low + b.low;
	result.high = a.high + b.high + (result.low < a.low);
	return result;
}

int128_t negate128(int128_t value)
{
	int128_t result = { ~value.high, ~value.low };
	return add128(result, (int128_t){ .high = 0, .low = 1 });
}

int128_t multiply128(int128_t value, int base)
{
	int128_t result = { 0, 0 };
	for (int i = 0; i < base; ++i)
	{
		result = add128(result, value);
	}
	return result;
}

int128_t divide128(int128_t value, int base, uint64_t *remainder)
{
	int128_t result = { 0, 0 };
	uint64_t rem_high = value.high % base;
	result.high = value.high / base;

	uint64_t combined = (rem_high << 32) | (value.low >> 32);
	result.low = (combined / base) << 32;

	uint64_t rem_low = combined % base;
	combined = (rem_low << 32) | (value.low & 0xFFFFFFFF);
	result.low |= combined / base;

	if (remainder)
	{
		*remainder = combined % base;
	}

	return result;
}
