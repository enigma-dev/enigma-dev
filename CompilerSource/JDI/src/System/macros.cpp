/**
 * @file  macros.cpp
 * @brief System source implementing functions for parsing macros.
 * 
 * In general, the implementation is unremarkable.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <map>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

#include "macros.h"
#include <General/parse_basics.h>
#include <General/debug_macros.h>

using namespace jdip;
using namespace parse_bacics::visible;

macro_type::macro_type(string n, int ac): argc(ac), refc(1), name(n) {}
macro_type::~macro_type() {}

macro_scalar::macro_scalar(string n, string val): macro_type(n,-1), value(val) {}
macro_scalar::~macro_scalar() {}

macro_function::macro_function(string n, string val): macro_type(n,0), value(), args() {
  const size_t bs = val.length();
  if (bs) {
    char *buf = new char[bs]; memcpy(buf, val.c_str(), bs);
    value.push_back(mv_chunk(buf, bs));
  }
}
macro_function::macro_function(string n, const vector<string> &arglist, string val, bool variadic, error_handler *herr): macro_type(n,arglist.size()+variadic), value(), args(arglist) {
  preparse(val, herr);
}
macro_function::~macro_function() {
  for (size_t i = 0; i < value.size(); ++i)
    if (!value[i].is_arg) delete []value[i].data;
}

void macro_type::free(const macro_type* whom) {
  if (!-- whom->refc){
    if (whom->argc >= 0) delete (macro_function*)whom;
    else delete (macro_scalar*)whom;
  }
}

bool macro_type::is_variadic() const {
  return argc > 0 && argc > ((macro_function*) this)->args.size();
}


//======================================================================================================
//=====: Macro function data chunk constructors :=======================================================
//======================================================================================================

macro_function::mv_chunk::mv_chunk(const char* str, size_t start, size_t len): metric(len), is_arg(false) {
  data = new char[len];
  memcpy(data, str+start, len);
}
macro_function::mv_chunk::mv_chunk(char* ndata, size_t len): data(ndata), metric(len), is_arg(false) {}
macro_function::mv_chunk::mv_chunk(size_t argnum): data(NULL), metric(argnum), is_arg(true) {}

string macro_function::mv_chunk::toString(macro_function *mf) {
  if (is_arg) return "{{" + mf->args[metric] + "}}";
  return string(data,metric);
}

//======================================================================================================
//=====: Macro function parsing methods :===============================================================
//======================================================================================================

/**
  @section Implementation
  This function actually hauls quite a bit of ass. Its purpose is to make sure that
  when the parse is done, evaluating a macro function down the road is as simple as
  imploding the vector with a lookup on each element before concatenation.
  
  Since the function value parameter is read in raw, the function must handle both
  comments and strings, as well as, according to the expanded-value specification
  in \c jdip::macro_function::value, handle the # and ## operators.
**/
inline void jdip::macro_function::preparse(string val, error_handler *herr)
{
  unsigned push_from = 0;
  map<string,int> parameters;
  
  value.clear(); // Wipe anything in the output array
  
  for (pt i = 0; i < args.size(); i++) // Load parameters into map for searching and fetching index
    parameters[args[i]] = i;
  
  for (pt i = 0; i < val.length(); ) // Iterate the string
  {
    if (val[i] == '\"' or val[i] == '\'') { // Skip strings
      char nc = val[i];
      while (++i < val.length() and val[i] != nc) // We're ultimately searching for the matching quote.
        if (val[i] == '\\') ++i; // No need to handle escape sequences; only '\\' and '\"' are of concern.
      continue;
    }
    
    if (val[i] == '/') { // Unfortunately, we must also do comment checking.
      if (val[i+1] == '*') { i += 3; // This is a hack to save lines on this trivialty
        while (i < val.length() and (val[i] != '/' or val[i-1] != '*')) ++i;
      } else if (val[i+1] == '/')
        while (++i < val.length() and val[i] != '\n');        
    }
    
    if (is_digit(val[i])) { // We must also be sure to skip numbers,
      while (is_digit(val[++i])); // As they could have postfixes matching a parameter
      continue;
    }
    
    if (is_letter(val[i])) // If we do encounter a letter, the fun begins.
    {
      const unsigned sp = i; // We record where we're leaving off
      while (is_letterd(val[++i])); // And navigate to the end of the identifier.
      map<string,int>::iterator pi = parameters.find(val.substr(sp,i-sp));
      if (pi != parameters.end()) // Then check if it's a parameter.
      {
        if (sp > push_from) // If we've covered any ground since our last value push,
          value.push_back(mv_chunk(val.c_str(), push_from, sp-push_from)); // Push it onto our value
        value.push_back(mv_chunk(pi->second)); // Push this identifier as a distinct item onto value
        push_from = i; // Indicate the new starting position of data to be pushed
      }
      continue;
    }
    
    if (val[i] == '#')
    {
      if (val[++i] == '#') {
        if (i <= 1) continue;
        pt ie = i - 2;
        while (ie > 0 and is_useless(val[ie])) --ie; // eliminate any leading whitespace
        if (ie > push_from) // If there's anything non-white to push since last time,
          value.push_back(mv_chunk(val.c_str(), push_from, ie-push_from+1)); // Then push it
        while (is_useless(val[++i])); // Skip any trailing whitespace
        push_from = i;
        continue;
      }
      if (--i > push_from) // If we've covered any ground since our last value push,
        value.push_back(mv_chunk(val.c_str(), push_from, i-push_from)); // Push it onto our value
      while (is_useless_macros(val[++i]));
      push_from = i; // Store current position just in case something stupid happens
      if (!is_letter(val[i])) { herr->error("Expected parameter name following '#' token; `" + val + "'[" + ::toString(i) + "] is not a valid identifier character"); continue; }
      const size_t asi = i;
      while (is_letterd(val[++i]));
      map<string,int>::iterator pi = parameters.find(val.substr(asi,i-asi));
      if (pi == parameters.end()) { herr->error("Expected parameter name following '#' token: `" + val.substr(asi,i-asi) + "' does not name a parameter"); continue; }
      
      // Push our string junk
      value.push_back(mv_chunk("#", 0, 1));
      value.push_back(mv_chunk(pi->second));
      
      push_from = i; // Indicate the new starting position of data to be pushed
    }
    
    i++;
  }
  if (val.length() > push_from)
    value.push_back(mv_chunk(val.c_str(), push_from, val.length() - push_from));
}

