/**
  @file  write_resource_names.cpp
  @brief Implements the part that writes resource names and IDs to the engine.
    
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

#include <fstream>
#include <sstream>
#include <cstring>
#include <languages/lang_CPP.h>

#include <makedir.h> // FIXME: This is ludicrous
#include <general/estring.h>

int lang_CPP::compile_write_settings(compile_context &ctex)
{
  ofstream wto;
  GameSettings gameSet = ctex.es->gameSettings;
  wto.open((makedir +"Preprocessor_Environment_Editable/Resources.rc").c_str(),ios_base::out);
    wto << gen_license;
    wto << "#include <windows.h>\n";
    if (gameSet.gameIcon != NULL && strlen(gameSet.gameIcon) > 0) {
      wto << "IDI_MAIN_ICON ICON          \"" << string_replace_all(gameSet.gameIcon,"\\","/")  << "\"\n";
    } else {
      wto << "IDI_MAIN_ICON ICON          \"\"\n";
    }
    wto << "VS_VERSION_INFO VERSIONINFO\n";
    wto << "FILEVERSION " << gameSet.versionMajor << "," << gameSet.versionMinor << "," << gameSet.versionRelease << "," << gameSet.versionBuild << "\n";
    wto << "PRODUCTVERSION " << gameSet.versionMajor << "," << gameSet.versionMinor << "," << gameSet.versionRelease << "," << gameSet.versionBuild << "\n";
    wto << "BEGIN\n" << "BLOCK \"StringFileInfo\"\n" << "BEGIN\n" << "BLOCK \"040904E4\"\n" << "BEGIN\n";
    wto << "VALUE \"CompanyName\",         \"" << gameSet.company << "\"\n";
    wto << "VALUE \"FileDescription\",     \"" << gameSet.description << "\"\n";
    wto << "VALUE \"FileVersion\",         \"" << gameSet.version << "\\0\"\n";
    wto << "VALUE \"ProductName\",         \"" << gameSet.product << "\"\n";
    wto << "VALUE \"ProductVersion\",      \"" << gameSet.version << "\\0\"\n";
    wto << "VALUE \"LegalCopyright\",      \"" << gameSet.copyright << "\"\n";
    if (ctex.es->filename != NULL && strlen(ctex.es->filename) > 0) {
        wto << "VALUE \"OriginalFilename\",         \"" << string_replace_all(ctex.es->filename,"\\","/") << "\"\n";
    } else {
        wto << "VALUE \"OriginalFilename\",         \"\"\n";
    }
    wto << "END\nEND\nBLOCK \"VarFileInfo\"\nBEGIN\n";
    wto << "VALUE \"Translation\", 0x409, 1252\n";
    wto << "END\nEND";
  wto.close();
  
  wto.open((makedir +"Preprocessor_Environment_Editable/GAME_SETTINGS.h").c_str(),ios_base::out);
    wto << lang_CPP::gen_license;
    wto << "#define ASSUMEZERO 0\n";
    wto << "void ABORT_ON_ALL_ERRORS() { " << (false?"game_end();":"") << " }\n";
    wto << '\n';
  wto.close();
   
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h").c_str(),ios_base::out);
    wto << lang_CPP::gen_license;
    wto << "#define BUILDMODE " << 0 << "\n";
    wto << "#define DEBUGMODE " << 0 << "\n";
    wto << '\n';
  wto.close();

  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h").c_str(),ios_base::out);
  wto.close();
}
