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

/**
  @brief E-Yaml is a compromise format between my original idea of "a good way
         to store user-editable information" and, well, the official YAML spec.
         I wanted something lightweight and very easy to implement, too. Hence,
         E-YAML was born as a forwards-compatible implementation of YAML for my
         uses. E-YAML files can be read by official YAML readers, but are much
         easier to write a custom reader for since the format doesn't implement
         most of the noise that makes YAML general-purpose.
*/

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

#include "eyaml.h"
#include "../general/parse_basics.h"

inline string tolower(string x) {
  for (size_t i = 0; i < x.length(); i++)
    if (x[i] >= 'A' and x[i] <= 'Z') x[i] -= 'A' - 'a';
  return x;
}

ey_base::ey_base(bool y): name( ), is_scalar(y) {}
ey_base::ey_base(string x,bool y): name(x), is_scalar(y) {}

ey_data::eylist::eylist(): value(NULL), next(NULL) {}
ey_data::eylist::eylist(ey_data::eylist* p): value(NULL), next(NULL) { p->next = this; }

ey_string::ey_string(): ey_base(true), value( ) {}
ey_string::ey_string(string y): ey_base(true), value(y) {}
ey_string::ey_string(string x, string y): ey_base(x, true), value(y) {}

ey_data::ey_data(): ey_base(false), values(), values_order() {}
ey_data::ey_data(string n): ey_base(n, false), values(), values_order() {}
ey_data::~ey_data()
{
  for (eycit it = values_order.next; it != NULL; )
  {
    eycit t = it; it = it->next;
    t->value->is_scalar ? delete (ey_string*)t->value : delete (ey_data*)t->value;
    delete t;
  }
}

eyit eyaml_ci_find(ey_data &dat, string str) {
  return dat.values.find(str);
}

struct y_level {
  y_level *prev;
  ey_data *s;
  int i;
  y_level(y_level* a, ey_data *b, int c): prev(a), s(b), i(c) {}
};

ey_data parse_eyaml(ifstream &file, string filename)
{
  string line;
  ey_data res;
  getline(file,line);
  
  if (tolower(line.substr(0,7)) != "%e-yaml")
    return res;
  
  if (filename == "") filename = "(unspecified)";
  
  y_level *cur = new y_level(NULL,&res,0);
  eycit latestc = &cur->s->values_order;
  eyit latest = cur->s->values.end();
  
  string unlowered = "pizza, lol";
  int linenum = 1;
  continue_2:
  while (!file.eof())
  {
    getline(file,line); linenum++;
    if (line.substr(0,3) == "---") continue;
    int inds = 0; pt pos = 0;
    while (is_useless(line[pos])) pos++, inds++;
    
    if (!(pos < line.length()) or line[pos] == '#' or line[pos] == '%')
      continue;
    
    const pt nsp = pos;
    while (line[pos] != ':' and pos < line.length()) if (line[pos++] == '#') goto continue_2;
    const string nname = line.substr(nsp, pos-nsp);
    
    if (inds != cur->i)
    {
      if (inds > cur->i) // This level has more indentation.
      {
        if (cur->s->values.empty())
          cur->i = inds;
        else
        {
          if (latest->second != NULL)
            cout << "Indent increased unexpectedly on line " << linenum << " of file " << filename;
          else {
            cur->s->values_order_last = latestc;
            cur = new y_level(cur, (ey_data*)(latest->second = latest->second = latestc->value = new ey_data(unlowered)), inds);
            latest = cur->s->values.insert(eypair(tolower(nname), NULL)) . first;
            latestc = new ey_data::eylist(&cur->s->values_order);
          }
        }
      }
      else // This level has less indentation than the last; drop.
      {
        if (latest->second == NULL)
          latest->second = latestc->value = new ey_string(unlowered,"");
        while (cur->prev and inds < cur->i) {
          y_level *cd = cur;
          cur = cur->prev;
          delete cd;
        }
        latest = cur->s->values.insert(eypair(tolower(nname), NULL)) . first;
        latestc = new ey_data::eylist(cur->s->values_order_last);
      }
    }
    else // We are at the same indentation as before, and so we will simply add an item to this current scope
    {
      if (latest != cur->s->values.end() and latest->second == NULL)
        latest->second = latestc->value =  new ey_string(unlowered,""); 
      latest = cur->s->values.insert(eypair(tolower(nname), NULL)) . first;
      latestc = new ey_data::eylist(latestc);
    }
    
   // cout << endl << nname << endl;
    unlowered = nname;
    if (line[pos] != ':')
      continue;
    
    while (is_useless(line[++pos]));
    
    const pt vsp = pos;
    while (line[pos] and line[pos] != '#') pos++;
    while (is_useless(line[--pos]));
    
    if (++pos > vsp) {
      //cout << "Added " << unlowered << " as '" << line.substr(vsp, pos - vsp) << "' naturally.\n";
      latest->second = latestc->value = new ey_string(unlowered, line.substr(vsp, pos - vsp));
    }
  }
  if (latest != cur->s->values.end() and latest->second == NULL)
    latest->second = latestc->value =  new ey_string(unlowered,"");
  return res;
}
