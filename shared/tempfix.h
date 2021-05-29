// assumes correct headers and fs namespace included/using before calling macro
#if (defined(_WIN32) && !defined(_WIN64))
/* Tempfix for fs::temp_directory_path() on Michaelsoft Binbows (MinGW 32-bit) */
#define TEMP_DIRECTORY_PATH fs::path(getenv("TMP") ? getenv("TMP") : "C:/").string()
#else
#define TEMP_DIRECTORY_PATH fs::temp_directory_path().string()
#endif
