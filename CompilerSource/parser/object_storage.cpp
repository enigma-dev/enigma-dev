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

#include <map>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

#include "general/darray.h"

#include <Storage/definition.h>
#include "object_storage.h"
#include "settings-parse/crawler.h"
#include "compiler/compile_common.h"


map<string,int> shared_object_locals;
map<string,dectrip> dot_accessed_locals;

#include "languages/lang_CPP.h"
int shared_locals_load(vector<string> exts)
{
  cout << "Finding parent..."; fflush(stdout);

  // Find namespace enigma
  jdi::definition* pscope = main_context->get_global()->look_up("enigma");
  if (!pscope or !(pscope->flags & jdi::DEF_SCOPE)) {
    cerr << "ERROR! Can't find namespace enigma!" << endl;
    return 1;
  }
  jdi::definition_scope* ns_enigma = (jdi::definition_scope*)pscope;
  jdi::definition* parent = ns_enigma->look_up(system_get_uppermost_tier());
    if (!parent) {
      cerr << "ERROR! Failed to find parent scope `" << system_get_uppermost_tier() << endl;
      return 2;
    }
  if (not(parent->flags & jdi::DEF_CLASS)) {
    cerr << "PARSE ERROR! Parent class is not a class?" << endl;
    cout << parent->parent->name << "::" << parent->name << ":  " << parent->toString() << endl;
    return 3;
  }
  jdi::definition_class *pclass = (jdi::definition_class*)parent;
  
  // Find the parent object
  cout << "Found parent scope" << endl;

  shared_object_locals.clear();

  //Iterate the tiers of the parent object
  for (jdi::definition_class *cs = pclass; cs; cs = (cs->ancestors.size() ? cs->ancestors[0].def : NULL) )
  {
    cout << " >> Checking ancestor " << cs->name << endl;
    for (jdi::definition_scope::defiter mem = cs->members.begin(); mem != cs->members.end(); ++mem)
      shared_object_locals[mem->first] = 0;
  }

  extensions::crawl_for_locals();
  extensions::dump_read_locals(shared_object_locals);
  return 0;
}

int shared_locals_clear()
{
  shared_object_locals.clear();
  return !shared_object_locals.empty();
}

dectrip::dectrip(): type(), prefix(), suffix() {}
dectrip::dectrip(string t): type(t), prefix(), suffix() {}
dectrip::dectrip(string t,string p, string s): type(t), prefix(p), suffix(s) {}

decquad::decquad(): type(), prefix(), suffix(), value() {}
decquad::decquad(string t): type(t), prefix(), suffix(), value() {}
decquad::decquad(string t,string p, string s, string v): type(t), prefix(p), suffix(s), value(v) {}


bool dectrip::defined() {
  return type != "" or prefix != "" or suffix != "";
}
bool dectrip::operator!= (const dectrip& x) {
  return type != x.type or prefix != x.prefix or suffix != x.suffix;
}
bool decquad::defined() {
  return type != "" or prefix != "" or suffix != "" or value != "";
}
bool decquad::operator!= (const decquad& x) {
  return type != x.type or prefix != x.prefix or suffix != x.suffix or value != x.value;
}

parsed_event::parsed_event():                               id(0), mainId(0), code(), synt(), strc(0), otherObjId(-4), myObj(NULL) {}
parsed_event::parsed_event(parsed_object *po):              id(0), mainId(0), code(), synt(), strc(0), otherObjId(-4), myObj(po) {}
parsed_event::parsed_event(int m, int s,parsed_object *po): id(s), mainId(m), code(), synt(), strc(0), otherObjId(-4), myObj(po) {}
parsed_object::parsed_object(): parent(NULL) {}
parsed_object::parsed_object(string n, int i, int s, int m, int p, bool vis, bool sol, double d,bool pers): name(n), id(i), sprite_index(s), mask_index(m), parent_index(p), visible(vis), solid(sol), persistent(pers), depth(d), parent(NULL) {}
map<int,parsed_object*> parsed_objects;
map<int,parsed_room*> parsed_rooms;
vector<parsed_extension> parsed_extensions;
vector<string> requested_extensions;

