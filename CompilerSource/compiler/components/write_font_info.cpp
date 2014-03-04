/**
  @file  write_font_info.cpp
  @brief Implements the method in the C++ plugin that writes information regarding
         font glyphs and metrics to the engine file.
  
  @section License
    Copyright (C) 2008-2014 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <cstdio>
#include <fstream>
#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/reshandlers/refont.h"
#include <string>
using namespace std;
#include "compiler/compile_common.h"


#include "languages/lang_CPP.h"
int lang_CPP::compile_write_font_info(compile_context &ctex)
{
  ofstream wto("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_fontinfo.h",ios_base::out);
  wto << gen_license << "#include \"Universal_System/fontstruct.h\"" << endl
      << endl;
  
  int maxid = -1, rawfontcount = 0;
  wto << "namespace enigma {" << endl;
    wto << "  rawfont rawfontdata[] = {" << endl;
    for (int i = 0; i < ctex.es->fontCount; i++)
    {
      wto << "    {\"" <<
      ctex.es->fonts[i].name   << "\", " <<   // string name;
      ctex.es->fonts[i].id     << ", \"" <<   // int id;
      
      ctex.es->fonts[i].fontName << "\", " << // string fontName;
      ctex.es->fonts[i].size     <<   ", " << // int size;
(bool)ctex.es->fonts[i].bold     <<   ", " << // bool bold;
(bool)ctex.es->fonts[i].italic   <<   ", " << // bool italic;
      ctex.es->fonts[i].rangeMin <<   ", " << // int rangeMin;
      ctex.es->fonts[i].rangeMax - ctex.es->fonts[i].rangeMin + 1 //  int rangeMax;
      << "}," << endl;
      
      if (ctex.es->fonts[i].id > maxid)
        maxid = ctex.es->fonts[i].id;
      rawfontcount++;
    }
    wto << "  };" << endl;
    wto << endl << "  int rawfontcount = " << rawfontcount << ", rawfontmaxid = " << maxid << ";" << endl;
	wto << "}" << endl;
	wto.close();
	return 0;
}
