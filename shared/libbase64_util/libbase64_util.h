#ifndef LIBBASE64_UTIL_H
#define LIBBASE64_UTIL_H

#include <string>

bool libbase64_is_base64(unsigned char byte);
std::string libbase64_encode(std::string const& str);
std::string libbase64_decode(std::string const& str);

#endif // LIBBASE64_UTIL_H
