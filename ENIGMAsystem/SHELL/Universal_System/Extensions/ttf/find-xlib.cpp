#include "find.h"

#include <X11/Xlib.h>

#include <climits>

namespace enigma {

// if name, bold, and italic all match
// then it's a perfect score and we can
// return early from our search
static const unsigned X11_FONT_PERFECT_SCORE = 3;

std::string font_lookup(std::string name, bool bold, bool italic, unsigned char** buffer, size_t &buffer_size) {
  Display *display = XOpenDisplay(NULL);

  char** fontNames;
  int fontCount = 0;
  XFontStruct *fontInfos;

  fontNames = XListFontsWithInfo(display, name.c_str(), INT_MAX, &fontCount, &fontInfos);

  std::string bestMatch = "";
  unsigned bestScore = 0;

  for (int i = 0; i < fontCount; ++i) {
    char* fontName = fontNames[i];
    XFontStruct &fontInfo = fontInfos[i];

    unsigned matchScore = (name == fontName);

    for (int j = 0; j < fontInfo.n_properties; ++j) {
      XFontProp &fontProp = fontInfo.properties[j];

      if (strcmp(XGetAtomName(display, fontProp.name), "RELATIVE_WEIGHT") == 0) {
        if (bold) {
          if (fontProp.card32 == 70) // bold
            matchScore++;
        } else {
          if (fontProp.card32 == 50) // normal
            matchScore++;
        }
      } else if (strcmp(XGetAtomName(display, fontProp.name), "SLANT") == 0) {
        matchScore += (fontProp.card32 == 'I'); // code-string for Italic
      }

      if (matchScore == X11_FONT_PERFECT_SCORE)
        break;
    }

    if (matchScore > bestScore) {
      bestMatch = fontName;
      bestScore = matchScore;
    }

    if (matchScore == X11_FONT_PERFECT_SCORE)
      break;
  }

  XFreeFontInfo(fontNames, fontInfos, fontCount);
  XCloseDisplay(display);

  if (bestMatch.empty()) {
    return "";
  } else {
    return bestMatch;
  }
}

}
