/**
  @file  language_adapter.h
  @brief Defines a class by which ENIGMA can interface with and output to other languages.

  @section License
    Copyright (C) 2008-2012 Josh Ventura
    Copyright (C) 2014 Seth N. Hetu
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

#ifndef ENIGMA_LANGUAGE_ADAPTER_H
#define ENIGMA_LANGUAGE_ADAPTER_H

#include <string>
#include <vector>
#include <map>
#include <set>

#include "backend/GameData.h"
#include "parser/object_storage.h"
#include "frontend.h"

struct language_adapter {
  virtual string get_name() = 0;

  // IDE_EDITABLEs added before compile
  virtual int compile_parseAndLink(const GameData &game, CompileState &state) = 0;
  virtual int link_globals(const GameData &game, CompileState &state) = 0;
  virtual int link_ambiguous(const GameData &game, CompileState &state) = 0;
  virtual int compile_parseSecondary(CompileState &state) = 0;

  virtual int compile_writeGlobals(const GameData &game, const parsed_object* global, const DotLocalMap &dot_accessed_locals) = 0;
  virtual int compile_writeObjectData(const GameData &game, const CompileState &state, int mode) = 0;
  virtual int compile_writeObjAccess(const ParsedObjectVec &parsed_objects, const DotLocalMap &dot_accessed_locals, const parsed_object* global, bool treatUninitAs0) = 0;
  virtual int compile_writeFontInfo(const GameData &game) = 0;
  virtual int compile_writeRoomData(const GameData &game, const ParsedRoomVec &parsed_rooms, parsed_object *EGMglobal, int mode) = 0;
  virtual int compile_writeShaderData(const GameData &game, parsed_object *EGMglobal) = 0;
  virtual int compile_writeDefraggedEvents(const GameData &game, const ParsedObjectVec &parsed_objects) = 0;

  // Resources added to module
  virtual int module_write_sprites(const GameData &game, FILE *gameModule) = 0;
  virtual int module_write_sounds(const GameData &game, FILE *gameModule) = 0;
  virtual int module_write_backgrounds(const GameData &game, FILE *gameModule) = 0;
  virtual int module_write_paths(const GameData &game, FILE *gameModule) = 0;
  virtual int module_write_fonts(const GameData &game, FILE *gameModule) = 0;

  // Globals and locals
  virtual int  load_shared_locals() = 0;
  virtual void load_extension_locals() = 0;
  virtual bool global_exists(string name) = 0;

  // Big things
  virtual syntax_error* definitionsModified(const char*, const char*) = 0;
  virtual int compile(const GameData &game, const char* exe_filename, int mode) = 0;

  virtual ~language_adapter();
};

extern map<string, language_adapter*> languages;
extern language_adapter *current_language;
extern string current_language_name;

#include <API/context.h>
extern jdi::context *main_context;

#endif
