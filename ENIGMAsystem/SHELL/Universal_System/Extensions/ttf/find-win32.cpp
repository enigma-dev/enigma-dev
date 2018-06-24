#include "find.h"

#include <windows.h>

namespace enigma {

// if name, bold, and italic all match
// then it's a perfect score and we can
// return early from our search
static const unsigned WIN_FONT_PERFECT_SCORE = 3;

struct WinFontDescription {
  std::string name;
  bool bold, italic;
  unsigned matchScore;
  std::string fullName;
};

int CALLBACK EnumFamCallback(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm, DWORD FontType, LPVOID lParam) {
  if (FontType != TRUETYPE_FONTTYPE) return TRUE;

  WinFontDescription *fontDesc = (WinFontDescription*) lParam;
  bool nameEqual = (fontDesc->name.c_str() == lpelf->elfLogFont.lfFaceName);
  bool boldEqual = false;
  if (!fontDesc->bold) {
    if (lpelf->elfLogFont.lfWeight == FW_NORMAL)
      boldEqual = true;
  } else {
    if (lpelf->elfLogFont.lfWeight == FW_BOLD)
      boldEqual = true;
  }
  bool italicEqual = (fontDesc->italic == lpelf->elfLogFont.lfItalic);

  unsigned matchScore = nameEqual + boldEqual + italicEqual;
  if (matchScore > fontDesc->matchScore) {
    fontDesc->fullName = lpelf->elfLogFont.lfFaceName;
    fontDesc->matchScore = matchScore;
  }

  if (matchScore == WIN_FONT_PERFECT_SCORE) {
    return FALSE;
  }
  return TRUE;
}

std::string font_lookup(std::string name, bool bold, bool italic) {
  std::string font_dir = std::string(getenv("windir")) + "/fonts/";
  WinFontDescription fontDesc = { name, bold, italic, 0, "" };
  HDC hDC = GetDC(NULL); // just grab the "common" device context of the entire desktop
  EnumFontFamilies(hDC, name.c_str(), (FONTENUMPROC)EnumFamCallback, (LPARAM)&fontDesc);
  ReleaseDC(NULL, hDC); // the desktop is a "common" device context and must be released
  if (fontDesc.fullName.empty()) {
    return "";
  } else {
    return font_dir + fontDesc.fullName + ".ttf";
  }
}

}
