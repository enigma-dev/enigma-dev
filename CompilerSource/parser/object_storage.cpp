/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

#include "../general/darray.h"

#include "object_storage.h"
#include "../externs/externs.h"


map<string,int> shared_object_locals;
int shared_locals_load()
{
  cout << "Finding parent..."; fflush(stdout);
  
  // Find namespace enigma
  externs* pscope = NULL;
  current_scope = &global_scope;
  extiter ns_enigma = current_scope->members.find("enigma");

  // Find the parent object
  if (ns_enigma != current_scope->members.end()) {
    extiter parent = ns_enigma->second->members.find("object_collisions");
    if (parent != ns_enigma->second->members.end())
      pscope = parent->second;
  }
  cout << "found"; fflush(stdout);
  
  //Iterate the tiers of the parent object
  for (externs *cs = pscope; cs; cs = (cs->ancestors.size ? cs->ancestors[0] : NULL) )
  {
    cout << " >> Checking ancestor " << cs->name << endl;
    for (extiter mem = cs->members.begin(); mem != cs->members.end(); mem++)
      shared_object_locals[mem->first] = 0;
  }
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
parsed_object::parsed_object() {}
parsed_object::parsed_object(string n, int i, int s, int p, bool vis, bool sol): name(n), id(i), sprite_index(s), parent(p), visible(vis), solid(sol) {}
map<int,parsed_object*> parsed_objects;

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
