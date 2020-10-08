#ifndef STATS_MISC_H
#define STATS_MICS_H

#define DO_NOT_USE_CXX_HEASERS
#include "../../include/f2c.h"
#include <R_ext/RS.h>

//from loessf.c
int lowesb_(doublereal *xx, doublereal *yy, doublereal *ww,
	doublereal *diagl, integer *infl, integer *iv, integer *liv, integer *
	lv, doublereal *wv);

int lowesd_(integer *versio, integer *iv, integer *liv,
	integer *lv, doublereal *v, integer *d__, integer *n, doublereal *f,
	integer *ideg, integer *nvmax, integer *setlf);

int F77_NAME(ehg169)(int*, int*, int*, int*, int*, int*,
		      double*, int*, double*, int*, int*, int*);
int F77_NAME(ehg196)(int*, int*, double*, double*);
/* exported (for loessf.f) : */
//int F77_SUB(ehg182)(int *i);
void F77_SUB(ehg182)(int *i);
//int F77_SUB(ehg183a)(char *s, int *nc,int *i,int *n,int *inc);
void F77_SUB(ehg183a)(char *s, int *nc,int *i,int *n,int *inc);
//int F77_SUB(ehg184a)(char *s, int *nc, double *x, int *n, int *inc);
void F77_SUB(ehg184a)(char *s, int *nc, double *x, int *n, int *inc);

int dv7dfl_(integer *, integer *, doublereal *);
void F77_NAME(dv7cpy)(int *n, double dest[], double src[]);
void F77_NAME(divset)(int *Alg, int iv[], int *Liv,
		      int *Lv, double v[]);
void F77_NAME(ditsum)(double d[], double g[],
		      int iv[], int *liv, int *lv,
		      int *n, double v[], double x[]);
void F77_NAME(dn2cvp)(int iv[], int *liv, int *lv, int *p,
		      double v[]);
void F77_NAME(i7pnvr)(int *n, int x[], int y[]);
void F77_NAME(dv7ipr)(int *n, int ip[], double x[]);
int F77_NAME(stopx)(int*);
void F77_NAME(dv2axy)(int *n, double w[], double *a,
		      double x[], double y[]);
void F77_NAME(i7copy)(int *n, int dest[], int src[]);
void F77_NAME(dv7scl)(int *n, double dest[],
		      double *scal, double src[]);

//functions in init, called by R
int hclust_(integer *n, integer *len, integer *iopt, integer
	*ia, integer *ib, doublereal *crit, doublereal *membr, integer *nn,
	doublereal *disnn, integer *flag__, doublereal *diss);









#endif
