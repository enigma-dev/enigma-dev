/*********************************************************************************\
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
\*********************************************************************************/

#include "../OS_Switchboard.h" //Tell us where the hell we are
#include "../backend/EnigmaStruct.h" //LateralGM interface structures

#ifdef _WIN32
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

#include <iostream>
using namespace std;

enum
{
  E_ERROR_NO_ERROR_LOL,
  E_ERROR_PLUGIN_FUCKED_UP,
  E_ERROR_WUT
};

dllexport int gameNew()
{
  cout << "\nI .love. you.\n";
  return 0;
};

dllexport int compileEGMf(EnigmaStruct *es)
{
  cout << "Hey. I wanted you to know: I have no fucking idea what to do with this input. :D\n";
  
  cout << "Location in memory of structure: " << es << endl;
  if (es == NULL)
    return E_ERROR_PLUGIN_FUCKED_UP;
  
  cout << "File version: " << es->fileVersion << endl << endl;
  
  cout << es->spriteCount << " Sprites:" << endl;
  for (int i = 0; i < es->spriteCount; i++) {
    cout << " " << es->sprites[i].name << endl;
    fflush(stdout);
  }
  cout << es->spriteCount << " Sounds:" << endl;
  for (int i = 0; i < es->soundCount; i++) {
    cout << " " << es->sounds[i].name << endl;
    fflush(stdout);
  }
  cout << es->spriteCount << " Scripts:" << endl;
  for (int i = 0; i < es->scriptCount; i++) {
    cout << " " << es->scripts[i].name << endl;
    fflush(stdout);
  }
  cout << es->spriteCount << " Rooms:" << endl;
  for (int i = 0; i < es->roomCount; i++) {
    cout << " " << es->rooms[i].name << endl;
    fflush(stdout);
  }

  cout << es->spriteCount << " Objects:" << endl;
  for (int i = 0; i < es->gmObjectCount; i++) {
    cout << " " << es->gmObjects[i].name;
    int ev = 0;
    for (int j = 0; j < es->gmObjects[i].mainEventCount; j++)
      ev += es->gmObjects[i].mainEvents[j].eventCount;
    cout << " (" << ev << " event(s))" << endl;
    fflush(stdout);
  }
  
  return 0;
};
