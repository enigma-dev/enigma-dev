#ifndef LIBBASE64_UTIL_H
#define LIBBASE64_UTIL_H

#include <string>

bool is_base64(unsigned char byte);
std::string base64_encode(std::string const& str);
std::string base64_decode(std::string const& str);

#endif // LIBBASE64_UTIL_H
