/**
  @file  lang_CPP.h
  @brief Defines the C++ languages adapter class.

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

#ifndef ENIGMA_LANG_CPP_H
#define ENIGMA_LANG_CPP_H
#include "language_adapter.h"
#include <Storage/definition.h>
#include <API/context.h>

struct lang_CPP: language_adapter {
  /// The context of all parsed definitions.
  jdi::context definitions;

  /// The ENIGMA namespace.
  jdi::definition_scope *namespace_enigma;

  // Utility
  string get_name() final;

  // IDE_EDITABLEs added before compile
  int compile_parseAndLink(const GameData &game, CompileState &state) final;
  int link_globals(const GameData &game, CompileState &state) final;
  int link_ambiguous(const GameData &game, CompileState &state) final;
  int compile_parseSecondary(CompileState &state) final;

  int compile_writeGlobals(const GameData &game, const parsed_object* global, const DotLocalMap &dot_accessed_locals) final;
  int compile_writeObjectData(const GameData &game, const CompileState &state, int mode) final;
  int compile_writeObjAccess(const ParsedObjectVec &parsed_objects, const DotLocalMap &dot_accessed_locals, const parsed_object* global, bool treatUninitAs0) final;
  int compile_writeFontInfo(const GameData &game) final;
  int compile_writeRoomData(const GameData &game, const ParsedRoomVec &parsed_rooms, parsed_object *EGMglobal, int mode) final;
  int compile_writeShaderData(const GameData &game, parsed_object *EGMglobal) final;
  int compile_writeDefraggedEvents(const GameData &game, const ParsedObjectVec &parsed_objects) final;

  // Resources added to module
  int module_write_sprites(const GameData &game, FILE *gameModule) final;
  int module_write_sounds(const GameData &game, FILE *gameModule) final;
  int module_write_backgrounds(const GameData &game, FILE *gameModule) final;
  int module_write_paths(const GameData &game, FILE *gameModule) final;
  int module_write_fonts(const GameData &game, FILE *gameModule) final;

  int  load_shared_locals() final;
  void load_extension_locals() final;
  bool global_exists(string name) final;

  virtual syntax_error* definitionsModified(const char*, const char*) final;
  virtual int compile(const GameData &game, const char* exe_filename, int mode) final;

  jdi::definition* find_typename(string name);

  virtual ~lang_CPP();
};

#endif