void parsed_object::copy_from(parsed_object& source, string sourcename, string destname)
{
  parsed_object& dest = *this;
  //Copy
  for (parsed_object::dotit vit = source.dots.begin(); vit != source.dots.end(); vit++)
    dest.dots[vit->first] = 0;
  for (parsed_object::locit vit = source.locals.begin(); vit != source.locals.end(); vit++)
  {
    dectrip &t = dest.locals[vit->first];
    if (!t.defined())
      t = vit->second, cout << "Copied `" << vit->first << "' from " << sourcename << " to " << destname;
    else if (vit->second.defined() and vit->second != t)
      cout << "***ENIGMA: WARNING: Conflicting types `" << vit->second.type << vit->second.prefix << vit->second.suffix << "' and `" << t.type << t.prefix << t.suffix << "' to variable `" << vit->first << "' in " << destname;
  }
  for (parsed_object::ambit vit = source.ambiguous.begin(); vit != source.ambiguous.end(); vit++)
  {
    dectrip &t = dest.ambiguous[vit->first];
    if (!t.defined())
      t = vit->second, cout << "Copied `" << vit->first << "' from " << sourcename << " to " << destname;
    else if (vit->second.defined() and vit->second != t)
      cout << "***ENIGMA: WARNING: Conflicting types `" << vit->second.type << vit->second.prefix << vit->second.suffix << "' and `" << t.type << t.prefix << t.suffix << "' to variable `" << vit->first << "' in " << destname;
  }
  for (parsed_object::globit vit = source.globals.begin(); vit != source.globals.end(); vit++)
  {
    dectrip &t = dest.globals[vit->first];
    if (!t.defined())
      t = vit->second, cout << "Copied `" << vit->first << "' from " << sourcename << " to " << destname;
    else if (vit->second.defined() and vit->second != t)
      cout << "***ENIGMA: WARNING: Conflicting types `" << vit->second.type << vit->second.prefix << vit->second.suffix << "' and `" << t.type << t.prefix << t.suffix << "' to variable `" << vit->first << "' in " << destname;
  }
  for (parsed_object::constit vit = source.consts.begin(); vit != source.consts.end(); vit++)
  {
    decquad &t = dest.consts[vit->first];
    if (!t.defined())
      t = vit->second, cout << "Copied `" << vit->first << "' from " << sourcename << " to " << destname;
    else if (vit->second.defined() and vit->second != t)
      cout << "***ENIGMA: WARNING: Conflicting types `" << vit->second.type << vit->second.prefix << vit->second.suffix << "' and `" << t.type << t.prefix << t.suffix << "' to variable `" << vit->first << "' in " << destname;
  }
}
//subscr->second
void parsed_object::copy_calls_from(parsed_object& source)
{
  for (parsed_object::funcit sit = source.funcs.begin(); sit != source.funcs.end(); sit++)
  {
    int &x = funcs[sit->first]; //copy the function into curscript's called list.
    x = x > sit->second ? x : sit->second; //use larger max param count
  }
}

void parsed_object::copy_tlines_from(parsed_object& source)
{
  for (parsed_object::tlineit sit = source.tlines.begin(); sit != source.tlines.end(); sit++)
  {
    tlines[sit->first] = 1;
  }
}


#include <vector>
#include "backend/ideprint.h"

typedef map<string,dectrip> msi;
struct useinfo { dectrip dec; int c; string lastobject; };
void add_dot_accessed_local(string name)
{
  pair<msi::iterator, bool> insd = dot_accessed_locals.insert(msi::value_type(name,dectrip()));
  if (!insd.second) // If we didn't insert new,
    return; // all this figuring has been done already

  user << "Add dot accessed local " << name << flushl;

  map<string,useinfo> uses;
  insd.first->second.type = "var"; // Default, just in case of stupidity.

  int maxvotes = 0; // The highest number of votes on a type for this variable
  for (po_i it = parsed_objects.begin(); it != parsed_objects.end(); it++) // For each of our objects
  {
    msi::iterator itt = it->second->locals.find(name); // Find a variable by this name
    if (itt != it->second->locals.end() and itt->second.type != "") // If found, and indeed declared locally
    {
      pair<map<string,useinfo>::iterator,bool> ins = uses.insert(pair<string,useinfo>(itt->second.type,useinfo()));
      if (ins.second and uses.size() > 1) {
        user << "Warning: variable `" << name << "` is accessed via GM Integer, but varies in type.";
        user << "Info: Declaration in object " << it->second->name << " conflicts with that in object " << ins.first->second.lastobject << flushl;
      }
      ins.first->second.lastobject = it->second->name;
      ins.first->second.dec = itt->second;
      ins.first->second.c++;

      if (ins.first->second.c > maxvotes)
        insd.first->second = ins.first->second.dec, maxvotes = ins.first->second.c;
    }
  }
  user << insd.first->second.prefix << " " << insd.first->second.type << " " << insd.first->second.suffix << flushl;
}
