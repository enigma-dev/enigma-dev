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

#include "darray.h"

#include <Storage/definition.h>
#include "object_storage.h"
#include "settings-parse/crawler.h"
#include "compiler/compile_common.h"

#include "languages/lang_CPP.h"

dectrip::dectrip(): type(), prefix(), suffix() {}
dectrip::dectrip(string t): type(t), prefix(), suffix() {}
dectrip::dectrip(string t,string p, string s): type(t), prefix(p), suffix(s) {}

decquad::decquad(): type(), prefix(), suffix(), value() {}
decquad::decquad(string t): type(t), prefix(), suffix(), value() {}
decquad::decquad(string t,string p, string s, string v): type(t), prefix(p), suffix(s), value(v) {}


bool dectrip::defined() const {
  return type != "" or prefix != "" or suffix != "";
}
bool dectrip::operator!= (const dectrip& x) const {
  return type != x.type or prefix != x.prefix or suffix != x.suffix;
}
bool decquad::defined() const {
  return type != "" or prefix != "" or suffix != "" or value != "";
}
bool decquad::operator!= (const decquad& x) const {
  return type != x.type or prefix != x.prefix or suffix != x.suffix or value != x.value;
}

parsed_object::parsed_object(): parent(NULL) {}
parsed_object::parsed_object(string n, int i, string s, string m, string p,
                             bool vis, bool sol, double d,bool pers):
    name(n), id(i), sprite_name(s), mask_name(m), parent_name(p), visible(vis),
    solid(sol), persistent(pers), depth(d), parent(NULL) {}

void ParsedScope::copy_from(const ParsedScope &source,
                            const string &sourcename, const string &destname) {
  ParsedScope &dest = *this;
  //Copy
  for (auto vit = source.dots.begin(); vit != source.dots.end(); vit++)
    dest.dots[vit->first] = 0;
  for (auto vit = source.locals.begin(); vit != source.locals.end(); vit++)
  {
    dectrip &t = dest.locals[vit->first];
    if (!t.defined())
      t = vit->second, cout << "Copied `" << vit->first << "' from " << sourcename << " to " << destname;
    else if (vit->second.defined() and vit->second != t)
      cout << "***ENIGMA: WARNING: Conflicting types `" << vit->second.type << vit->second.prefix << vit->second.suffix << "' and `" << t.type << t.prefix << t.suffix << "' to variable `" << vit->first << "' in " << destname;
  }
  for (auto vit = source.ambiguous.begin(); vit != source.ambiguous.end(); vit++)
  {
    dectrip &t = dest.ambiguous[vit->first];
    if (!t.defined())
      t = vit->second, cout << "Copied `" << vit->first << "' from " << sourcename << " to " << destname;
    else if (vit->second.defined() and vit->second != t)
      cout << "***ENIGMA: WARNING: Conflicting types `" << vit->second.type << vit->second.prefix << vit->second.suffix << "' and `" << t.type << t.prefix << t.suffix << "' to variable `" << vit->first << "' in " << destname;
  }
  for (auto vit = source.globals.begin(); vit != source.globals.end(); vit++)
  {
    dectrip &t = dest.globals[vit->first];
    if (!t.defined())
      t = vit->second, cout << "Copied `" << vit->first << "' from " << sourcename << " to " << destname;
    else if (vit->second.defined() and vit->second != t)
      cout << "***ENIGMA: WARNING: Conflicting types `" << vit->second.type << vit->second.prefix << vit->second.suffix << "' and `" << t.type << t.prefix << t.suffix << "' to variable `" << vit->first << "' in " << destname;
  }
  for (auto vit = source.consts.begin(); vit != source.consts.end(); vit++)
  {
    decquad &t = dest.consts[vit->first];
    if (!t.defined())
      t = vit->second, cout << "Copied `" << vit->first << "' from " << sourcename << " to " << destname;
    else if (vit->second.defined() and vit->second != t)
      cout << "***ENIGMA: WARNING: Conflicting types `" << vit->second.type << vit->second.prefix << vit->second.suffix << "' and `" << t.type << t.prefix << t.suffix << "' to variable `" << vit->first << "' in " << destname;
  }
}

void ParsedScope::copy_calls_from(const ParsedScope &source)
{
  for (auto sit = source.funcs.begin(); sit != source.funcs.end(); sit++)
  {
    int &x = funcs[sit->first]; //copy the function into curscript's called list.
    x = x > sit->second ? x : sit->second; //use larger max param count
  }
}

void ParsedScope::copy_tlines_from(const ParsedScope &source)
{
  for (auto sit = source.tlines.begin(); sit != source.tlines.end(); sit++)
  {
    tlines[sit->first] = 1;
  }
}


#include <vector>
#include "backend/ideprint.h"

typedef map<string,dectrip> msi;
struct useinfo { dectrip dec; int c; string lastobject; };
void CompileState::add_dot_accessed_local(string name) {
  pair<msi::iterator, bool> insd = dot_accessed_locals.insert(msi::value_type(name,dectrip()));
  if (!insd.second) // If we didn't insert new,
    return; // all this figuring has been done already

  user << "Add dot accessed local " << name << flushl;

  map<string,useinfo> uses;
  insd.first->second.type = "var"; // Default, just in case of stupidity.

  int maxvotes = 0; // The highest number of votes on a type for this variable
  for (parsed_object *obj : parsed_objects) {
    msi::iterator itt = obj->locals.find(name); // Find a variable by this name
    if (itt != obj->locals.end() and itt->second.type != "") // If found, and indeed declared locally
    {
      pair<map<string,useinfo>::iterator,bool> ins = uses.insert(pair<string,useinfo>(itt->second.type,useinfo()));
      if (ins.second and uses.size() > 1) {
        user << "Warning: variable `" << name << "` is accessed via GM Integer, but varies in type.";
        user << "Info: Declaration in object " << obj->name << " conflicts with that in object " << ins.first->second.lastobject << flushl;
      }
      ins.first->second.lastobject = obj->name;
      ins.first->second.dec = itt->second;
      ins.first->second.c++;

      if (ins.first->second.c > maxvotes)
        insd.first->second = ins.first->second.dec, maxvotes = ins.first->second.c;
    }
  }
  user << insd.first->second.prefix << " " << insd.first->second.type << " " << insd.first->second.suffix << flushl;
}

// TODO: DELETEME
vector<string> requested_extensions_last_parse;
ParsedExtensionVec parsed_extensions;
