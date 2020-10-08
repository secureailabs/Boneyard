/*
 * locale.h
 *
 *  Created on: Dec 10, 2018
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_LOCALE_H_
#define GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_LOCALE_H_

#define LC_CTYPE 1
#define LC_NUMERIC 2
#define LC_ALL 3
#define CODESET 1

typedef int nl_item;

struct lconv{
	char* decimal_point;
	char* thousands_sep;
    char* grouping;
    char* int_curr_symbol;
    char* currency_symbol;
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char frac_digits;
	char p_cs_precedes;
    char n_cs_precedes;
    char p_sep_by_space;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
	char int_frac_digits;
};

char *setlocale(int category, const char *locale);
char *nl_langinfo(nl_item item);
struct lconv* localeconv(void);

#endif /* GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_LOCALE_H_ */
