#include <stdlib.h>
#include <inttypes.h>
#include "../include/rand.h"

unsigned long long __rand48_step(unsigned short *xi, unsigned short *lc);
extern unsigned short __seed48[7];

long jrand48(unsigned short s[3])
{
	return (int32_t)(__rand48_step(s, __seed48+3) >> 16);
}

long mrand48(void)
{
	return jrand48(__seed48);
}
