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
#include <cstdio>
#include <fstream>
#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/reshandlers/refont.h"
#include <string>
using namespace std;
#include "compiler/compile_common.h"


#include "languages/lang_CPP.h"
int lang_CPP::compile_writeFontInfo(EnigmaStruct* es)
{
  ofstream wto((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_fontinfo.h").c_str(),ios_base::out);
  wto << license << "#include \"Universal_System/fonts_internal.h\"" << endl
      << endl;

  int maxid = -1, rawfontcount = 0;
  wto << "namespace enigma {" << endl;
    wto << "  rawfont rawfontdata[] = {" << endl;
    for (int i = 0; i < es->fontCount; i++)
    {
      wto << "    {\"" <<
      es->fonts[i].name   << "\", " <<   // string name;
      es->fonts[i].id     << ", \"" <<   // int id;

      es->fonts[i].fontName << "\", " << // string fontName;
      es->fonts[i].size     <<   ", " << // int size;
      bool(es->fonts[i].bold)     <<   ", " << // bool bold;
      bool(es->fonts[i].italic)   <<   ", " << // bool italic;
      es->fonts[i].glyphRangeCount // int glyphRangeCount;
      << "}," << endl;

      if (es->fonts[i].id > maxid)
        maxid = es->fonts[i].id;
      rawfontcount++;
    }
    wto << "  };" << endl;
    wto << endl << "  int rawfontcount = " << rawfontcount << ", rawfontmaxid = " << maxid << ";" << endl;
	wto << "}" << endl;
	wto.close();
	return 0;
}
