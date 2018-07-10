#ifndef __ENCRYPTION_H__
#define __ENCRYPTION_H__

#include "hackbase.h"

std::string XOR(std::string str, int key);

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

std::string encode(std::string data);
std::string decode(std::string data);

#endif