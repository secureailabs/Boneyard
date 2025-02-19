#include "f2c.h"
#include "f2cf.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
VOID pow_ci(p, a, b) 	/* p = a**b  */
 complex *p, *a; integer *b;
#else
void pow_ci(complex *p, complex *a, integer *b) /* p = a**b  */
#endif
{
doublecomplex p1, a1;

a1.r = a->r;
a1.i = a->i;

pow_zi(&p1, &a1, b);

p->r = p1.r;
p->i = p1.i;
}
#ifdef __cplusplus
}
#endif