string macro_type::toString() const {
  if (argc >= 0)
  {
    macro_function *mf = (macro_function*)this;
    string res = "#define " + name + "(";
    for (size_t i = 0; i < mf->args.size(); i++)
      res += mf->args[i] + (i+1 < mf->args.size() ? ", " : ((size_t)argc > mf->args.size()? "...": ""));
    res += ") \\\n";
    for (size_t i = 0; i < mf->value.size(); i++)
      res += "  " + mf->value[i].toString(mf) + (i+1 < mf->value.size()? "\\\n" : "");
    return res;
  }
  else {
    return "#define " + name + "\n  " + ((macro_scalar*)this)->value;
  }
}

string macro_type::valueString() const {
  if (argc >= 0) {
    macro_function *mf = (macro_function*)this;
    string res;
    for (size_t i = 0; i < mf->value.size(); i++)
      res += mf->value[i].toString(mf) + (i+1 < mf->value.size()? " " : "");
    return res;
  } else {
    return ((macro_scalar*)this)->value;
  }
}

#include <iostream>
#include <System/token.h>
bool macro_function::parse(vector<string> &arg_list, char* &dest, char* &destend, token_t errtok, error_handler *herr) const
{
  if (arg_list.size() < args.size()) {
    if (arg_list.size() + 1 < args.size())
      return errtok.report_error(herr, "Too few arguments to macro function `" + name + "': provided " + ::toString(arg_list.size()) + ", requested " + ::toString(args.size())), false;
    arg_list.push_back("");
  }
  else if ((arg_list.size() > args.size() and args.size() == (unsigned)argc))
    return errtok.report_error(herr, "Too many arguments to macro function `" + name + "'"), false;
  size_t alloc = 1;
  
  if (value.empty())
    return false;
  
  for (size_t i = 0; i < value.size(); i++) {
    if (value[i].is_arg)
      alloc += arg_list[value[i].metric].length();
    else {
      dbg_assert(value[i].metric > 0);
      if (*value[i].data == '#') {
        dbg_assert(value[i].metric == 1);
        alloc += (1 + arg_list[value[++i].metric].length()) << 1; // Make sure we have enough room for a string of nothing but newlines and backslashes
        continue;
      }
      alloc += value[i].metric;
    }
  }
  
  bool last_was_arg = false; // True if the previous chunk was an argument.
  char* buf = new char[alloc], *bufat = buf;
  for (size_t i = 0; i < value.size(); i++)
    if (value[i].is_arg)
    {
      if (last_was_arg) {
        while (bufat > buf and is_useless(*--bufat)); ++bufat;
        const string &ts = arg_list[value[i].metric];
        const char* argname = ts.c_str();
        size_t sz = ts.length();
        while (is_useless(*argname)) ++argname, --sz;
        memcpy(bufat, argname, sz);
        bufat += sz;
      }
      else {
        const string& argname = arg_list[value[i].metric];
        memcpy(bufat, argname.c_str(), argname.length());
        bufat += argname.length();
      }
      last_was_arg = true;
    }
    else {
      last_was_arg = false;
      dbg_assert(value[i].metric > 0);
      if (value[i].metric == 1 and *value[i].data == '#') {
        *bufat++ = '"';
        dbg_assert(value[i+1].is_arg /* This should be guaranteed by the preparser. */);
        for (const char* bi = arg_list[value[++i].metric].c_str(); *bi; bi++) {
          if (*bi == '\n') { *bufat++ = '\\', *bufat++ = 'n'; continue; }
          if (*bi == '\\') { *bufat++ = '\\', *bufat++ = '\\'; continue; }
          if (*bi == '\r') { *bufat++ = '\\', *bufat++ = 'r'; continue; }
          *bufat++ = *bi;
        }
        *bufat++ = '"';
        continue;
      }
      memcpy(bufat, value[i].data, value[i].metric);
      bufat += value[i].metric;
    }
  *bufat = 0;
  dest = buf, destend = bufat;
  return true;
}
