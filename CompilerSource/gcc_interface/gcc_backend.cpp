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

string GCC_location, MAKE_location;

inline int rdir_system(string x, string y)
{
  return system((x + " " + y).c_str());
}

extern string GCC_MACRO_STRING;

//Find us the GCC, get info about it and ourself
int establish_bearings()
{
  // Clear a blank file
  fclose(fopen("blank.txt","wb"));
  
  string defs;
  bool got_success = false;
  
  cout << "Probing for GCC..." << endl;
  
  // See if we've been down this road before
  string bin_path = fc("gcc_adhoc.txt");
  
  if (bin_path != "") //We have in fact been down this road before...
  {
    string cm = bin_path + "cpp";
    got_success = !better_system(cm, "-dM -x c++ -E blank.txt", ">", "defines.txt");
    if (!got_success) got_success = !better_system(cm = bin_path + "cpp.exe", "-dM -x c++ -E blank.txt", ">", "defines.txt");
    if (!got_success) cout << "Failed to load GCC from Ad-Hoc location:\n" << bin_path << endl;
    defs = fc("defines.txt");
    if (defs == "") return (cout << "Bailing: Error 3: Defines are empty.\n" , 1);
  }
  if (!got_success)
  {
    puts("Scouring for Make");
    const char *cpath;
    int failing = better_system(cpath = "cpp", "-dM -x c++ -E blank.txt", ">", "defines.txt");
    if (failing) failing = better_system(cpath = "/MinGW/bin/cpp.exe", "-dM -x c++ -E blank.txt", ">", "defines.txt");
    if (failing) failing = better_system(cpath = "\\MinGW\\bin\\cpp.exe", "-dM -x c++ -E blank.txt", ">", "defines.txt");
    if (failing) failing = better_system(cpath = "C:\\MinGW\\bin\\cpp.exe", "-dM -x c++ -E blank.txt", ">", "defines.txt");
    
    if (failing)
      return (cout << "Bailing: Error 1\n" , 1);
    
    defs = fc("defines.txt");
    if (defs == "") return (cout << "Bailing: Error 3: Defines are empty.\n" , 1);
    
    string scpath = cpath;
    size_t sp = scpath.find_last_of("/\\");
    bin_path = sp == string::npos? "" : scpath.erase(sp+1);
    
    cout << "Good news; it should seem I can reach make from `" << MAKE_location << "'\n";
  }
  
  int failing = better_system(MAKE_location = bin_path + "make", "--ver");
  if (failing) failing = better_system(MAKE_location = bin_path + "make.exe", "--ver");
  if (failing) failing = better_system(MAKE_location = bin_path + "mingw32-make.exe", "--ver");
  if (failing) failing = better_system(MAKE_location = bin_path + "mingw64-make.exe", "--ver");
  if (failing)
    return (cout << "Bailing: Error 2\n" , 1);
  
  int gfailing = better_system(GCC_location = bin_path + "gcc", "-dumpversion");
  if (gfailing) gfailing = better_system(GCC_location = bin_path + "gcc.exe", "-dumpversion");
  if (gfailing) return (cout << "Can't find GCC for some reason. Error PI.", 3);
  cout << "GCC located. Path: `" << GCC_location << '\'' << endl;
  
  pt a = parse_cfile(defs);
  if (a != pt(-1)) {
    cout << "Highly unlikely error. But stupid things can happen when working with files.\n\n";
    return (cout << "Bailing: Error 4\n" , 1);
  }
  GCC_MACRO_STRING = defs;
  
  cout << "Successfully loaded GCC definitions\n";
  
  //Read the search dirs
  fclose(fopen("blank.txt","wb"));
  int sdfail = better_system(GCC_location, "-E -x c++ -v blank.txt", "2>", "searchdirs.txt"); //For some reason, the info we want is written to stderr
  if (sdfail) {
    cout << "Failed to read search directories. Error 5.\n";
    return 5;
  }
  
  string idirs = fc("searchdirs.txt");
  if (idirs == "") {
    cout << "Invalid search directories returned. Error 6.\n";
    return 6;
  }
  
  pt pos = idirs.find("#include <...> search starts here:");
  if (pos == string::npos or (pos > 0 and idirs[pos-1] != '\n' and idirs[pos-1] != '\r')) {
    cout << "Invalid search directories returned. Error 7: " << (pos == string::npos?"former":"latter") << ".\n";
    return 7;
  }
  
  pos += 34;
  while (is_useless(idirs[++pos]));
  const pt endpos = idirs.find("End of search list.");
  idirs = idirs.substr(pos,endpos-pos); //Assume the rest of the file is full of these
  
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
  
  cout << include_directory_count << "dirs:\n";
  for (unsigned i = 0; i < include_directory_count; i++)
    cout << '"' << include_directories[i] << '"' << endl;
  
  return 0;
}

dllexport int gccDefinePath(const char* gccPath)
{
  ofstream a("gcc_adhoc.txt");
  if (a.is_open()) {
    a << gccPath << "/bin/";
    a.close();
  }
  return establish_bearings();
}
