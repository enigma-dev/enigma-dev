// assumes correct headers and fs namespace included/using before calling macro
#if (defined(_WIN32) && !defined_WIN64)
/* Tempfix for fs::temp_directory_path() on Michaelsoft Binbows
Broken in MinGW and Visual Studio since std::fs's conception */
#define TEMP_DIRECTORY_PATH fs::path(getenv("TMP") ? getenv("TMP") : "C:/").string()
#else
#define TEMP_DIRECTORY_PATH fs::temp_directory_path().string()
#endif
