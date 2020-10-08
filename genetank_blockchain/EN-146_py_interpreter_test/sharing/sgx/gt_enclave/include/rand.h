#ifndef RAND_PRNG_H
#define RNAD_PRNG_H

#include <stdio.h>

#define	RAND_MAX	0x7fffffff

#ifdef __cplusplus
extern "C"{
#endif

double	 drand48(void);
double	 erand48(unsigned short[3]);
void	 srand48(long);
unsigned short*  seed48(unsigned short[3]);
int	 rand_r(unsigned int *);
void	 lcong48(unsigned short[7]);
long	 lrand48(void);
long	 nrand48(unsigned short[3]);
long	 jrand48(unsigned short[3]);
long	 mrand48(void);
int	 rand(void);
void	 srand(unsigned);
unsigned long long __rand48_step(unsigned short *, unsigned short *);
void	 srandom(unsigned int);
char*    initstate(unsigned, char *, size_t);
char*    setstate(char *);
long	 random(void);

#ifdef __cplusplus
}
#endif

#endif
