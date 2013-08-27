/**
  @file  language_adapter.h
  @brief Defines a class by which ENIGMA can interface with and output to other languages.
  
  @section License
    Copyright (C) 2008-2012 Josh Ventura
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

#ifndef _LANGUAGE_ADAPTER__H
#define _LANGUAGE_ADAPTER__H

#include <string>
using namespace std;
#include "parser/object_storage.h"
#include "backend/EnigmaStruct.h"
#include "frontend.h"

struct language_adapter {
  virtual string get_name() = 0;
  
  // Sizable utilities
  virtual int link_globals(parsed_object*, EnigmaStruct*, parsed_script*[]) = 0;

  // IDE_EDITABLEs added before compile
  virtual int compile_parseAndLink(EnigmaStruct*,parsed_script*[]) = 0;
  virtual int compile_parseSecondary(map<int,parsed_object*>&,parsed_script*[],int scrcount,map<int,parsed_room*>&,parsed_object*) = 0;
  virtual int compile_writeGlobals(EnigmaStruct*,parsed_object*) = 0;
  virtual int compile_writeObjectData(EnigmaStruct*,parsed_object*) = 0;
  virtual int compile_writeObjAccess(map<int,parsed_object*>&,parsed_object*) = 0;
  virtual int compile_writeFontInfo(EnigmaStruct* es) = 0;
  virtual int compile_writeRoomData(EnigmaStruct* es,parsed_object *EGMglobal) = 0;
  virtual int compile_writeShaderData(EnigmaStruct* es,parsed_object *EGMglobal) = 0;
  virtual int compile_writeDefraggedEvents(EnigmaStruct* es) = 0;
  virtual int compile_handle_templates(EnigmaStruct* es) = 0;

  // Resources added to module
  virtual int module_write_sprites(EnigmaStruct *es, FILE *gameModule) = 0;
  virtual int module_write_sounds(EnigmaStruct *es, FILE *gameModule) = 0;
  virtual int module_write_backgrounds(EnigmaStruct *es, FILE *gameModule) = 0;
  virtual int module_write_paths(EnigmaStruct *es, FILE *gameModule) = 0;
  virtual int module_write_fonts(EnigmaStruct *es, FILE *gameModule) = 0;
  
  // Globals and locals
  virtual int  load_shared_locals() = 0;
  virtual void load_extension_locals() = 0;
  virtual bool global_exists(string name) = 0;
  
  // Big things
  virtual syntax_error* definitionsModified(const char*, const char*) = 0;
  virtual int compile(EnigmaStruct *es, const char* exe_filename, int mode) = 0;
  
  virtual ~language_adapter();
};

extern map<string,language_adapter*> languages;
extern language_adapter *current_language;
extern string current_language_name;

#include <API/context.h>
extern jdi::context *main_context;

#endif
