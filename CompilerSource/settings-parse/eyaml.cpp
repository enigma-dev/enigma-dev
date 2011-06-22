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
ey_data::~ey_data() {
  for (eycit it = values_order.next; it != NULL; ) {
    eycit t = it; it = it->next;
    if (t->value) t->value->is_scalar ? delete (ey_string*)t->value : delete (ey_data*)t->value;
    delete t;
  }
}

// The error message should never be seen
static ey_string eys_empty("[ERROR: REQUESTED VALUE NOT SCALAR]","");

ey_string &ey_data::get(string n) { //Fetch a key's value as a scalar
  eyit it = values.find(n); // Find it
  if (it != values.end() and it->second->is_scalar) // Make sure it's alive and scalar
    return *((ey_string*)it->second); // Return it if so
  return eys_empty; // Otherwise, return empty
}

// Tree iteration
eyit ey_data::find(string n)  { return values.find(n); }
eyit ey_data::begin()         { return values.begin(); }
eyit ey_data::end()           { return values.end(); }
eycit ey_data::first()     { return values_order.next; }

ey_data   &ey_base::data()   { return *(ey_data*)this; }
ey_string &ey_base::scalar() { return *(ey_string*)this; }

ey_string &eyscalar(ey_base* x)
{  return x->is_scalar ? *((ey_string*)x) : eys_empty; }
ey_string &eyscalar(eycit x) // Get a string from an eYAML chronological iterator
{  return x->value->is_scalar ? *((ey_string*)x->value) : eys_empty; }
ey_string &eyscalar(eyit x)   
{ ey_base *const xx = x->second; return xx->is_scalar ? *((ey_string*)xx): eys_empty; }

#include <cstdlib>
ey_string::operator string&() { return value; }
char ey_string::toByte()      { return atoi(value.c_str()); }
double ey_string::toDouble()  { return atof(value.c_str()); }
int ey_string::toInt()        { cout << "TOINT: " << value << endl; return atoi(value.c_str()); }
long long ey_string::toLong() { return atoll(value.c_str()); }
string ey_string::toString()
{
  if (value[0] != '"')
    return value;
  string ret = value.substr(1);
  for (size_t i = 0; i < ret.length(); i++)
  {
    if (ret[i] == '\\')
    switch (ret[i+1])
    {
      case 'r':
          ret[i] = '\r';
        goto accepted_escape;
      case 'n':
          ret[i] = '\n';
        goto accepted_escape;
      case 't':
          ret[i] = '\t';
        goto accepted_escape;
      case '"':
          ret[i] = '\"';
        goto accepted_escape;
      case '\'':
          ret[i] = '\'';
        goto accepted_escape;
      case '#':
          ret[i] = '#';
        goto accepted_escape;
      case '%':
          ret[i] = '%';
        goto accepted_escape;
      case '\\':
        goto accepted_escape;
        
      accepted_escape:
        ret.erase(i+1,1);
    }
    else if (ret[i] == '"') {
      ret.erase(i); break;
    }
  }
  return ret;
}

struct y_level {
  y_level *prev;
  ey_data *s; int i;
  y_level(y_level* a, ey_data *b, int c): prev(a), s(b), i(c) {}
};

ey_data parse_eyaml(istream &file, string filename)
{
  ey_data res;
  string line, unlowered;
  int linenum = 1, multi = false; // Line number for error reporting, multiline representing not in multi (false), starting multi (-1), or in multi (multiple line count)
  char mchar = 0; // The character that started a multiline entry
  
  getline(file,line);
  if (tolower(line.substr(0,7)) != "%e-yaml")
    return res;
  
  if (filename == "") filename = "(unspecified)";
  
  y_level *cur = new y_level(NULL,&res,0);
  eycit latestc = &cur->s->values_order;
  eyit latest = cur->s->values.end();
  
  continue_2:
  while (!file.eof())
  {
    getline(file,line); linenum++;
    if (line.substr(0,3) == "---") continue;
    int inds = 0; pt pos = 0;
    
    while (pos < line.length() and is_useless(line[pos])) pos++, inds++;
    
    if (multi != 0)
    {
      if (multi == -1)
        multi = inds;
      else if (inds < multi)
        multi = 0;
      if (multi) {
        string &str = ((ey_string*)latest->second)->value;
        str == "" ? str = line.substr(multi) : str += "\n" + line.substr(multi);
        continue;
      }
    }
    
    if (!(pos < line.length()) or line[pos] == '#' or line[pos] == '%')
      continue;
    
    if (line[pos] == '-')
      while (++pos < line.length() and is_useless(line[pos]));
    
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
              cout << "Indent increased unexpectedly on line " << linenum << " of file " << filename << endl;
          else
          {
            cur->s->values_order_last = latestc;
            cur = new y_level(cur, (ey_data*)(latest->second = latestc->value = new ey_data(unlowered)), inds);
            latest = cur->s->values.insert(eypair(tolower(nname), NULL)) . first; // We're only interested in the iterator; if a value exists, it's referenced in the linear area anyway.
            latestc = new ey_data::eylist(&cur->s->values_order);
            latest->second = NULL; // We do want to keep this open, thogh.
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
        latest->second = NULL; // We do want to keep this open, thogh.
      }
    }
    else // We are at the same indentation as before, and so we will simply add an item to this current scope
    {
      if (latest != cur->s->values.end() and latest->second == NULL)
        latest->second = latestc->value =  new ey_string(unlowered,""); 
      latest = cur->s->values.insert(eypair(tolower(nname), NULL)) . first;
      latestc = new ey_data::eylist(latestc);
      latest->second = NULL; // We do want to keep this open, thogh.
    }
    
    unlowered = nname;
    if (line[pos] != ':')
      continue;
    
    /* Get the value
    *********************/
    
    while (is_useless(line[++pos])); // Skip the whitespace between colon and value
    
    const pt vsp = pos; // Store value start position
    if (line[pos] == '"')
    {
      while (++pos < line.length() and line[pos] != '"')
        if (line[pos] == '"') pos++;
    }
    while (pos < line.length() and line[pos] != '#' and line[pos] != '%') pos++; // Find end of line (or start of comment)
    while (is_useless(line[--pos])); // Trim trailing whitespace
    
    if (++pos > vsp) // If we have any non-white value after this colon at all...
      if (pos - vsp == 1 and (line[vsp] == '|' or line[vsp] == '>')) // Pipe => Multiline value
        latest->second = latestc->value = new ey_string(unlowered, ""), // Store this value as a string
        multi = -1, mchar = line[pos]; // Indicate that we are starting a multiline value, and note the character invoking it
      else // Otherwise, just an ordinary scalar
        latest->second = latestc->value = new ey_string(unlowered, line.substr(vsp, pos - vsp)); // Store this value as a string
    else;
  }
  if (latest != cur->s->values.end() and latest->second == NULL)
    latest->second = latestc->value =  new ey_string(unlowered,"");
  
  while (cur) { // Clean up
    y_level *const curd = cur; cur = cur->prev;
    delete curd;
  }
  
  return res;
}

#include <sstream>
ey_data parse_eyaml_str(string yaml)
{
  istringstream iss(yaml);
  return parse_eyaml(iss,"LGM Settings");
}
