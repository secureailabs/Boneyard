#pragma once
#include "../include/stdafx.h"

bool isxdigit(char ch);
char toupper(char ch);
void myHex2Bin(const utility::string_t& source, int max_size, uint8_t *buf);
utility::string_t mBin2Hex(const uint8_t *source, int count);
uint8_t hexnibble(char ch);

