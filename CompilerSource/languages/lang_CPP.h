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
#include "event_reader/event_parser.h"
#include <Storage/definition.h>
#include <System/builtins.h>
#include <API/context.h>

struct lang_CPP: language_adapter {
  /// The context of all parsed definitions.
  jdi::Context definitions;

  /// The ENIGMA namespace.
  jdi::definition_scope *namespace_enigma, *namespace_enigma_user;
  jdi::definition *enigma_type__var, *enigma_type__variant, *enigma_type__varargs;

  // Utility
  string get_name() final;

  // IDE_EDITABLEs added before compile
  int compile_parseAndLink(const GameData &game, CompileState &state) final;
  int link_globals(const GameData &game, CompileState &state) final;
  int link_ambiguous(const GameData &game, CompileState &state) final;
  int compile_parseSecondary(CompileState &state) final;

  int compile_writeGlobals(const GameData &game, const ParsedScope* global, const DotLocalMap &dot_accessed_locals) final;
  int compile_writeObjectData(const GameData &game, const CompileState &state, int mode) final;
  int compile_writeObjAccess(const ParsedObjectVec &parsed_objects, const DotLocalMap &dot_accessed_locals, const ParsedScope* global, bool treatUninitAs0) final;
  int compile_writeFontInfo(const GameData &game) final;
  int compile_writeRoomData(const GameData &game, const CompileState &state, int mode) final;
  int compile_writeShaderData(const GameData &game, ParsedScope *EGMglobal) final;
  int compile_writeDefraggedEvents(const GameData &game, const std::set<EventGroupKey> &used_events, const ParsedObjectVec &parsed_objects) final;

  // Resources added to module
  int module_write_sprites(const GameData &game, FILE *gameModule) final;
  int module_write_sounds(const GameData &game, FILE *gameModule) final;
  int module_write_backgrounds(const GameData &game, FILE *gameModule) final;
  int module_write_paths(const GameData &game, FILE *gameModule) final;
  int module_write_fonts(const GameData &game, FILE *gameModule) final;

  int  load_shared_locals() final;
  void load_extension_locals() final;
  bool global_exists(string name) const final;

  syntax_error* definitionsModified(const char*, const char*) final;
  int compile(const GameData &game, const char* exe_filename, int mode) final;

  const EventData &event_data() const final { return evdata_; }
  const enigma::parsing::MacroMap &builtin_macros() const final { return builtin_macros_; }
  const setting::CompatibilityOptions &compatibility_opts() const final { return compatibility_opts_; }
  const NameSet &shared_object_locals() const final { return shared_object_locals_; }

  // ===============================================================================================
  // == The following methods are implemented in jdi_utility.cpp ===================================
  // ===============================================================================================

  /// Look up a type by its name.
  jdi::definition* find_typename(std::string_view name) const final;

  // Returns whether the given definition is a function accepting `enigma::varargs`.
  bool is_variadic_function(jdi::definition *d) const final;
  // Returns the index at which a function parameters ref_stack is variadic;
  // that is, at which argument position it accepts `enigma::varargs`.
  int function_variadic_after(jdi::definition_function *func) const final;

  /// Check whether the given definition is callable as a function.
  bool definition_is_function(jdi::definition *d) const final;
  /// Assuming this definition is a function, retrieve the number of overloads it has.
  size_t definition_overload_count(jdi::definition *d) const final;
  /// Read parameter bounds from the current definition into args min and max. For variadics, max = unsigned(-1).
  void definition_parameter_bounds(jdi::definition *d, unsigned &min, unsigned &max) const final;

  /// Create a script with the given name (and an assumed 16 parameters, all defaulted) to the given scope.
  void quickmember_script(jdi::definition_scope* scope, string name) final;
  /// Create a standard integer variable member in the given scope.
  void quickmember_integer(jdi::definition_scope* scope, string name) final {
    return quickmember_variable(scope, jdi::builtin_type__int, name);
  }
  /// Look up an enigma_user definition by its name.
  jdi::definition* look_up(std::string_view name) const final;

  // Reads in event data automatically. This isn't great, but is better than
  // accessing everything statically (for future refactors).
  lang_CPP();
  virtual ~lang_CPP() = default;

 private:
  /// Create a standard variable member in the given scope.
  void quickmember_variable(jdi::definition_scope* scope, jdi::definition* type, string name);
  // Stores event data loaded from events.ey.
  EventData evdata_;
  // Cache of all built-in macros computed just after JDI parse finishes.
  enigma::parsing::MacroMap builtin_macros_;
  // Settings stuffed here instead of the global scope...
  setting::CompatibilityOptions compatibility_opts_;
  // Cache of object locals mined from object_basic and the object tiers.
  NameSet shared_object_locals_;
};

#endif
