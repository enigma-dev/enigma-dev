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

#include <Storage/definition.h>
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

  virtual int compile_writeGlobals(const GameData &game, const ParsedScope* global, const DotLocalMap &dot_accessed_locals) = 0;
  virtual int compile_writeObjectData(const GameData &game, const CompileState &state, int mode) = 0;
  virtual int compile_writeObjAccess(const ParsedObjectVec &parsed_objects, const DotLocalMap &dot_accessed_locals, const ParsedScope* global, bool treatUninitAs0) = 0;
  virtual int compile_writeFontInfo(const GameData &game) = 0;
  virtual int compile_writeRoomData(const GameData &game, const ParsedRoomVec &parsed_rooms, ParsedScope *EGMglobal, int mode) = 0;
  virtual int compile_writeShaderData(const GameData &game, ParsedScope *EGMglobal) = 0;
  virtual int compile_writeDefraggedEvents(const GameData &game, const std::set<EventGroupKey> &used_events, const ParsedObjectVec &parsed_objects) = 0;

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

  // Getter to retrieve event data. Sort of arbitrary but idgaf right now.
  // Lots of loose ends to sort regarding ownership of static state.
  // Maybe this all belongs in CompileState. Who knows.
  virtual const EventData &event_data() const = 0;

  // ===============================================================================================
  // == Language Integration =======================================================================
  // ===============================================================================================

  // Returns whether the given definition is a function accepting `enigma::varargs`.
  virtual bool is_variadic_function(jdi::definition *d) = 0;
  // Returns the index at which a function parameters ref_stack is variadic;
  // that is, at which argument position it accepts `enigma::varargs`.
  virtual int function_variadic_after(jdi::definition_function *refs) = 0;

  //v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@v&v@
  // FIXME: JDI has leaked into all languages. I've moved that leak here, into the light.  X
  //^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@^&^@

  /// Look up a type by its name.
  virtual jdi::definition* find_typename(string name) = 0;
  /// Check whether the given definition is callable as a function.
  virtual bool definition_is_function(jdi::definition *d) = 0;
  /// Assuming this definition is a function, retrieve the number of overloads it has.
  virtual size_t definition_overload_count(jdi::definition *d) = 0;
  /// Read parameter bounds from the current definition into args min and max. For variadics, max = unsigned(-1).
  virtual void definition_parameter_bounds(jdi::definition *d, unsigned &min, unsigned &max) = 0;
  /// Create a script with the given name (and an assumed 16 parameters, all defaulted) to the given scope.
  virtual void quickmember_script(jdi::definition_scope* scope, string name) = 0;
  /// Create a standard integer variable member in the given scope.
  virtual void quickmember_integer(jdi::definition_scope* scope, string name) = 0;
  /// Look up an enigma_user definition by its name.
  virtual jdi::definition* look_up(const string &name) = 0;

  virtual ~language_adapter();
};

extern map<string, language_adapter*> languages;
extern language_adapter *current_language;
extern string current_language_name;

#include <API/context.h>
extern jdi::context *main_context;

#endif
