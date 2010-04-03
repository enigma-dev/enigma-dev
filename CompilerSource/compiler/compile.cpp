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

#include "../OS_Switchboard.h" //Tell us where the hell we are
#include "../backend/EnigmaStruct.h" //LateralGM interface structures

#include "../general/darray.h"

#ifdef _WIN32
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif


#include <string>
#include <iostream>
using namespace std;
#define flushl (fflush(stdout), "\n")

#include "../externs/externs.h"
#include "../syntax/syncheck.h"
#include "../parser/parser.h"
#include "compile_includes.h"

dllexport int compileEGMf(EnigmaStruct *es)
{
  cout << "Hey. I wanted you to know: I have no fucking idea what to do with this input. :D\n";
  
  cout << "Location in memory of structure: " << es << flushl;
  if (es == NULL)
    return E_ERROR_PLUGIN_FUCKED_UP;
  
  cout << "File version: " << es->fileVersion << endl << flushl;
  if (es->fileVersion != 600)
    cout << "Error: Incorrect version. File is too " << ((es->fileVersion > 600)?"new":"old") << " for this compiler.";
  
  /**
    Segment One: This segment of the compile process is responsible for
    translating the code into C++. Basically, anything essential to the
    compilation of said code is dealt with during this segment.
  */
  
  ///The segment begins by adding resource names to the collection of variables that should not be automatically re-scoped.  
  
  //First, we make a space to put our globals.
  globals_scope = scope_get_using(&global_scope);
  globals_scope = globals_scope->members["ENIGMA Resources"] = new externs;
    globals_scope->name  = "ENIGMA Resources";
    globals_scope->flags = EXTFLAG_NAMESPACE;
    globals_scope->type  = NULL;
  
  
  
  //Next, add the resource names to that list
  cout << "COPYING SOME FUCKING RESOURCES:" << flushl;
  
  cout << "Copying sprite names [" << es->spriteCount << "]" << flushl;
  for (int i = 0; i < es->spriteCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sprites[i].name);
    
  cout << "Copying sound names [" << es->soundCount << "]" << flushl;
  for (int i = 0; i < es->soundCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sounds[i].name);
    
  cout << "Copying background names [" << es->backgroundCount << "]" << flushl;
  for (int i = 0; i < es->backgroundCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->backgrounds[i].name);
    
  cout << "Copying path names [kidding, these are totally not implemented :P] [" << es->pathCount << "]" << flushl;
  for (int i = 0; i < es->spriteCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sprites[i].name);
    
  cout << "Copying script names [" << es->scriptCount << "]" << flushl;
  for (int i = 0; i < es->scriptCount; i++)
    quickmember_script(globals_scope,es->scripts[i].name);
    
  cout << "Copying font names [kidding, these are totally not implemented :P] [" << es->fontCount << "]" << flushl;
  for (int i = 0; i < es->fontCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->fonts[i].name);
    
  cout << "Copying timeline names [kidding, these are totally not implemented :P] [" << es->timelineCount << "]" << flushl;
  for (int i = 0; i < es->timelineCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->timelines[i].name);
  
  cout << "Copying object names [" << es->gmObjectCount << "]" << flushl;
  for (int i = 0; i < es->gmObjectCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->gmObjects[i].name);
  
  cout << "Copying room names [" << es->roomCount << "]" << flushl;
  for (int i = 0; i < es->roomCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->rooms[i].name);
  
  
  
  ///Next we do a simple pares of the code, scouting for some variable names and adding semicolons.
  cout << "SYNTAX CHECKING AND PRIMARY PARSING" << flushl;
  
  cout << es->scriptCount << " Scripts:" << endl;
  for (int i = 0; i < es->scriptCount; i++)
  {
    int a = syncheck::syntacheck(es->scripts[i].code);
    if (a != -1) {
      cout << "Syntax error in script `" << es->scripts[i].name << "'" << endl << syncheck::error << flushl;
      return E_ERROR_SYNTAX;
    }
    fflush(stdout);
  }

  cout << es->gmObjectCount << " Objects:" << endl;
  for (int i = 0; i < es->gmObjectCount; i++)
  {
    //For every object in Ism's struct, make our own
    unsigned ev_count = 0;
    parsed_object* pob = parsed_objects[es->gmObjects[i].id] = new parsed_object(es->gmObjects[i].name);
    cout << " " << es->gmObjects[i].name << ": " << es->gmObjects[i].mainEventCount << " sub-events: " << flushl;
    
    for (int ii = 0; ii < es->gmObjects[i].mainEventCount; ii++)
      if (es->gmObjects[i].mainEvents[ii].eventCount) //For every MainEvent that contains event code
    {
      //For each main event in that object, make a copy
      const int mev_id = es->gmObjects[i].mainEvents[ii].id;
      cout << "  Event[" << es->gmObjects[i].mainEvents[ii].id << "]: ";
      
      for (int iii = 0; iii < es->gmObjects[i].mainEvents[ii].eventCount; iii++)
      {
        //For each individual event (like begin_step) in the main event (Step), parse the code
        const int sev_id = es->gmObjects[i].mainEvents[ii].events[iii].id;
        parsed_event &pev = pob->events[ev_count++];
        cout << "[" << mev_id << "," << sev_id << "]";
        
        //Copy the code into a string, and its attributes elsewhere
        string code = es->gmObjects[i].mainEvents[ii].events[iii].code;
        
        //Syntax check the code
        int sc = syncheck::syntacheck(code);
        if (sc != -1)
        {
          cout << "Syntax error in object `" << es->gmObjects[i].name << "', event " << mev_id << ":"
               << es->gmObjects[i].mainEvents[ii].events[iii].id << ":\n" << format_error(code,syncheck::error,sc) << flushl;
          return E_ERROR_SYNTAX;
        }
        
        //Add this to our objects map
        pev.myObj = pob; //link to its parent
        parser_main(code,&pev);
      }
    }
    fflush(stdout);
  }
  
  cout << es->roomCount << " Rooms:" << endl;
  for (int i = 0; i < es->roomCount; i++) {
    cout << " " << es->rooms[i].name << endl;
    fflush(stdout);
  }
  
  //Now, time to review
  cout << "Printing all objects and events: " << flushl;
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
  {
    cout << "Object `"<< i->second->name << "':" << flushl;
    for (unsigned ii = 0; ii < i->second->events.size; ii++)
    {
      cout << "Event(" << i->second->events[ii].mainId << "," << i->second->events[ii].id << "):" << flushl;
      cout << i->second->events[ii].code << flushl << flushl << flushl;
    }
  }
  
  
  //Export resources to each file.
  
  
  
  /*
    Segment three: Add resources into the game executable
  */
  
  cout << es->spriteCount << " Sprites:" << endl;
  for (int i = 0; i < es->spriteCount; i++)
  {
    cout << "Sprite " << es->sprites[i].name << endl;
    cout << "Data at: " << es->sprites[i].subImages[0].pixels << endl << endl;
    //for (int ii = 0; ii <  es->sprites[i].subImages[0].width *  es->sprites[i].subImages[0].height; ii++) {
    //  cout << (void*)(es->sprites[i].subImages[0].pixels[ii]);
    //}
    fflush(stdout);
  }
  
  cout << es->soundCount << " Sounds:" << endl;
  for (int i = 0; i < es->soundCount; i++) {
    cout << " " << es->sounds[i].name << endl;
    fflush(stdout);
  }
  
  
  return 0;
};
