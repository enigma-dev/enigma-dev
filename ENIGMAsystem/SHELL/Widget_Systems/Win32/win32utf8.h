#include <windows.h>
#include <wchar.h>
#include <string>
#include <vector>
using std::string;

typedef std::basic_string<WCHAR> tstring;
tstring widen(const string &str);
string shorten(tstring str);
