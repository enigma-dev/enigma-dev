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
#include <map>

using namespace std;
#define flushl '\n' << flush
#define flushs flush

#include "../externs/externs.h"
    #include "../cfile_parse/cfile_parse.h"
    #include "../syntax/checkfile.h"
    #include "../general/parse_basics.h"
    #include "../general/bettersystem.h"

string fc(const char* fn);

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
varray<string> include_directories;
unsigned int include_directory_count;

string MAKE_location, MAKE_paths;

inline int rdir_system(string x, string y)
{
  return system((x + " " + y).c_str());
}

extern my_string fca(const char*);
extern my_string GCC_MACRO_STRING;
    
#include "../OS_Switchboard.h"
#include "../settings-parse/eyaml.h"
    
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

// Read info about our compiler configuration and run with it
const char* establish_bearings(const char *compiler)
{
  string GCC_location;
  string compfq = "Compilers/" CURRENT_PLATFORM_NAME "/"; compfq += compiler; compfq += ".ey";
  ifstream compis(compfq.c_str());
  
  // Bail if error
  if (!compis.is_open())
    return (sprintf(errbuf,"Could not open compiler descriptor `%s`.", compfq.c_str()), errbuf);
  
  // Parse our compiler data file
  ey_data compey = parse_eyaml(compis,compiler);
  
  my_string defs;
  bool got_success = false;
  
  // Now we begin interfacing with the toolchain.
  string cmd, toolchainexec, parameters; // Full command line, executable part, parameter part
  bool redir; // Whether or not to redirect the output manually
  
  /* Get a list of all macros defined by our compiler.
  ** These will help us through parsing available libraries.
  ***********************************************************/
  if ((cmd = compey.get("defines")) == "")
    return (sprintf(errbuf,"Compiler descriptor file `%s` does not specify 'defines' executable.\n", compfq.c_str()), errbuf);
  redir = toolchain_parseout(cmd, toolchainexec,parameters,"defines.txt");
  cout << "Read key `defines` as `" << cmd << "`\nParsed `" << toolchainexec << "` `" << parameters << "`: redirect=" << (redir?"yes":"no") << "\n";
  got_success = !(redir? better_system(toolchainexec, parameters, ">", "defines.txt") : better_system(toolchainexec, parameters));
  if (!got_success) return "Call to 'defines' toolchain executable returned non-zero!\n";
  else cout << "Call succeeded" << endl;
  
  /* Get a list of all available search directories.
  ** These are where we'll look for headers to parse.
  ****************************************************/
  if ((cmd = compey.get("searchdirs")) == "")
    return (sprintf(errbuf,"Compiler descriptor file `%s` does not specify 'searchdirs' executable.", compfq.c_str()), errbuf);
  redir = toolchain_parseout(cmd, toolchainexec,parameters,"searchdirs.txt");
  cout << "Read key `searchdirs` as `" << cmd << "`\nParsed `" << toolchainexec << "` `" << parameters << "`: redirect=" << (redir?"yes":"no") << "\n";
  got_success = !(redir? better_system(toolchainexec, parameters, "&>", "searchdirs.txt") : better_system(toolchainexec, parameters));
  if (!got_success) return "Call to 'searchdirs' toolchain executable returned non-zero!";
  else cout << "Call succeeded" << endl;
  
  /* Parse include directories
  ****************************************/
    string idirs = fc("searchdirs.txt");
    if (idirs == "")
      return "Invalid search directories returned. Error 6.";
    
    pt pos;
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
    
    while (is_useless(idirs[++pos]));
    
    const pt endpos = (idirend != "")? idirs.find(idirend): string::npos;
    idirs = idirs.substr(pos, endpos-pos); //Assume the rest of the file is full of these
    
    pt spos = 0;
    for (pos = 0; pos < idirs.length(); pos++)
    {
      if (idirs[pos] == '\r' or idirs[pos] == '\n')
      {
        idirs[pos] = '/';
        include_directories[include_directory_count++] = idirs.substr(spos,pos-spos+(idirs[pos-1] != '/'));
        while (is_useless(idirs[++pos]));
        spos = pos--;
      }
    }
    
    cout << "Toolchain returned " << include_directory_count << " search directories:\n";
    for (unsigned i = 0; i < include_directory_count; i++)
      cout << " =>  \"" << include_directories[i] << '"' << endl;
  
  /* Parse built-in #defines
  ****************************/
    defs = fc("defines.txt");
    if (!defs or !*defs) return "Call to toolchain executable returned no data.\n";
    
    pt a = parse_cfile(defs);
    if (a != pt(-1)) {
      return "Highly unlikely error. But stupid things can happen when working with files.";
    }
    GCC_MACRO_STRING = defs;
  
  /* Note `make` location
  *****************************/
  
  if ((cmd = compey.get("make")) == "")
    cmd = "make", cout << "WARNING: Compiler descriptor file `" << compfq <<"` does not specify 'make' executable. Using 'make'.\n";
  toolchain_parseout(cmd, toolchainexec,parameters);
  MAKE_location = toolchainexec;
  if (parameters != "")
    cout << "WARNING: Discarding parameters `" << parameters << "` to " << MAKE_location << "." << endl;
  
  MAKE_paths = compey.get("path");
  
  return 0;
}

/*dllexport const char* gccDefinePath(const char* compiler)
{
  return establish_bearings(compiler);
}*/
