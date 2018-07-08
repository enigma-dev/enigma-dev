#include "find.h"

#include <windows.h>

namespace enigma {

std::string font_lookup(std::string name, bool bold, bool italic, unsigned char** buffer, size_t &buffer_size) {
  HDC hDC = GetDC(NULL); // just grab the "common" device context of the entire desktop

  LOGFONT requestedFont = { };
  requestedFont.lfWeight = bold ? FW_BOLD : FW_NORMAL;
  requestedFont.lfItalic = italic;
  requestedFont.lfCharSet = DEFAULT_CHARSET;
  strncpy(requestedFont.lfFaceName, name.c_str(), name.length() < LF_FACESIZE ? name.length() + 1 : (LF_FACESIZE - 1));
  if (name.length() >= LF_FACESIZE) requestedFont.lfFaceName[LF_FACESIZE - 1] = '\0';
  HFONT hFont = CreateFontIndirect(&requestedFont);

  SelectObject(hDC, hFont);
  buffer_size = GetFontData(hDC, 0, 0, NULL, 0);
  *buffer = new unsigned char[buffer_size];
  GetFontData(hDC, 0, 0, *buffer, buffer_size);

  DeleteObject(hFont);
  ReleaseDC(NULL, hDC); // the desktop is a "common" device context and must be released

  return "";
}

}
