/* strdup() replacement (from stdwin, if you must know) */

#include "pgenheaders.h"

//#define strdup strdup_orig
//#undef strdup
//
//char *
//strdup(const char *str)
//{
//	if (str != NULL) {
//		register char *copy = malloc(strlen(str) + 1);
//		if (copy != NULL)
//			return strcpy(copy, str);
//	}
//	return NULL;
//}
