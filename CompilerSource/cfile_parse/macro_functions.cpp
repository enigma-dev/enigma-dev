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

#include <iostream>
using namespace std;
#include "../general/parse_basics.h"
#include "../general/darray.h"
#include "cparse_shared.h"

string escaped_string(string x)
{
  string res = '"' + x;
  for (unsigned i = 1; i < res.length(); i++)
    if (res[i] == '"' or res[i] == '\\') { 
      res.insert(i,"\\"); i++; }
  return res + '"';
}

string stripspace(string x)
{
  size_t i;
  for (i=0; is_useless(x[i]); i++);
  x.erase(0,i);
  return x;
}

bool is_entirely_white(string x)
{
  for (size_t i=0; i<x.length(); i++)
    if (!is_useless(x[i])) return 0;
  return 1;
}

#include "macro_functions.h"

//This function has its own recursion stack.
//It is only called when a parameter is passed to a macro function.
bool preprocess_separately(string &macs)
{
  bool stringify = false;
  unsigned int macrod = 0;
  varray<string> inmacros;
  //cout << "parse " << macs << endl;
  pt i = 0; 
  while (i < macs.length())
  {
    if (is_digit(macs[i]))
    {
      const unsigned is = i;
      while (is_letterd(macs[++i]));
      if (stringify)
      {
        stringify = false;
        const string os = macs.substr(is,i-is);
        const size_t osl = os.length();
        string ns = escaped_string(os);
        macs.replace(is,i-is,ns);
        i += ns.length() - osl;
      }
      i++; continue;
    }
    if (is_letter(macs[i]))
    {
      const pt si = i;
      while (is_letterd(macs[++i]));
      string n = macs.substr(si,i-si);
      
      maciter t;
      if ((t=macros.find(n)) != macros.end())
      {
        if (stringify)
        {
          stringify = false;
          macs.replace(si,i-si,escaped_string(n));
          i++; continue;
        }
        
        bool recurs=0;
        for (unsigned int iii=0;iii<macrod;iii++)
           if (inmacros[iii]==n) { recurs=1; break; }
        if (!recurs)
        {
          string macrostr = t->second;
          
          if (t->second.argc != -1) //Expect ()
          {
            if (!macro_function_parse(macs.c_str(),macs.length(),n,i,macrostr,t->second.args,t->second.argc,t->second.args_uat)) {
              cferr = macrostr;
              return false;
            }
          }
          
          macs.replace(si,i-si,macrostr);
          inmacros[macrod++] = n;
          i = si; continue;
        }
        //else puts("Recursing macro. This may be a problem.\r\n");
      }
      i++; continue;
    }
    if (macs[i] == '#')
    {
      if (macs[i+1] == '#')
      {
        if (stringify)
          return false;
        pt ib = i;
        while (is_useless(macs[--ib])); ib++;
        i++;  while (is_useless(macs[++i]));
        
        macs.erase(ib,i-ib);
        i = ib;
      }
      else //Stringify
      {
        stringify = true; //Notice the parallel in structure to the below function. No, not "redundant." The word is "parallel." <_<
        macs.erase(i,1);
      }
      i++; continue;
    }
    i++; continue;
  }
  //cout << " >> " << macs << endl;
  return true;
}

