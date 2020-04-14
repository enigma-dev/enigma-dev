/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2011 Josh Ventura                                        **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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

#ifndef ENIGMA_OBJECT_STORAGE_H
#define ENIGMA_OBJECT_STORAGE_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include "darray.h"

using namespace std;

//Represent an initializer (name(value) in constructor)
typedef pair<string,string> initpair;

//These parallel ism's structs, but offer additional properties we need to finish compile
struct parsed_object;
struct parsed_event
{
  int id, mainId;
  string code;
  string synt;
  unsigned int strc;
  varray<string> strs;
  int otherObjId;
  parsed_object* myObj; //This will let us add to locals from the code

  parsed_event();
  parsed_event(parsed_object*);
  parsed_event(int,int,parsed_object*);
};
struct dectrip {
  string type, prefix, suffix;
  dectrip();
  dectrip(string t);
  dectrip(string t,string p, string s);

  bool defined() const;
  bool operator!= (const dectrip&) const;
};
struct decquad {
  string type, prefix, suffix, value;
  decquad();
  decquad(string t);
  decquad(string t,string p, string s, string v);

  bool defined() const;
  bool operator!= (const decquad&) const;
};

struct parsed_object
{
  varray<parsed_event> events;

  string name;
  int id;
  string sprite_name, mask_name, parent_name;
  bool visible, solid, persistent;
  double depth;

  parsed_object* parent; ///< The parent of this object, or NULL if the object has none.
  vector<parsed_object*> children; ///< A vector of the children of this object; parsed_objects which list this object as a parent.

  map<string,dectrip> locals;   ///< Any variable KEY used but not declared, or declared as local VALUE.
  map<string,dectrip> ambiguous;///< Any variable KEY used in with statement, but not locally declared.
  map<string,dectrip> globals;  ///< Any variable KEY declared as global VALUE.
  map<string,decquad> consts;   ///< Any variable KEY declared as constant VALUE.
  map<string,int> globallocals; ///< Any shared local variable KEY used.
  map<string,int> funcs;        ///< Any function KEY called with at most VALUE parameters.
  map<string,int> tlines;       ///< Any timeline KEY set (or possibly set) by this object.
  map<string,int> dots;         ///< Any attribute KEY accessed via a dot, as in a.KEY

  vector<initpair> initializers; // Variables that need initialized in the constructor for this object

  typedef map<string,dectrip>::iterator locit;
  typedef map<string,dectrip>::iterator ambit;
  typedef map<string,dectrip>::iterator globit;
  typedef map<string,dectrip>::const_iterator cglobit;
  typedef map<string,decquad>::iterator constit;
  typedef map<string,int>::iterator funcit;
  typedef map<string,int>::const_iterator const_funcit;
  typedef map<string,int>::iterator tlineit;
  typedef map<string,int>::const_iterator const_tlineit;
  typedef map<string,int>::iterator dotit;

  void copy_from(parsed_object&, string, string);
  void copy_calls_from(parsed_object&);
  void copy_tlines_from(parsed_object&);

  parsed_object();
  parsed_object(string,int,string,string,string,bool,bool,double,bool);
};

struct parsed_script {
  parsed_object obj; //Script will pretend to be an object, having locals and globals inherited by all who call it.
  parsed_event pev, *pev_global; // The former is this scripts code, as an "event". The latter is a global scope version; see documentation
  int globargs; // The maximum number of arguments with which this was invoked from all contexts.
  parsed_script(): obj(), pev(&obj), pev_global(NULL), globargs(0) {}; //automatically link our even to our object.
};

struct parsed_moment {
  int step;
  parsed_script *script;
  parsed_moment(int s, parsed_script *scr): step(s), script(scr) {}
};

struct parsed_timeline {
  int id;
  vector<parsed_moment> moments;
};

struct parsed_room: parsed_object {
  struct parsed_icreatecode { parsed_event* pe; string object_name; };
  map<int,parsed_icreatecode> instance_create_codes;
  //PreCreate code uses the same struct, as nothing is really different
  map<int,parsed_icreatecode> instance_precreate_codes;
};

typedef map<string,dectrip>::iterator deciter;
typedef map<string,dectrip>::const_iterator decciter;

struct parsed_extension {
  string name, path;
  string pathname;
  string implements, init;
};

typedef set<string> NameSet;
typedef set<string> SharedLocalSet;
typedef map<string, dectrip> DotLocalMap;
typedef vector<parsed_object*> ParsedObjectVec;
typedef vector<parsed_script*> ParsedScriptVec;
typedef vector<parsed_room*> ParsedRoomVec;
typedef vector<parsed_extension> ParsedExtensionVec;

typedef map<string,parsed_script*> ScriptLookupMap;
typedef map<string, parsed_timeline> TimelineLookupMap;

typedef ParsedObjectVec::iterator po_i;
typedef map<int,parsed_event*>  :: iterator pe_i;
typedef map<int,parsed_room*>   :: iterator pr_i;

// Global because seriously everything uses it; will need to be moved in a new PR
extern SharedLocalSet shared_object_locals;
// This is global because it's cached between builds
extern vector<string> requested_extensions_last_parse;
extern ParsedExtensionVec parsed_extensions;

struct CompileState {
  //Locals that are inherited by all instances of all objects from the core system.
  DotLocalMap dot_accessed_locals;
  ParsedObjectVec parsed_objects;
  ParsedScriptVec parsed_scripts;
  ParsedScriptVec parsed_tlines;
  ParsedRoomVec parsed_rooms;
  
  ScriptLookupMap script_lookup;
  TimelineLookupMap timeline_lookup;
  
  parsed_object global_object;

  void add_dot_accessed_local(string name);
};

#endif
