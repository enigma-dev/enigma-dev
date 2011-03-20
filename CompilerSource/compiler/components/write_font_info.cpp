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

#include <cstdio>
#include <fstream>
#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../reshandlers/refont.h"
#include <string>
using namespace std;
#include "../compile_common.h"


int compile_writeFontInfo(EnigmaStruct* es)
{
  ofstream wto("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h",ios_base::out);
  wto << license << "namespace enigma {" << endl;
    wto << "  fontstruct baseFonts[] = {" << endl;
    for (int i = 0; i < es->fontCount; i++)
    {
      wto << "    {" <<
      es->fonts[i].name     << ", " << //  string name;
      es->fonts[i].id       << ", " << //  int id;
      
      es->fonts[i].fontName << ", " << //  string fontName;
      es->fonts[i].size     << ", " << //  int size;
      es->fonts[i].bold     << ", " << //  bool bold;
      es->fonts[i].italic   << ", " << //  bool italic;
      es->fonts[i].rangeMin << ", " << //  int rangeMin;
      es->fonts[i].rangeMax << "}," << //  int rangeMax;
      
      endl;
    }
    wto << "  }" << endl;
	wto << "}" << endl;
	wto.close();
	return 0;
}