bool macro_function_parse(const char* cfile,const size_t len,string macroname,pt &pos,string& macrostr, varray<string> &args, const int numparams, const int au_at, bool cppcomments, bool gmlbrackets)
{
  //cout << "parse " << macrostr << endl;
  //Skip comments. Ignore this block; it's savage but efficient.
  //Basically, I don't trust the compiler to correctly unroll a large conditional of shared parts.
    pos--; do you_know_you_love_this_block: if (cfile[++pos] == '/') {
    if (cfile[++pos] == '/') {
      pos += cppcomments;
      while (cfile[++pos] and cfile[pos] != '\n' and cfile[pos] != '\r'); goto you_know_you_love_this_block; //continue is a bad idea here
    } else if (cfile[pos] == '*') {
      pos += cppcomments;
      while (cfile[pos] and (cfile[pos++] != '*' or cfile[pos] != '/')); goto you_know_you_love_this_block;
    } } while (is_useless(cfile[pos]));
  //You can resume reading the code
  
  if (cfile[pos] != '(') { macrostr = macroname; return true; } //"Expected parameters to macro function"; return false; }
  pos++;
  
  //cout << endl << endl << endl << endl << endl << endl << "Raw: \r\n" << macrostr;
  
  int args_given = 0;
  varray<string> macro_args;
  
  unsigned lvl = 1;
  for (int i = 0; i < numparams or lvl > 0; i++) //parse out each parameter value into an array
  {
    if (pos >= len) {
      macrostr = "Macro function parameters unterminated: Unmatched parenthesis";
      return false;
    }
    if (i > numparams)
      return macrostr = "Expected closing parenthesis for macro function at this point: too many parameters", false;
    if (lvl <= 0)
      return macrostr = "Expected additional macro parameters before end parenthesis: function requires " + tostring(numparams) + " parameters, passed only " + tostring(args_given), false;
    const pt spos = pos;
    
    // Skip whitespace after opening parenthesis
    while (is_useless(cfile[pos])) pos++; // Now we are at first argument or closing parenthesis
    
    while (pos < len and (lvl > 1 or (cfile[pos] != ',' or args_given == au_at)) and lvl)
    {
      if (cfile[pos] == ')') lvl--;
      else if (cfile[pos] == '(') lvl++;
      else if (cfile[pos] == '[') lvl+=gmlbrackets;
      else if (cfile[pos] == ']') lvl-=gmlbrackets;
      else if (cfile[pos] == '\"') { pos++; while (cfile[pos] != '\"' and pos < len) { if (cfile[pos] == '\\') pos++; pos++; } } 
      else if (cfile[pos] == '\'') { pos++; while (cfile[pos] != '\'' and pos < len) { if (cfile[pos] == '\\') pos++; pos++; } }  
      if (lvl) pos++; 
    }
    //Comma drops out as soon as cfile[pos] == ','
    //End Parenth will not increment if !lvl, so cfile[pos] == ')'
    const string rw(cfile,spos,pos-spos);
    if (args_given or cfile[pos] != ')' or !is_entirely_white(rw))
      macro_args[args_given++] = rw;
    pos++;
  }
  
  if (args_given != numparams)
  {
    if (au_at == -1 or args_given <= au_at) {
      macrostr = "Macro function requires " + tostring(numparams) + " parameters, passed only " + tostring(args_given);
      return false;
    }
  }
  
  //cout << "Params: "; for (int i=0; i<args_given; i++) cout << macro_args[i] << ", "; cout << "end.";
  
  bool stringify = 0, concat = 0;
  
  for (pt i = 0; i < macrostr.length(); ) //unload the array of values we created before into the macro's definiens
  {
    if (is_digit(macrostr[i]))
    {
      const pt is = i;
      while (is_letterd(macrostr[++i]));
      if (stringify) {
        macrostr.replace(is,i,"\""+macrostr.substr(is,i)+"\"");
        stringify = false;
      }
      concat = false;
      continue;
    }
    if (is_letter(macrostr[i]))
    {
      const pt si = i;
      
      i++; //Guaranteed letter here, so incrememnt
      while (i < macrostr.length() and is_letterd(macrostr[i])) i++;
      string sstr = macrostr.substr(si,i-si);
      
      for (int ii = 0; ii < numparams; ii++)
        if (args[ii] ==  sstr)
        {
          string iinto = macro_args[ii];
          if (stringify)
            macrostr.replace(si,i-si,escaped_string(iinto));
          else
          {
            if (!concat)
              if (!preprocess_separately(iinto))
                return false;
            macrostr.replace(si,i-si,iinto);
          }
              
          i = si + iinto.length();
          break;
        }
      concat = false;
      stringify = false;
    }
    //Be on the lookout for such homosexualities as # and ##
    //To be ISO compliant, add a space between everything that doesn't have a ## in it
    else if (macrostr[i] == '#')
    {
      if (macrostr[i+1] == '#')
      {
        pt end = i+2;
        while (i > 0 and is_useless(macrostr[--i])); i++;
        while (end < macrostr.length() and is_useless(macrostr[end]))
          if (macrostr[end++] == '\\') end++;
        macrostr.erase(i,end-i);
        concat = true;
      }
      else
      {
        stringify = true;
        macrostr[i] = ' ';
        i++;
      }
    }
    else i++;
  }
  //cout << endl << endl << endl << macrostr << endl << endl << endl << endl << endl << endl;
  //cout << " >> " << macrostr << endl;
  return true;
}
