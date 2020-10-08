#include "tools.h"

#include "../../gt_enclave/src/conversion.cpp"

bool isxdigit(char ch) {
	return ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'));
}

char toupper(char ch) {
	if (ch >= 'a' && ch <= 'z') {
		return ch - 'a' + 'A';
	}
	return ch;
}
void myHex2Bin(const utility::string_t& source, int max_size, uint8_t *buf)
{
	static int nibbles[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15 };
	int ind = 0;
	if (source[0] == '0' && (source[1] == 'x' || source[1] == 'X'))
		ind = 2;
	bool str_end = false;
	for (int i = 0; i<max_size; i++) {
		int v = 0, idx = i * 2 + ind;
		if (!str_end) {
			str_end = (source[idx] == 0 || source[idx + 1] == 0);
			if (isxdigit(source[idx])) {
				v = nibbles[toupper(source[idx]) - '0'] << 4;
			}
			if (source[idx + 1] != 0 && isxdigit(source[idx + 1]))
				v += nibbles[toupper(source[idx + 1]) - '0'];
		}
		buf[i] = (uint8_t)v;
	}
}

utility::string_t mBin2Hex(const uint8_t *source, int count)
{
	static char syms[] = "0123456789ABCDEF";
	utility::string_t ss;
	ss += '0'; ss += 'x';
	for (int i = 0; i < count; i++) {
		ss += syms[((source[i] >> 4) & 0xf)];
		ss += syms[source[i] & 0xf];
	}
	return ss;
}

uint8_t hexnibble(char ch) {
	if (ch >= '0'&&ch <= '9')
		return ch - '0';
	if (ch >= 'A'&&ch <= 'F')
		return ch - 'A' + 10;
	if (ch >= 'a'&&ch <= 'f')
		return ch - 'a' + 10;
	return 0;
}


uint64_t currentSecond(){
	time_t timer;
	struct tm y2018 = {0};
	uint64_t seconds;

	y2018.tm_hour = 0;   y2018.tm_min = 0; y2018.tm_sec = 0;
	y2018.tm_year = 2018-1900; y2018.tm_mon = 0; y2018.tm_mday = 1;

	time(&timer);  /* get current time; same as: timer = time(NULL)  */

	seconds = difftime(timer,mktime(&y2018));
	return seconds;
}

