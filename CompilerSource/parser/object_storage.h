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

#ifndef _OBJECT_STORAGE__H
#define _OBJECT_STORAGE__H

#include <map>
#include <string>
#include <vector>
#include "general/darray.h"

using namespace std;

//Locals that are inherited by all instances of all objects from the core system.
extern map<string,int> shared_object_locals;
extern map<string,struct dectrip> dot_accessed_locals;
extern int shared_locals_load(vector<string> exts);
extern int shared_locals_clear();

void add_dot_accessed_local(string name);

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

  bool defined();
  bool operator!= (const dectrip&);
};
struct decquad {
  string type, prefix, suffix, value;
  decquad();
  decquad(string t);
  decquad(string t,string p, string s, string v);

  bool defined();
  bool operator!= (const decquad&);
};
struct parsed_object
{
  varray<parsed_event> events;

  string name;
  int id, sprite_index, mask_index, parent_index;
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
  parsed_object(string,int,int,int,int,bool,bool,double,bool);
};

extern map<int,parsed_object*> parsed_objects;

struct parsed_script
{
  parsed_object obj; //Script will pretend to be an object, having locals and globals inherited by all who call it.
  parsed_event pev, *pev_global; // The former is this scripts code, as an "event". The latter is a global scope version; see documentation
  int globargs; // The maximum number of arguments with which this was invoked from all contexts.
  parsed_script(): obj(), pev(&obj), pev_global(NULL), globargs(0) {}; //automatically link our even to our object.
};


struct parsed_room: parsed_object {
  struct parsed_icreatecode { parsed_event* pe; int object_index; };
  map<int,parsed_icreatecode> instance_create_codes;
  //PreCreate code uses the same struct, as nothing is really different 
  map<int,parsed_icreatecode> instance_precreate_codes;
};
extern map<int,parsed_room*> parsed_rooms;
typedef map<int,parsed_object*> :: iterator po_i;
typedef map<int,parsed_event*>  :: iterator pe_i;
typedef map<int,parsed_room*>   :: iterator pr_i;

typedef map<string,dectrip>::iterator deciter;

struct parsed_extension {
  string name, path;
  string pathname;
  string implements;
};

extern vector<parsed_extension> parsed_extensions;
extern vector<string> requested_extensions;

#endif
