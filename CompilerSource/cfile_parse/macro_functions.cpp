/*********************************************************************************\
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
\*********************************************************************************/

#include <string>
using namespace std;
#include "../general/parse_basics.h"
#include "../general/darray.h"
string tostring(int x);

string escaped_string(string x)
{
  string res = '"' + x;
  for (unsigned i = 0; i < res.length(); i++)
  if (res[i] == '"' or res[i] == '\\') res.insert(i,"\\");
  return res + '"';
}

bool macro_function_parse(string cfile, unsigned int& pos,string& macrostr, varray<string> &args, const unsigned numparams)
{
  while (is_useless(cfile[pos])) pos++; 
  if (cfile[pos] != '(') { macrostr = "Expected parameters to macro function"; return false; }
  pos++;

  varray<string> macro_args;
  unsigned int args_given = 0;
  const unsigned len = cfile.length();

  unsigned lvl = 1;
  for (unsigned i = 0; i < numparams; i++) //parse out each parameter value into an array
  {
    const unsigned spos = pos;
    while (is_useless(cfile[pos])) pos++;
    while ((cfile[pos] != ',' or lvl > 1) and pos < len and lvl)
    {      if (cfile[pos] == '\"') { pos++; while (cfile[pos] != '\"' and pos < len) { if (cfile[pos] == '\\') pos++; pos++; } } 
      else if (cfile[pos] == '\'') { pos++; while (cfile[pos] != '\'' and pos < len) { if (cfile[pos] == '\\') pos++; pos++; } } 
      else if (cfile[pos] == '(') lvl++; else if (cfile[pos] == ')') lvl--; 
      pos++; 
    }
    macro_args[args_given++] = cfile.substr(spos,pos-spos);
  }

  while (is_useless(cfile[pos])) pos++; 
  if (lvl) { macrostr = "Expected closing parenthesis for macro function"; return false; }

  if (args_given != numparams)
  {
    macrostr = "Macro function requires " + tostring(numparams) + " parameters, passed " + tostring(args_given);
    return false;
  }
  
  for (unsigned i = 0; i < macrostr.length(); i++) //unload the array of values we created before into the macro's defiens
  {
    if ((is_letter(macrostr[i]) and !is_digit(macrostr[i-1])) or macrostr[i] == '#')
    {
      const unsigned si = i;
      
      //Be on the lookout for such homosexualities as # and ##
      //To be ISO compliant, add a space between everything that doesn't have a ## in it
      bool skipspace = 0, stringify = 0;
      if (macrostr[i] == '#')
      {
        if (macrostr[i+1] == '#')
          pos++, skipspace = true;
        else stringify = true;
        
        pos++;
        if (!is_letter(macrostr[i]))
          continue;
      }
      
      pos++; //Guaranteed letter here
      while (is_letterd(macrostr[i])) i++;
      const string sstr = macrostr.substr(si,i-si);
      
      for (unsigned ii = 0; ii < numparams; ii++)
        if (args[ii] ==  sstr)
          macrostr.replace(si,i-si,stringify?escaped_string(macro_args[ii]):skipspace?macro_args[ii]:macro_args[ii]+" ");
    }
  }
  
  return true;
}
