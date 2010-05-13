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
    #include "../general/parse_basics.h"

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
  string bin_path = fc("gcc_adhoc.txt");
  
  if (bin_path != "") //We have in fact been down this road before...
  {
    string cm = "\"" + bin_path + "cpp\" -dM -x c++ -E  blank.txt > defines.txt";
    got_success = !system(cm.c_str());
    if (!got_success)
      cout << "Failed to load GCC from Ad-Hoc location:\n" << bin_path << endl;
    else
    {
      GCC_location = "\"" + bin_path + "gcc\"";
      MAKE_location = "\"" + bin_path + "make\"";
    }
  }
  if (!got_success)
  {
    got_success = !system(((bin_path = "") + "cpp -dM -x c++ -E  blank.txt > defines.txt").c_str());
  
    if (!got_success)
      got_success = !system(((bin_path = "/MinGW/bin/") + "cpp -dM -x c++ -E blank.txt > defines.txt").c_str());
    
    if (!got_success)
      got_success = !system(((bin_path = "C:/MinGW/bin/") + "cpp -dM -x c++ -E blank.txt > defines.txt").c_str());
    
    if (!got_success)
      got_success = !system(((bin_path = "./ENIGMAsystem/MinGW/bin/") + "cpp -dM -x c++ -E blank.txt > defines.txt").c_str());
      
    if (!got_success)
      got_success = !system(((bin_path = "./ENIGMAsystem/bin/") + "cpp -dM -x c++ -E blank.txt > defines.txt").c_str());
    
    if (!got_success)
      return (cout << "Bailing: Error 1\n" , 1);
    
    GCC_location = bin_path + "gcc";
    MAKE_location = bin_path + "make";
  }
  
  cout << "GCC located. Path: `" << bin_path << '\'' << endl;
  
  defs = fc("defines.txt");
  if (defs == "")
    return (cout << "Bailing: Error 2\n" , 1);
  
  pt a = parse_cfile(defs);
  if (a != pt(-1)) {
    cout << "Highly unlikely error. But stupid things can happen when working with files.\n\n";
    return (cout << "Bailing: Error 3\n" , 1);
  }
  
  cout << "Successfully loaded GCC definitions\n";
  cout << "Undefining _GLIBCXX_EXPORT_TEMPLATE\n";
  macros["_GLIBCXX_EXPORT_TEMPLATE"] = "0"; //Save us some work
  
  //Read the search dirs
  fclose(fopen("blank.txt","wb"));
  got_success = !system((GCC_location + " -E -x c++ -v blank.txt 2> searchdirs.txt").c_str()); //For some reason, the info we want is written to stderr
  if (!got_success) {
    cout << "Failed to read search directories. Error 4.\n";
    return 4;
  }
  
  string idirs = fc("searchdirs.txt");
  if (idirs == "") {
    cout << "Invalid search directories returned. Error 5.\n";
    return 5;
  }
  
  pt pos = idirs.find("#include <...> search starts here:");
  if (pos == string::npos or (pos > 0 and idirs[pos-1] != '\n' and idirs[pos-1] != '\r')) {
    cout << "Invalid search directories returned. Error 5: " << (pos == string::npos?"former":"latter") << ".\n";
    return 5;
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
  
  
  //Now we'll look for make
  cout << "All that worked. Trying to find make.\n";
  
  int askmake = 1;
  //At present, MAKE_location = "\"" + bin_path + "make\"";
  askmake = system(MAKE_location.c_str());
  if (askmake) //Didn't answer us right; let's try someone else
  {
    askmake = system((MAKE_location = "\"" + bin_path + "mingw32-make\"").c_str());
    if (askmake)
    {
      cout << "Sir, I can't find make.\n";
      return 6;
    }
  }
  
  cout << "Good news; it should seem I can reach make from " << MAKE_location << "\n";
  
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
