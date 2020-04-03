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
#include "makedir.h"
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
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

using namespace std;
#define flushl '\n' << flush
#define flushs flush

bool init_found_gcc = false;
bool init_load_successful = false;

static char errbuf[1024];
static string lastbearings;
static string lastcodegen_directory;

// This function parses one command line specified to the eYAML into a filename string and a parameter string,
// then returns whether or not the output from this call must be manually redirected to the output file ofile.
static bool toolchain_parseout(string line, string &exename, string &command, string ofile = "")
{ 
  pt pos = 0, spos;

  /* Isolate the executable path and filename
  ***********************************************/
    while (is_useless(line[pos]) and pos<line.length()) pos++; // Skip leading whitespace
    if (pos == line.length()) return false;
    spos = pos;

    if (line[pos] == '"' and ++spos)
      while (line[++pos] != '"' and pos<line.length())
        if (line[pos] == '\\') pos++; else;
    else if (line[pos] == '\'' and ++spos)
      while (line[++pos] != '\'' and pos<line.length())
        if (line[pos] == '\\') pos++; else;
    else while (!is_useless(line[++pos]) and pos<line.length());

  exename = line.substr(spos,pos-spos);
  if (pos >= line.length())
    return (command = "", true);

  /* Isolate the command part of our input line
  **********************************************/
  while (is_useless(line[++pos]));
  command = line.substr(pos);

  /* Parse the command for keywords such as $out and $blank
  ************************************************************/
    size_t srp = command.find("$out");

    bool redir = true;
    while (srp != string::npos) {
      redir = false;
      command.replace(srp,4,ofile);
      srp = command.find("$out");
    }

    bool mblank = false;
    srp = command.find("$blank");
    while (srp != string::npos) {
      command.replace(srp,6,("\"" + codegen_directory + "enigma_blank.txt\"").c_str());
      srp = command.find("$blank");
      mblank = true;
    }
    if (mblank)
      fclose(fopen((codegen_directory + "enigma_blank.txt").c_str(),"wb"));

  /* Return whether or not to redirect */
  return redir;
}

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

  // Now we begin interfacing with the toolchain.
  string cmd, toolchainexec, parameters; // Full command line, executable part, parameter part
  bool redir; // Whether or not to redirect the output manually

  std::string MAKE_paths = compilerInfo.make_vars["PATH"];
  
  std::string dirs = "CODEGEN=" + codegen_directory + " ";
  dirs += "WORKDIR=" + eobjs_directory + " ";
  e_execs("make", dirs, "required-directories");

  /* Get a list of all macros defined by our compiler.
  ** These will help us through parsing available libraries.
  ***********************************************************/
  cmd = compilerInfo.defines_cmd;
  redir = toolchain_parseout(cmd, toolchainexec,parameters,("\"" + codegen_directory + "enigma_defines.txt\""));
  cout << "Read key `defines` as `" << cmd << "`\nParsed `" << toolchainexec << "` `" << parameters << "`: redirect=" << (redir?"yes":"no") << "\n";
  got_success = !(redir? e_execsp(toolchainexec, parameters, ("> \"" + codegen_directory + "enigma_defines.txt\""),MAKE_paths) : e_execsp(toolchainexec, parameters, MAKE_paths));
  if (!got_success) return "Call to 'defines' toolchain executable returned non-zero!\n";
  else cout << "Call succeeded" << endl;

  /* Get a list of all available search directories.
  ** These are where we'll look for headers to parse.
  ****************************************************/
  cmd = compilerInfo.searchdirs_cmd;
  redir = toolchain_parseout(cmd, toolchainexec,parameters,("\"" + codegen_directory + "enigma_searchdirs.txt\""));
  cout << "Read key `searchdirs` as `" << cmd << "`\nParsed `" << toolchainexec << "` `" << parameters << "`: redirect=" << (redir?"yes":"no") << "\n";
  got_success = !(redir? e_execsp(toolchainexec, parameters, ("&> \"" + codegen_directory + "enigma_searchdirs.txt\""), MAKE_paths) : e_execsp(toolchainexec, parameters, MAKE_paths));
  if (!got_success) return "Call to 'searchdirs' toolchain executable returned non-zero!";
  else cout << "Call succeeded" << endl;

  /* Parse include directories
  ****************************************/
    string idirs = fc((codegen_directory + "enigma_searchdirs.txt").c_str());
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
    jdi::builtin->add_search_directory("/home/greg/enigma-dev/ENIGMAsystem/SHELL");
    jdi::builtin->add_search_directory(codegen_directory.c_str());

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
    llreader macro_reader((codegen_directory + "enigma_defines.txt").c_str());
    if (!macro_reader.is_open())
      return "Call to `defines' toolchain executable returned no data.\n";

    int res = jdi::builtin->parse_C_stream(macro_reader, (codegen_directory + "enigma_defines.txt").c_str());
    jdi::builtin->add_macro("_GLIBCXX_USE_CXX11_ABI", "0");
    if (res)
      return "Highly unlikely error: Compiler builtins failed to parse. But stupid things can happen when working with files.";

  /* Note `make` location
  *****************************/

  if (compilerInfo.make_vars.find("MAKE") != compilerInfo.make_vars.end())
    cmd = compilerInfo.make_vars["MAKE"];
  else
    cmd = "make", cout << "WARNING: Compiler descriptor file `" << compiler <<"` does not specify 'make' executable. Using 'make'.\n";
  toolchain_parseout(cmd, toolchainexec,parameters);
  compilerInfo.MAKE_location = toolchainexec;
  if (parameters != "")
    cout << "WARNING: Discarding parameters `" << parameters << "` to " << compilerInfo.MAKE_location << "." << endl;

  return 0;
}

/*dllexport const char* gccDefinePath(const char* compiler)
{
  return establish_bearings(compiler);
}*/
