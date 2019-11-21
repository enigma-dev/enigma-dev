/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "makedir.h"

#include "backend/GameData.h"
#include "compiler/compile_common.h"
#include "languages/lang_CPP.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int lang_CPP::compile_writeFontInfo(const GameData &game)
{
  ofstream wto((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_fontinfo.h").c_str(),ios_base::out);
  wto << license
      << "#ifndef JUST_DEFINE_IT_RUN" << endl
      << "#undef INCLUDED_FROM_SHELLMAIN" << endl
      << "#endif" << endl
      << "#include \"Universal_System/Resources/fonts_internal.h\"" << endl
      << endl;

  int maxid = -1, rawfontcount = 0;
  wto << "namespace enigma {" << endl;
  wto << "  std::vector<rawfont> rawfontdata = {" << endl;
  for (const auto &font : game.fonts) {
    wto << "    {\""
        << font.name        << "\", "     // string name;
        << font->id()        << ", \""    // int id;
        << font->font_name() << "\", "    // string fontName;
        << font->size()      << ", "      // int size;
        << font->bold()      << ", "      // bool bold;
        << font->italic()    << ", "      // bool italic;
        << font.normalized_ranges.size()  // int glyphRangeCount;
        << "}," << endl;

    if (font.id() > maxid)
      maxid = font.id();
    rawfontcount++;
  }
  wto << "  };" << endl;
  wto << endl << "  int rawfontcount = " << rawfontcount << ", rawfontmaxid = " << maxid << ";" << endl;
  wto << "}" << endl;
  wto.close();
  return 0;
}
