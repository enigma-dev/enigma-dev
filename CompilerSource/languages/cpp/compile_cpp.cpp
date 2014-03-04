/**
  @file  compile_cpp.cpp
  @brief Implements the piece of the C++ plugin that invokes make to build the engine.
  
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

#include <languages/lang_CPP.h>
#include <general/bettersystem.h>
#include <settings-parse/crawler.h>
#include <backend/JavaCallbacks.h>
#include <backend/ideprint.h>
#include <OS_Switchboard.h>
#include <cstdio>

#include <makedir.h> // FIXME: This is ludicrous

language_adapter::resource_writer *lang_CPP::compile(compile_context &ctex, const char* exe_filename)
{
  string desstr = "./ENIGMAsystem/SHELL/design_game" + extensions::targetOS.buildext;
  string gameFname = ctex.mode == emode_design ? desstr.c_str() : (desstr = exe_filename, exe_filename); // We will be using this first to write, then to run

  string make = "Game ";

  make += "WORKDIR=\"" + makedir + "\" ";
  make += ctex.mode == emode_debug? "GMODE=Debug ": ctex.mode == emode_design? "GMODE=Design ": ctex.mode == emode_compile? "GMODE=Compile ": "GMODE=Run ";
  make += "GRAPHICS=" + extensions::targetAPI.graphicsSys + " ";
  make += "AUDIO=" + extensions::targetAPI.audioSys + " ";
  make += "COLLISION=" + extensions::targetAPI.collisionSys + " ";
  make += "WIDGETS="  + extensions::targetAPI.widgetSys + " ";
  make += "NETWORKING="  + extensions::targetAPI.networkSys + " ";
  make += "PLATFORM=" + extensions::targetAPI.windowSys + " ";

  if (CXX_override.length()) make += "CXX=" + CXX_override + " ";
  if (CC_override.length()) make += "CC=" + CC_override + " ";
  if (WINDRES_location.length()) make += "WINDRES=" + WINDRES_location + " ";

  if (ctex.mode != emode_debug) {
    if (TOPLEVEL_cflags.length()) make += "CFLAGS=\"" + TOPLEVEL_cflags + "\" ";
    if (TOPLEVEL_cppflags.length()) make += "CPPFLAGS=\"" + TOPLEVEL_cppflags + "\" ";
    if (TOPLEVEL_cxxflags.length()) make += "CXXFLAGS=\"" + TOPLEVEL_cxxflags + "\" ";
    if (TOPLEVEL_ldflags.length()) make += "LDFLAGS=\"" + TOPLEVEL_ldflags + "\" ";
  }
  else {
    if (TOPLEVEL_cflags.length())   make += "CFLAGS=\"" + TOPLEVEL_cflags + " -g -DDEBUG_MODE\" ";
    if (TOPLEVEL_cppflags.length()) make += "CPPFLAGS=\"" + TOPLEVEL_cppflags + "\" ";
    if (TOPLEVEL_cxxflags.length()) make += "CXXFLAGS=\"" + TOPLEVEL_cxxflags + " -g -DDEBUG_MODE\" ";
    if (TOPLEVEL_ldflags.length())  make += "LDFLAGS=\"" + TOPLEVEL_ldflags + "\" ";
  }
  
  string compilepath = CURRENT_PLATFORM_NAME "/" + extensions::targetOS.identifier;
  make += "COMPILEPATH=\"" + compilepath + "\" ";

  string extstr = "EXTENSIONS=\"";
  for (unsigned i = 0; i < parsed_extensions.size(); i++)
      extstr += " " + parsed_extensions[i].pathname;
  make += extstr + "\"";

  string mfgfn = gameFname;
  for (size_t i = 0; i < mfgfn.length(); i++)
    if (mfgfn[i] == '\\') mfgfn[i] = '/';
  make += string(" OUTPUTNAME=\"") + mfgfn + "\" ";
  make += "eTCpath=\"" + MAKE_tcpaths + "\"";

  edbg << "Running make from `" << MAKE_location << "'" << flushl;
  edbg << "Full command line: " << MAKE_location << " " << make << flushl;
  
  // Pick a file and flush it
  const string redirfile = (makedir + "enigma_compile.log");
  fclose(fopen(redirfile.c_str(),"wb"));

  // Redirect it
  ide_output_redirect_file(redirfile.c_str()); //TODO: If you pass this function the address it will screw up the value; most likely a JNA/Plugin bug.
  int makeres = e_execs(MAKE_location,make,"&> \"" + redirfile + "\"");

  // Stop redirecting GCC output
  ide_output_redirect_reset();

  if (makeres) {
    build_error = "C++ MAKE failed: Code not generated correctly.";
    return NULL;
  }
  
  user << "+++++Make completed successfully.++++++++++++++++++++++++++++++++++++\n";
  
  /* Return our resource writer, linked to the game module we just built.
  ** The writer will handle resource exportation to the executable.
  */
  
  resource_writer_cpp *resw = NULL;
  if (extensions::targetOS.resfile == "$exe")
  {
    resw = new resource_writer_cpp(fopen(gameFname.c_str(),"ab"), gameFname);
    if (!resw->gameModule) {
      user << "Failed to append resources to the game. Did compile actually succeed?" << flushl;
      build_error = "Failed to add resources."; return NULL;
    }

    fseek(resw->gameModule,0,SEEK_END); //necessary on Windows for no reason.
    resw->resourceblock_start = ftell(resw->gameModule);

    if (resw->resourceblock_start < 128) {
      user << "Compiled game is clearly not a working module; cannot continue" << flushl;
      build_error = "Failed to add resources."; return NULL;
    }
  }
  else
  {
    string resname = extensions::targetOS.resfile;
    for (size_t p = resname.find("$exe"); p != string::npos; p = resname.find("$game"))
      resname.replace(p,4,gameFname);
    resw->gameModule = fopen(resname.c_str(),"wb");
    if (!resw->gameModule) {
      user << "Failed to write resources to compiler-specified file, `" << resname << "`. Writeable valid path?" << flushl;
      build_error = "Failed to write resources."; return NULL;
    }
  }

  // Start by setting off our location with a DWord of NULLs
  fwrite("\0\0\0",1,4,resw->gameModule);
  return resw;
}
