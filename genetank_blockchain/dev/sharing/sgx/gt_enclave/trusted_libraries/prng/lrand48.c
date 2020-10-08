#include <stdlib.h>
#include <inttypes.h>
#include "../include/rand.h"

unsigned long long __rand48_step(unsigned short *xi, unsigned short *lc);
extern unsigned short __seed48[7];

long nrand48(unsigned short s[3])
{
	return __rand48_step(s, __seed48+3) >> 17;
}

long lrand48(void)
{
	return nrand48(__seed48);
}
