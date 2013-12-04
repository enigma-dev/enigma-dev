#include "workdir.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
	#include <cstdlib>
	std::string workdir = getenv(TEMP) + "/";
#else
	std::string workdir = "/tmp/";
#endif
