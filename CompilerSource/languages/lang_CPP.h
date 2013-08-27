/**
  @file  lang_CPP.h
  @brief Defines the C++ languages adapter class.
  
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

#ifndef _LANG_CPP__H
#define _LANG_CPP__H
#include "language_adapter.h"
#include <Storage/definition.h>
#include <API/context.h>

struct lang_CPP: language_adapter {
  /// A map of all global local variables.
  map<string,dectrip> locals;
  
  /// The context of all parsed definitions.
  jdi::context definitions;
  
  /// The ENIGMA namespace.
  jdi::definition_scope *namespace_enigma;
  
  // Utility
  string get_name();
  
  // Sizable utilities
  int link_globals(parsed_object*, EnigmaStruct*,parsed_script*[]);

  // IDE_EDITABLEs added before compile
  int compile_parseAndLink(EnigmaStruct*,parsed_script*[]);
  int compile_parseSecondary(map<int,parsed_object*>&,parsed_script*[],int scrcount,map<int,parsed_room*>&,parsed_object*);
  int compile_writeGlobals(EnigmaStruct*,parsed_object*);
  int compile_writeObjectData(EnigmaStruct*,parsed_object*);
  int compile_writeObjAccess(map<int,parsed_object*>&,parsed_object*);
  int compile_writeFontInfo(EnigmaStruct* es);
  int compile_writeRoomData(EnigmaStruct* es, parsed_object *EGMglobal);
  int compile_writeShaderData(EnigmaStruct* es, parsed_object *EGMglobal);
  int compile_writeDefraggedEvents(EnigmaStruct* es);
  int compile_handle_templates(EnigmaStruct* es);

  // Resources added to module
  int module_write_sprites(EnigmaStruct *es, FILE *gameModule);
  int module_write_sounds(EnigmaStruct *es, FILE *gameModule);
  int module_write_backgrounds(EnigmaStruct *es, FILE *gameModule);
  int module_write_paths(EnigmaStruct *es, FILE *gameModule);
  int module_write_fonts(EnigmaStruct *es, FILE *gameModule);
  
  int  load_shared_locals();
  void load_extension_locals();
  
  virtual syntax_error* definitionsModified(const char*, const char*);
  virtual int compile(EnigmaStruct *es, const char* exe_filename, int mode);
  
  bool global_exists(string name);
  jdi::definition* find_typename(string name);
  
  virtual ~lang_CPP();
};

#endif
