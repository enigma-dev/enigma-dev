/** Copyright (C) 2008 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/


#include "OS_Switchboard.h"
#include "settings.h"
#include "gcc_backend.h"
#include "settings.h"

#include "general/parse_basics_old.h"
#include "general/bettersystem.h"

#include "languages/lang_CPP.h"

#include "System/builtins.h"

#include "API/context.h"

#include <time.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

#ifdef _WIN32
  #define byte __windows_byte_workaround
  #include <windows.h>
  #undef byte
 #define DLLEXPORT extern "C" __declspec(dllexport)
#else
 #define DLLEXPORT extern "C"
 #include <cstdio>
#endif

using namespace std;
#define flushl '\n' << flush
#define flushs flush

bool init_found_gcc = false;
bool init_load_successful = false;

static char errbuf[1024];
static string lastbearings;
static std::filesystem::path lastcodegen_directory;

// Read info about our compiler configuration and run with it
const char* establish_bearings(const char *compiler)
{
  if (compiler == lastbearings && codegen_directory == lastcodegen_directory)
    return 0;

  if (!load_compiler_ey(compiler))
    return (sprintf(errbuf, "Could not open compiler descriptor `%s`.", compiler), errbuf);
  
  lastbearings = compiler;
  lastcodegen_directory = codegen_directory;
  
  string GCC_location;

  bool got_success = false;

  std::string MAKE_paths = compilerInfo.make_vars["PATH"];
  
  std::string dirs = "CODEGEN=" + unixfy_path(codegen_directory) + " ";
  dirs += "WORKDIR=" + unixfy_path(eobjs_directory) + " ";
  actually_bash("make " + dirs + " required-directories");

  /* Get a list of all macros defined by our compiler.
  ** These will help us through parsing available libraries.
  ***********************************************************/
  cout << "Read key `defines` as `" << compilerInfo.defines_cmd << "`" << std::endl;
  got_success = actually_bash(compilerInfo.defines_cmd + " > " + (codegen_directory/"enigma_defines.txt").u8string());
  if (got_success != 0) return "Call to 'defines' toolchain executable returned non-zero!\n";
  else cout << "Call succeeded" << endl;

  /* Get a list of all available search directories.
  ** These are where we'll look for headers to parse.
  ****************************************************/
  cout << "Read key `searchdirs` as `" << compilerInfo.searchdirs_cmd << "`" << std::endl;
  got_success = actually_bash(compilerInfo.searchdirs_cmd + " &> " + (codegen_directory/"enigma_searchdirs.txt").u8string());
  if (got_success != 0) return "Call to 'searchdirs' toolchain executable returned non-zero!";
  else cout << "Call succeeded" << endl;

  /* Parse include directories
  ****************************************/
    string idirs = fc((codegen_directory/"enigma_searchdirs.txt").u8string().c_str());
    if (idirs == "")
      return "Invalid search directories returned. Error 6.";

    pt pos = 0;
    string idirstart = compilerInfo.searchdirs_start, idirend = compilerInfo.searchdirs_end;
    cout << "Searching for directories between \"" << idirstart << "\" and \"" << idirend << "\"" << endl;
    if (idirstart != "")
    {
      pos = idirs.find(idirstart);
      if (pos == string::npos or (pos > 0 and idirs[pos-1] != '\n' and idirs[pos-1] != '\r')) {
        return "Invalid search directories returned. Start search string does not match a line.";
      }
      pos += idirstart.length();
    }
  
    jdi::builtin->add_search_directory((enigma_root/"ENIGMAsystem/SHELL").u8string().c_str());
    jdi::builtin->add_search_directory((enigma_root/"shared").u8string().c_str());
    jdi::builtin->add_search_directory(codegen_directory.u8string().c_str());

    while (is_useless(idirs[++pos]));

    const pt endpos = (idirend != "")? idirs.find(idirend): string::npos;
    idirs = idirs.substr(pos, endpos-pos); //Assume the rest of the file is full of these

    pt spos = 0;
    for (pos = 0; pos < idirs.length(); pos++)
    {
      if (idirs[pos] == '\r' or idirs[pos] == '\n')
      {
        idirs[pos] = '/';
        jdi::builtin->add_search_directory(idirs.substr(spos,pos-spos+(idirs[pos-1] != '/')));
        while (is_useless(idirs[++pos]));
        spos = pos--;
      }
    }

    cout << "Toolchain returned " << jdi::builtin->search_dir_count() << " search directories:\n";

  /* Parse built-in #defines
  ****************************/
    llreader macro_reader((codegen_directory/"enigma_defines.txt").u8string().c_str());
    if (!macro_reader.is_open())
      return "Call to `defines' toolchain executable returned no data.\n";

    int res = jdi::builtin->parse_C_stream(macro_reader, (codegen_directory/"enigma_defines.txt").u8string().c_str());
    jdi::builtin->add_macro("_GLIBCXX_USE_CXX11_ABI", "0");
    if (res)
      return "Highly unlikely error: Compiler builtins failed to parse. But stupid things can happen when working with files.";

  return 0;
}

/*DLLEXPORT const char* gccDefinePath(const char* compiler)
{
  return establish_bearings(compiler);
}*/
