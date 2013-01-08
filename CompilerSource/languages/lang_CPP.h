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
#include <ostream>

struct lang_CPP: language_adapter {
  /// The context of all parsed definitions.
  jdi::context definitions;
  
  /// The ENIGMA namespace.
  jdi::definition_scope *namespace_enigma;
  
  // Utility
  string get_name();
  
  // Sizable utilities
  int link_globals(parsed_object*, EnigmaStruct*, compile_context &ctex);

  // IDE_EDITABLEs added before compile
  int compile_writeGlobals(compile_context &ctex);
  int compile_write_resource_names(compile_context &ctex);
  int compile_writeObjectData(compile_context &ctex);
  int compile_writeObjAccess(compile_context &ctex);
  int compile_writeFontInfo(compile_context &ctex);
  int compile_writeRoomData(compile_context &ctex);
  int compile_writeDefraggedEvents(compile_context &ctex);
  int compile_handle_templates(compile_context &ctex);
  
  struct resource_writer_cpp: resource_writer {
    FILE *gameModule; ///< The executable built by the C++ compiler
    string gameFname; ///< The filename of the game module; used to run it
    size_t resourceblock_start; ///< The starting position of the resources we are writing to this game.
    int module_write_sprites    (compile_context &ctex); ///< Method to write all sprites to the compiled module
    int module_write_sounds     (compile_context &ctex); ///< Method to write all sounds to the compiled module
    int module_write_backgrounds(compile_context &ctex); ///< Method to write all backgrounds to the compiled module
    int module_write_paths      (compile_context &ctex); ///< Method to write all paths to the compiled module
    int module_write_fonts      (compile_context &ctex); ///< Method to write all fonts to the compiled module
    int module_write_extensions (compile_context &ctex); ///< Method to write any and all extension resources to the compiled module
    int module_finalize         (compile_context &ctex); ///< Finalize the module
    resource_writer_cpp(FILE *gmod, string gfname); ///< Construct with a game module
  };
  
  int  load_shared_locals();
  int load_extension_locals();
  void clear_ide_editables();
  typedef map<string, definition*> sol_map; ///< Shared Object Locals Map. lol sol
  sol_map shared_object_locals;
  
  const char* establish_bearings(const char *compiler);
  syntax_error* definitionsModified(const char*, const char*);
  resource_writer *compile(compile_context &ctex, const char* filename);
  void run_game(compile_context &ctex, resource_writer *resw);
  int rebuild();
  
  bool global_exists(string name);
  jdi::definition* find_typename(string name);
  
  string format_expression(AST *ast); ///< Format an AST into a C++ expression string
  void print_to_stream(compile_context &ctex, parsed_code& code, int indent, ostream &os); ///< Print formatted C++ code to a stream
  
  // Oher attributes and properties
  static const char *gen_license; ///< The license placed at the top of generated files
  string MAKE_paths;
  string MAKE_tcpaths;
  string MAKE_location;
  string TOPLEVEL_cflags;
  string TOPLEVEL_cppflags;
  string TOPLEVEL_cxxflags;
  string TOPLEVEL_links;
  string CXX_override;
  string CC_override;
  string WINDRES_location;
  string TOPLEVEL_ldflags;
  
  virtual ~lang_CPP();
};

#endif
