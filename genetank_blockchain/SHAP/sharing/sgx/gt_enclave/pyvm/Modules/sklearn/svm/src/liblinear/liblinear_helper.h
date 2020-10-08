#ifndef LIBLINEAR_HELPER_H
#define LIBLINEAR_HELPER_H

#include "./linear.h"
#include <numpy/arrayobject.h>

struct problem * set_problem_linear(char *X,char *Y, npy_intp *dims, double bias, char* sample_weight);
struct problem * csr_set_problem_linear (char *values, npy_intp *n_indices,
	char *indices, npy_intp *n_indptr, char *indptr, char *Y,
        npy_intp n_features, double bias, char *sample_weight);
struct parameter *set_parameter_linear(int solver_type, double eps, double C,
                                npy_intp nr_weight, char *weight_label,
                                char *weight, int max_iter, unsigned seed, 
                                double epsilon);
void copy_w_linear(void *data, struct model *model, int len);
double get_bias_linear(struct model *model);
void free_problem_linear(struct problem *problem);
void free_parameter_linear(struct parameter *param);
void set_verbosity_linear(int verbosity_flag);


#endif
