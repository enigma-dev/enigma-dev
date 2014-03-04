/**
  @file  clear_cpp_editables.cpp
  @brief Defines a function to clean up exported definitions from previous builds.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
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
#include <languages/lang_CPP.h>
#include <settings-parse/crawler.h>
#include <settings-parse/eyaml.h>
#include <makedir.h> // FIXME: This is ludicrous

inline string fc(const char* fn)
{
  FILE *ptf = fopen(fn,"rb");
  if (!ptf) return "";
  
  fseek(ptf,0,SEEK_END);
  size_t sz = ftell(ptf);
  fseek(ptf,0,SEEK_SET);

  char *a = (char*)alloca(sz+1);
  sz = fread(a,1,sz,ptf);
  fclose(ptf);

  a[sz] = 0;
  return a;
}

void lang_CPP::clear_ide_editables()
{
  ofstream wto;
  string f2comp = fc((makedir + "API_Switchboard.h").c_str());
  string f2write = gen_license;
    string inc = "/include.h\"\n";
    f2write += "#include \"Platforms/" + (extensions::targetAPI.windowSys)            + "/include.h\"\n"
               "#include \"Graphics_Systems/" + (extensions::targetAPI.graphicsSys)   + "/include.h\"\n"
               "#include \"Audio_Systems/" + (extensions::targetAPI.audioSys)         + "/include.h\"\n"
               "#include \"Collision_Systems/" + (extensions::targetAPI.collisionSys) + "/include.h\"\n"
               "#include \"Networking_Systems/" + (extensions::targetAPI.networkSys) + "/include.h\"\n"
               "#include \"Widget_Systems/" + (extensions::targetAPI.widgetSys)       + inc;

    const string incg = "#include \"", impl = "/implement.h\"\n";
    f2write += "\n// Extensions selected by user\n";
    for (unsigned i = 0; i < parsed_extensions.size(); i++)
    {
      ifstream ifabout((parsed_extensions[i].pathname + "/About.ey").c_str());
      ey_data about = parse_eyaml(ifabout,parsed_extensions[i].path + parsed_extensions[i].name + "/About.ey");
      f2write += incg + parsed_extensions[i].pathname + inc;
      if (parsed_extensions[i].implements != "")
        f2write += incg + parsed_extensions[i].pathname + impl;
    }

  if (f2comp != f2write)
  {
    wto.open((makedir +"API_Switchboard.h").c_str(),ios_base::out);
      wto << f2write << endl;
    wto.close();
  }

  wto.open((makedir +"Preprocessor_Environment_Editable/LIBINCLUDE.h").c_str());
    wto << gen_license;
    wto << "/*************************************************************\nOptionally included libraries\n****************************/\n";
    wto << "#define STRINGLIB 1\n#define COLORSLIB 1\n#define STDRAWLIB 1\n#define PRIMTVLIB 1\n#define WINDOWLIB 1\n"
           "#define STDDRWLIB 1\n#define GMSURFACE 0\n#define BLENDMODE 1\n";
    wto << "/***************\nEnd optional libs\n ***************/\n";
  wto.close();

  wto.open((makedir +"Preprocessor_Environment_Editable/GAME_SETTINGS.h").c_str(),ios_base::out);
    wto << gen_license;
    wto << "#define ASSUMEZERO 0\n";
    wto << "#define PRIMBUFFER 0\n";
    wto << "#define PRIMDEPTH2 6\n";
    wto << "#define AUTOLOCALS 0\n";
    wto << "#define MODE3DVARS 0\n";
    wto << "void ABORT_ON_ALL_ERRORS() { }\n";
    wto << '\n';
  wto.close();
}
