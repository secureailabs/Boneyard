/*
 * locale.c
 *
 *  Created on: Dec 10, 2018
 *      Author: jingwei
 */

#include "../Include/enclave_wrapper/locale.h"

char *setlocale(int category, const char *locale){
    return "";
}

char *nl_langinfo(nl_item item){
	return "";
}

struct lconv* localeconv(){
	static struct lconv ret;
	static char a='.';
	static char b=' ';
	static char c='+';
	static char d='-';
	ret.decimal_point=&a;
	ret.thousands_sep=&b;
	ret.grouping=&b;
	ret.int_curr_symbol=&b;
	ret.currency_symbol=&b;
	ret.mon_decimal_point=&b;
	ret.mon_thousands_sep=&b;
	ret.mon_grouping=&b;
	ret.positive_sign=&c;
	ret.negative_sign=&d;
	ret.frac_digits=' ';
	ret.p_cs_precedes=' ';
	ret.n_cs_precedes=' ';
	ret.p_sep_by_space=' ';
	ret.n_sep_by_space=' ';
	ret.p_sign_posn=' ';
	ret.n_sign_posn=' ';
	ret.int_frac_digits=' ';
	return &ret;
}
