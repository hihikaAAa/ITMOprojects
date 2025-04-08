#ifndef INT128_H
#define INT128_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
	uint64_t high;
	uint64_t low;
} int128_t;

bool is_zero(int128_t value);
bool is_negative128(int128_t value);
int128_t add128(int128_t a, int128_t b);
int128_t negate128(int128_t value);
int128_t multiply128(int128_t value, int base);
int128_t divide128(int128_t value, int base, uint64_t *remainder);

#endif	  // INT128_H
