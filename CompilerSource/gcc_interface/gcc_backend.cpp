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
#define flushl (fflush(stdout), "\n")

#include "../externs/externs.h"
    #include "../cfile_parse/cfile_parse.h"
    #include "../syntax/checkfile.h"

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

//Find us the GCC, get info about it and ourself
int establish_bearings()
{
  // Clear some files
  fclose(fopen("blank.txt","wb"));
  fclose(fopen("defines.txt","wb"));
  fclose(fopen("searchdirs.txt","wb"));
  
  string defs;
  fclose(fopen("defines.txt","wb"));
  bool got_success = false;
  
  cout << "Probing for GCC..." << endl;
  
  // See if we've been down this road before
  string GCC_location = fc("gcc_adhoc.txt");
  
  if (GCC_location != "") //We have in fact never been down this road before...
    got_success = !system((GCC_location + "cpp -dM -x c++ -E  blank.txt > defines.txt").c_str());
  
  if (!got_success)
    got_success = !system(((GCC_location = "") + "cpp -dM -x c++ -E  blank.txt > defines.txt").c_str());
  
  if (!got_success)
    got_success = !system(((GCC_location = "C:/MinGW/bin/") + "cpp -dM -x c++ -E blank.txt > defines.txt").c_str());
  
  if (!got_success)
    got_success = !system(((GCC_location = "./ENIGMAsystem/MinGW/bin/") + "cpp -dM -x c++ -E blank.txt > defines.txt").c_str());
    
  if (!got_success)
    got_success = !system(((GCC_location = "./ENIGMAsystem/bin/") + "cpp -dM -x c++ -E blank.txt > defines.txt").c_str());
  
  if (!got_success)
    return (cout << "Bailing: Error 1\n" , 1);
  
  cout << "GCC located. Path: `" << GCC_location << '\'' << endl;
  
  defs = fc("defines.txt");
  if (defs == "")
    return (cout << "Bailing: Error 2\n" , 1);
  
  unsigned a = parse_cfile(defs);
  if (a != unsigned(-1)) {
    cout << "Highly unlikely error. But stupid things can happen when working with files.\n\n";
    return (cout << "Bailing: Error 3\n" , 1);
  }
  
  cout << "Successfully loaded GCC definitions\n";
  cout << "Undefining _GLIBCXX_EXPORT_TEMPLATE\n";
  macros["_GLIBCXX_EXPORT_TEMPLATE"] = "0"; //Save us some work
  
  //Read the search dirs
  got_success = system((GCC_location + "gcc -print-search-dirs > searchdirs.txt").c_str());
  if (!got_success) {
    cout << "Failed to read search directories. Error 4.\n";
    return 4;
  }
  
  string idirs = fc("searchdirs.txt");
  if (idirs == "") {
    cout << "Invalid search directories returned. Error 5.\n";
    return 5;
  }
  
  size_t pos;
  for (pos = 0; (pos < idirs.length() - 10) and (idirs.substr(pos,10) != "libraries:"); pos = idirs.find("\n",pos));
  if (idirs.substr(pos,10) != "libraries:") {
    cout << "Invalid search directories returned. Error 5.\n";
    return 5;
  }
  
  pos += 10;
  while (pos < idirs.length() and idirs[++pos] != '=');
  if (idirs[pos] != '=') {
    cout << "Invalid search directories returned. Error 5.\n";
    return 5;
  }
  
  size_t endpos = idirs.find("\n",pos);
  if (endpos != string::npos)
    idirs = idirs.substr(pos, endpos-pos);
  else
    idirs.erase(0,pos);
  
  size_t spos = 0;
  for (pos = 0; pos < idirs.length(); pos++)
  {
    if (idirs[pos] == ';')
    {
      include_directories[include_directory_count++] = idirs.substr(spos,pos-spos);
      spos = pos+1;
    }
  }
  include_directories[include_directory_count++] = idirs.substr(spos);
  
  return 0;
}

dllexport int gccDefinePath(const char* gccPath)
{
  ofstream a("gcc_adhoc.txt");
  if (a.is_open()) {
    a << gccPath;
    a.close();
  }
  return establish_bearings();
}
