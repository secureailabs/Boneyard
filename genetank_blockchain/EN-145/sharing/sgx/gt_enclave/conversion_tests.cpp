// conversion_tests.cpp
#include "conversion.cpp"
#include "gtest/gtest.h"
#include <string>
#include <stdint.h>

TEST(IsLetter, Valid) {
  ASSERT_TRUE(isLetter('a'));
  ASSERT_TRUE(isLetter('m'));
  ASSERT_TRUE(isLetter('M'));
  ASSERT_TRUE(isLetter('Z'));
}

TEST(IsLetter, Invalid) {
  ASSERT_FALSE(isLetter('0'));
  ASSERT_FALSE(isLetter('1'));
}

TEST(bin2hex, size) {
	uint8_t src[32], dst[64];
	uint32_t size=17;
	for (int i=0; i<32; i++){
		src[i]=32+i;
	}
	for (int i=0; i<64; i++){
		dst[i]=0;
	}
	bin2hex(src, size, (char *)dst);
	ASSERT_TRUE(dst[0]=='2');
	ASSERT_TRUE(dst[1]=='0');
	ASSERT_TRUE(dst[(size-1)*2]=='3');
	ASSERT_TRUE(dst[(size-1)*2+1]=='0');
	ASSERT_TRUE(dst[(size)*2]==0);
	ASSERT_TRUE(dst[(size)*2+1]==0);
}
