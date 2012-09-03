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

#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;
#define flushl '\n' << flush
#define flushs flush

#include "syntax/checkfile.h"
#include "general/parse_basics_old.h"
#include "general/bettersystem.h"

#include "gcc_backend.h"

inline string fc(const char* fn)
{
    FILE *pt = fopen(fn,"rb");
    if (pt==NULL) return "";
    else {
      fseek(pt,0,SEEK_END);
      size_t sz = ftell(pt);
      fseek(pt,0,SEEK_SET);

      char a[sz+1];
      sz = fread(a,1,sz,pt);
      fclose(pt);

      a[sz] = 0;
      return a;
    }
}

#include <sys/time.h>

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

bool init_found_gcc = false;
bool init_load_successful = false;

string MAKE_paths, MAKE_tcpaths, MAKE_location, TOPLEVEL_cflags, TOPLEVEL_cppflags, TOPLEVEL_cxxflags, TOPLEVEL_links, CXX_override, CC_override, WINDRES_location, TOPLEVEL_ldflags;

inline int rdir_system(string x, string y)
{
  return system((x + " " + y).c_str());
}

#include "OS_Switchboard.h"
#include "settings-parse/eyaml.h"

#include "languages/lang_CPP.h"
#include <System/builtins.h>
#include <API/context.h>

// This function parses one command line specified to the eYAML into a filename string and a parameter string,
// then returns whether or not the output from this call must be manually redirected to the output file ofile.
static inline bool toolchain_parseout(string line, string &exename, string &command, string ofile = "")
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
      command.replace(srp,6,"blank.txt");
      srp = command.find("$blank");
      mblank = true;
    }
    if (mblank)
      fclose(fopen("blank.txt","wb"));

  /* Return whether or not to redirect */
  return redir;
}

static char errbuf[1024];
static string lastbearings;

// Read info about our compiler configuration and run with it
const char* establish_bearings(const char *compiler)
{
  if (compiler == lastbearings)
    return 0;
  lastbearings = compiler;
  
  string GCC_location;
  string compfq = compiler; //Filename of compiler.ey
  ifstream compis(compfq.c_str());

  // Bail if error
  if (!compis.is_open())
    return (sprintf(errbuf,"Could not open compiler descriptor `%s`.", compfq.c_str()), errbuf);

  // Parse our compiler data file
  ey_data compey = parse_eyaml(compis,compiler);

  bool got_success = false;

  // Now we begin interfacing with the toolchain.
  string cmd, toolchainexec, parameters; // Full command line, executable part, parameter part
  bool redir; // Whether or not to redirect the output manually

  /* Write down our PATH, etc
  ****************************/
  MAKE_paths = compey.get("path");
  MAKE_tcpaths = compey.get("tcpath");
  TOPLEVEL_cflags = compey.get("cflags");
  TOPLEVEL_cppflags = compey.get("cppflags");
  TOPLEVEL_cxxflags = compey.get("cxxflags");
  TOPLEVEL_links = compey.get("links");
  CXX_override = compey.get("cxx");
  CC_override = compey.get("cc");
  WINDRES_location = compey.get("windres");
  TOPLEVEL_ldflags = compey.get("ldflags");

  /* Get a list of all macros defined by our compiler.
  ** These will help us through parsing available libraries.
  ***********************************************************/
  if ((cmd = compey.get("defines")) == "")
    return (sprintf(errbuf,"Compiler descriptor file `%s` does not specify 'defines' executable.\n", compfq.c_str()), errbuf);
  redir = toolchain_parseout(cmd, toolchainexec,parameters,"defines.txt");
  cout << "Read key `defines` as `" << cmd << "`\nParsed `" << toolchainexec << "` `" << parameters << "`: redirect=" << (redir?"yes":"no") << "\n";
  got_success = !(redir? e_execsp(toolchainexec, parameters, "> defines.txt",MAKE_paths) : e_execsp(toolchainexec, parameters, MAKE_paths));
  if (!got_success) return "Call to 'defines' toolchain executable returned non-zero!\n";
  else cout << "Call succeeded" << endl;

  /* Get a list of all available search directories.
  ** These are where we'll look for headers to parse.
  ****************************************************/
  if ((cmd = compey.get("searchdirs")) == "")
    return (sprintf(errbuf,"Compiler descriptor file `%s` does not specify 'searchdirs' executable.", compfq.c_str()), errbuf);
  redir = toolchain_parseout(cmd, toolchainexec,parameters,"searchdirs.txt");
  cout << "Read key `searchdirs` as `" << cmd << "`\nParsed `" << toolchainexec << "` `" << parameters << "`: redirect=" << (redir?"yes":"no") << "\n";
  got_success = !(redir? e_execsp(toolchainexec, parameters, "&> searchdirs.txt", MAKE_paths) : e_execsp(toolchainexec, parameters, MAKE_paths));
  if (!got_success) return "Call to 'searchdirs' toolchain executable returned non-zero!";
  else cout << "Call succeeded" << endl;

  /* Parse include directories
  ****************************************/
    string idirs = fc("searchdirs.txt");
    if (idirs == "")
      return "Invalid search directories returned. Error 6.";

    pt pos = 0;
    string idirstart = compey.get("searchdirs-start").toString(), idirend = compey.get("searchdirs-end").toString();
    cout << "Searching for directories between \"" << idirstart << "\" and \"" << idirend << "\"" << endl;
    if (idirstart != "")
    {
      pos = idirs.find(idirstart);
      if (pos == string::npos or (pos > 0 and idirs[pos-1] != '\n' and idirs[pos-1] != '\r')) {
        return "Invalid search directories returned. Start search string does not match a line.";
      }
      pos += idirstart.length();
    }
    jdi::builtin->add_search_directory("ENIGMAsystem/SHELL/");

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
    llreader macro_reader("defines.txt");
    if (!macro_reader.is_open())
      return "Call to `defines' toolchain executable returned no data.\n";

    int res = jdi::builtin->parse_C_stream(macro_reader, "defines.txt");
    if (res)
      return "Highly unlikely error: Compiler builtins failed to parse. But stupid things can happen when working with files.";

  /* Note `make` location
  *****************************/

  if ((cmd = compey.get("make")) == "")
    cmd = "make", cout << "WARNING: Compiler descriptor file `" << compfq <<"` does not specify 'make' executable. Using 'make'.\n";
  toolchain_parseout(cmd, toolchainexec,parameters);
  MAKE_location = toolchainexec;
  if (parameters != "")
    cout << "WARNING: Discarding parameters `" << parameters << "` to " << MAKE_location << "." << endl;

  return 0;
}

/*dllexport const char* gccDefinePath(const char* compiler)
{
  return establish_bearings(compiler);
}*/
