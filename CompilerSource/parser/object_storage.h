/**
  @file  object_storage.h
  @brief Declares classes for representing various facets of ENIGMA games in memory.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
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

#ifndef _OBJECT_STORAGE__H
#define _OBJECT_STORAGE__H

#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct scope_harvest;
struct parsed_code;
struct parsed_event;
struct parsed_object;
struct parsed_script;
struct parsed_room;
struct parsed_extension;
struct compile_context;

typedef map<int,parsed_object*> po_map;
typedef map<int,parsed_event*> pe_map;
typedef map<int,parsed_room*> pr_map;
typedef map<string,parsed_script*> ps_map;
typedef po_map::iterator po_i;
typedef pe_map::iterator pe_i;
typedef pr_map::iterator pr_i;
typedef ps_map::iterator ps_i;

#include <Storage/definition.h>
using namespace jdi;

/// Structure depicting an initialization for a variable in a scope: Used for conflict resolution while initializing constants.
struct initialization {
  AST* initial_value; ///< The value to initialize as
  bool global; ///< The scope in which this is to be initialized; true for global scope, or false if local (to the object).
};

struct scrcall {
  parsed_script *script; ///< The script that was invoked
  int arg_max; ///< The largest number of arguments with which this script was invoked
  void consolidate(const scrcall &x); ///< Merge another scrcall into this one
};

typedef map<string,initialization> const_map; ///< Set of constants and their initializations (or NULL if uninitialized)
typedef map<string, scrcall> script_map;  ///< Set of functions used in a piece of code
typedef map<string,int> undec_map; ///< Set of undeclared variables and their maximum assumed dimensionality
typedef set<string> func_set;   ///< Set of functions used in a piece of code
typedef set<string> dot_set; ///< Set of arbitrarily dot-accessed variable names

typedef script_map::iterator script_it; ///< Iterator for collections of constant-initialization pairs.
typedef undec_map::iterator undec_it;  ///< Iterator for collections of undeclared local names and maximum assumed dimensionality.
typedef const_map::iterator const_it; ///< Iterator for collections of constant-initialization pairs.
typedef func_set::iterator func_it; ///< Iterator for collections of used function names.
typedef dot_set::iterator dot_it;  ///< Iterator for collection of dot-accessed variable names.

typedef map<string, full_type> dal_map; ///< Dot-Accessed Local map type
typedef dal_map::iterator dal_it; ///< Dot-Accessed Local map iterator type

#include <backend/EnigmaStruct.h>
#include <parser/parse_edl.h>

/// Structure containing harvested statistics of a piece of code, such as undeclared variables, 
struct scope_harvest {
  undec_map undeclared; ///< Any variable KEY used in this code, but not declared in it.
  const_map consts;    ///< Any variable KEY declared as constant and initialized as VALUE.
  script_map scripts; ///< Any function called within this code.
  func_set funcs;    ///< Any function called within this code.
  dot_set dots;     ///< Any attribute accessed via a dot, as in some_id.KEY
  int copy_from(const scope_harvest &sh, string scope1, string scope2); ///< Copy everything from another scope harvest, adding it to this one.
  int copy_scripts_from(const scope_harvest &sh); ///< Copy script calls from another scope harvest, adding them to this one
};

#include <parser/parse_edl.h>

/// Structure containing a representation for any generalized parsed piece of code
struct parsed_code {
  definition_scope code_scope; ///< The main scope of this code.
  EDL_AST ast; ///< An abstract syntax tree generated from the code.
  int parse(string code); ///< Parse the given code into this object
  parsed_code(definition_scope *object_scope, definition_scope *global_scope); ///< Construct with axiliary scopes
};

/// Structure containing details about an event
struct parsed_event
{
  int id; ///< The ID of this event, or the resource index for this event
  int main_id; ///< The Main ID of this event, or the event kind.
  const char *origcode; ///< The original code.
  parsed_code code; ///< The parsed code in this event
  parsed_object *my_obj; ///< The object which contains this event
  parsed_event(int mid, int id, parsed_object *owner, definition_scope *global_scope, const char *code); ///< Construct with full details
  int parse(); ///< Parse the code contained 
};

/// Structure containing 
struct parsed_object
{
  GmObject *properties; ///< General properties of this object supplied by the IDE.
  vector<parsed_event*> events; ///< All the events in this object that have been parsed.
  definition_scope self; ///< The local scope of this object, used for declaring and checking against local variables.
  string class_name; ///< Class name comprising a common prefix and a possibly mangled version of the resource name which can be safely used in code.
  
  scope_harvest sh; ///< Grand collection of undeclareds, constants, used functions, etc.

  void copy_from(parsed_object&, string, string);
  void copy_calls_from(parsed_object&);

  parsed_object(string class_name = string(), GmObject *gmo = NULL); ///< Default constructor, or construct from a GmObject.
  parsed_object(const parsed_object&); ///< Copy constructor.
};

/// Structure containing extra information about a script.
struct parsed_script: parsed_object //Script will pretend to be an object, having locals and globals inherited by all who call it.
{
  Script *properties; ///< The original script structure from the IDE
  definition_scope locals; ///< Local variables which should be inherited by any calling object
  parsed_code code; ///< The code in this script
  int global_args; ///< The maximum number of arguments with which this was invoked from all contexts.
  bool used_argument; ///< This denotes whether the script code used the `argument' variable; if so, one will be generated
  scope_harvest sh; ///< Information about stuff used in this code.
  
  int parse(); ///< Parse the code contained in this script's properties.
  
  parsed_script(Script *src, definition_scope *global); ///< Construct with global scope reference
};

/// Structure containing codes parsed in a room
struct parsed_room {
  Room *properties; ///< Properties of this room
  /// Structure for an instance creation code
  struct parsed_icreatecode {
    parsed_object *my_obj; ///< The object of which this this is an instance.
    parsed_code code; ///< The actual code contained here
    scope_harvest sh; ///< Information about stuff used in this code.
    int parse(string code); ///< Parse the given creation code into this
    parsed_icreatecode(parsed_object *obj, definition_scope *global); ///< Construct with a source object and a global scope
  };
  
  map<int, parsed_icreatecode*> instance_create_codes; ///< Map of instance creation codes by ID
  parsed_code *creation_code; ///< The room creation code, or NULL.
  
  parsed_room(Room *rm); ///< Construct from a Room* from the IDE
};

/// Structure containing information about an extension that has been loaded.
struct parsed_extension {
  string name; ///< The name of this extension.
  string path; ///< The path from which this extension was loaded.
  string pathname; ///< The full path and filename of the extension's descriptor file.
  string implements; ///< The class from which the locals object should inherit in order to use this extension.
};

/// Enumeration of compile modes; that is, modes for how to build and run the game.
enum compile_mode {
  emode_run, ///< The game is to be built as quickly as possible and run.
  emode_debug, ///< The game is to be built in debug mode, with debugging symbols and plenty of error checking.
  emode_design, ///< The game is to be built in design mode, with a built in level editor.
  emode_compile, ///< The game is to be built and optimized fully, but simply exported as a standalone, not run.
  emode_rebuild ///< The engine is to be rebuilt. This option may be removed at some point.
};

/// Structure containing everything you need to know while building a game.
struct compile_context {
  EnigmaStruct *es; ///< The IDE-supplied structure of all things to compile.
  error_handler *herr; ///< An error handler for conveying compile problems to the user.
  compile_mode mode; ///< The compile mode.
  
  definition_scope *global; ///< The scope to which all global definitions go; ie, those variables declared with the global keyword.
  
  po_map parsed_objects; ///< A collection of objects that have been parsed.
  pr_map parsed_rooms; ///< A collection of rooms that have had their room/instance creation codes parsed.
  ps_map parsed_scripts; ///< A collection of scripts that have been parsed.
  
  /// A final copy of all dot-accessed locals, now associated with their respective types
  dal_map dot_accessed_locals;
  /// Other scope harvest data, such as used functions, constants, etc...
  scope_harvest sh;
};

#endif
