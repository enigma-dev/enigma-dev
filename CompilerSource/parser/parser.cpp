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

//Welcome to the ENIGMA EDL-to-C++ parser; just add semicolons.
//No, it's not really that simple.

/*/\*\
  As a fun fact, this parser was the first one I ever wrote, and was originally
  coded in GML. When it was complete, it parsed itself to C++.
  Since then, however, the entire parser has been recoded, mostly for concerns
  of efficiency. and legibility. Also, the original wasn't built with some of
  C++'s features in mind.

  It's also worth noting that this is my only parser which incorporates a lexer
  or conducts multiple passes. It uses what I call a syntax string, which some-
  one once told me was akin to a token stream. I don't really care; I made this
  up myself long ago without any help from external sources.

  At any rate, this means that this parser is really easy to follow as far as
  telling what's going on, as it can be divided into separate sections.
\*\/*/

#include <map> //Log lookup
#include <string> //Ease of use
#include <iostream> //Print shit
#include <vector> //Store case labels
#include <cstdlib> //stdout, fflush
#include <cstdio> //stdout, fflush
using namespace std; //More ease
#include "../externs/externs.h" //To interface with externally defined types and functions
#include "parser_components.h" //duh

#include "../general/darray.h"
#include "../general/parse_basics.h"
#include "object_storage.h"

#include "collect_variables.h"

#include "../settings.h"
#include "parser.h"


//This adds all keywords to a tree structure for quick lookup of their token.
void parser_init()
{
  //Single statements taking no parameters requiring semicolon/other dividing symbol before and then a semicolon directly after
  //Tokenized as 'b'
  edl_tokens["break"]     = 'b';
  edl_tokens["continue"]  = 'b';
  edl_tokens["default"]   = 'b';
  //Labels; anything followed by :, can be classified the same way: semicolon before, but colon after.
  edl_tokens["private"]   = 'b';
  edl_tokens["protected"] = 'b';
  edl_tokens["public"]    = 'b';

  /*These are type flags that don't really imply a type
  edl_tokens["signed"]   = 'd';
  edl_tokens["unsigned"] = 'd';
  edl_tokens["register"] = 'd';
  edl_tokens["static"]   = 'd';
  edl_tokens["const"]    = 'd';*/

  //Word ops: these aren't actually replaced, just tokenized.
  edl_tokens["and"] = '&';
  edl_tokens["or"]  = '|';
  edl_tokens["xor"] = '^';
  edl_tokens["not"] = '!';

  //These must be marked separately for easy retrieval
  //tokenized as 'C'
  edl_tokens["class"]     = 'C';
  edl_tokens["enum"]      = 'C';
  edl_tokens["namespace"] = 'C';
  edl_tokens["struct"]    = 'C';

  //These two are... Special.
  edl_tokens["do"]   = 'r';
  edl_tokens["else"] = 'e';

  //These must be passed a parameter in parentheses
  //Token is 's'
  edl_tokens["case"]   = 's';
  edl_tokens["catch"]  = 's';
  edl_tokens["if"]     = 's';
  edl_tokens["repeat"] = 's';
  edl_tokens["switch"] = 's';
  edl_tokens["until"]  = 's';
  edl_tokens["while"]  = 's';
  edl_tokens["with"]   = 's';
  //For gets an f
  edl_tokens["for"] = 'f';

  //Type flags
  //token is 't'
  edl_tokens["unsigned"] = 't';
  edl_tokens["long"]     = 't';
  edl_tokens["short"]    = 't';
  edl_tokens["signed"]   = 't';
  edl_tokens["register"] = 't';
  edl_tokens["unsigned"] = 't';
  edl_tokens["const"]    = 't';
  edl_tokens["volatile"] = 't';
  edl_tokens["static"]   = 't';

  //These are all templates, which can later be revised to 'd' when passed a parameter in <>
  //token is also 't'
  edl_tokens["const_cast"]       = 't';
  edl_tokens["dynamic_cast"]     = 't';
  edl_tokens["reinterpret_cast"] = 't';
  edl_tokens["static_cast"]      = 't';
  edl_tokens["template"]         = 'X'; //template is disabled

  //Local and Global are special.
  edl_tokens["local"] = 'L';  // Each of these has a dual meaning. They can either change the scope
  edl_tokens["global"] = 'L'; // of the declaration that follows, or just serve as a negative constant

  //These each require a parameter of some sort, so should not be given a semicolon right after
  //Token is 'p'
  edl_tokens["delete"] = 'p';
  edl_tokens["goto"]   = 'p';
  edl_tokens["return"] = 'p';
  edl_tokens["throw"]  = 'p';

  //Token is 'a'
  edl_tokens["new"] = 'a';

  //Classify this as O then replace O() with nnn and then O(.*)\( with nn...n
  edl_tokens["operator"] = 'o';
}



string parser_main(string code, parsed_event* pev)
{
  //Converting EDL to C++ is still relatively simple.
  //It can be done, for the most part, using only find and replace.

  //For the sake of efficiency, however, we will reduce the number of passes by replacing multiple things at once.

  string synt;

  //Reset things
    //Nothing to reset :trollface:

  //Initialize us a spot in the global scope
  initscope("script scope");

  if (pev) {
    pev->strc = 0; //Number of strings in this code
    parser_ready_input(code,synt,pev->strc,pev->strs);
  }
  else
  {
    varray<string> strst;
    unsigned int strct = 0;
    parser_ready_input(code,synt,strct,strst);
  }

  parser_reinterpret(code,synt);

  parser_add_semicolons(code,synt);

  //cout << synt << endl;
  //cout << code << endl;

  if (pev) { cout << "collecting variables..."; fflush(stdout);
    collect_variables(code,synt,pev); cout << " done>"; fflush(stdout);
  }

  return code;
}

#include "../cfile_parse/type_resolver.h"

typedef map<string,dectrip> localscope;
pt move_to_beginning(string& code, string& synt, pt pos)
{
  bool lparenth = false; // true if the last symbol set skipped from this block was a set of brackets
  backloop:
  while (synt[pos] == ']' or synt[pos] == ')') // Seek to beginning of array subscripts and function args
  {
    lparenth = synt[pos] == ')';
    if (synt[pos] == ']' or synt[pos] == ')')
    {
      for (int cnt = (pos--, 1); cnt; pos--)
      {
        if (synt[pos] == '[' or synt[pos] == '(') cnt--;
        else if (synt[pos] == ')' or synt[pos] == ']') cnt++;
        if (!pos) goto hell; //Break 2;
      }
    }
  } // at beginning of function args/array subscripts
  if (lparenth and synt[pos] != 'n')
    return pos + 1;
  for (const char c = synt[pos--]; synt[pos] == c; pos--)
    if (!pos) goto hell;
  pos++;

  hell:
  if (pos) {
    if (synt[pos-1] == '.')
      { pos-=2; goto backloop; }
    if (synt[pos-1] == '>' and pos - 1 and synt[pos-2] == '-')
      { pos -= 3; goto backloop; }
    if (synt[pos-1] == ':' and synt[pos-2] == ':')
      { pos -= 3; goto backloop; }
  }

  return pos;
}

pt end_of_brackets(const string& synt, pt pos) // Given a string and the index OF the first bracket, find the matching right bracket.
{
  for (int lvl=0; synt[pos]; pos++)
  {
    if (synt[pos] == '[') lvl++;
    if (synt[pos] == '(') lvl++;
    if (synt[pos] == ')') if (!--lvl) return pos;
    if (synt[pos] == ']') if (!--lvl) return pos;
  }
  return pos;
}

extern externs *enigma_type__var, *enigma_type__variant;

// A little structure to hold detatched cases.
// Code and synt have their own, identical lengths; they are the expression following the `case` token.
// pos and len are the position and length of the entire case label, for use when replacing the code.
struct lexpair {
  string code, synt, mylabel, mylsynt; pt pos, len; int stri, strc;
  lexpair(string c, string s, pt p, pt l, int si, int sc):
    code(c), synt(s), mylabel(), mylsynt(), pos(p), len(l), stri(si), strc(sc) {}
};
bool is_integer(const lexpair& lp) {
  for (pt i = 0; i < lp.code.length(); i++)
    if (lp.synt[i] != '0' or lp.code[i] == '.') return 0;
  return 1;
}
bool is_float(const lexpair& lp) {
  for (pt i = 0; i < lp.code.length(); i++)
    if (lp.synt[i] != '0') return 0;
  return 1;
}
bool is_literal(const lexpair& lp) {
  for (pt i = 0; i < lp.code.length(); i++)
    if (lp.synt[i] != '0') {
      if (i or (lp.synt[0] != '"' and lp.synt[0] != '\''))
        return 0;
      for (; i < lp.code.length(); i++)
        if (lp.synt[i] != '"' and lp.synt[i] != '\'')
          return 0;
      break;
    }
  return 1;
}
int make_hash(const lexpair& lp, parsed_event* pev) {
  if (!is_literal(lp)) return -1;
  if (is_integer(lp)) return atol(lp.code.c_str());
  if (is_float(lp)) return int(atof(lp.code.c_str()) * 65536);

  // Now we assume it's a string and apply a simple hash
  int r = 0;
  for (int i = 0; i < lp.strc; i++) {
    string a = pev->strs[lp.stri + i];
    if (a.length() > 1) {
      if (a[0] == '"' and a[a.length() - 1] == '"') a = a.substr(1,a.length()-2);
      else if (a[0] == '\'' and a[a.length() - 1] == '\'') a = a.substr(1,a.length()-2);
    }
    for (pt ii = 0; ii < a.length(); ii++)
      r = 31*r + a[ii];
  }
  return r;
}

int parser_secondary(string& code, string& synt,parsed_object* glob,parsed_object* obj,parsed_event* pev)
{
  // We'll have to again keep track of temporaries
  // Fortunately, this time, there are no context-dependent tokens to resolve

  int slev = 0;
  darray<localscope*> sstack;
  sstack[slev] = new localscope();

  bool indecl = 0, deceq = 0;
  string dtype, dname, dpre, dsuf;
  int inbrack = 0, level = 0;
  bool rhs = false;
  int infor = 0;

  for (pt pos = 0; pos < synt.length(); pos++)
  {
    if (synt[pos] == 't')
    {
      pt spos = pos;
      while (synt[++pos] == 't'); // Move to end of type
      pos += parser_fix_templates(code,pos,spos,&synt); // This will default template parameters for you and return the change in end position

      if (rhs or indecl or synt[pos] == '(') // It's not a declaration if we're on the right of an =, already in a declaration, or are a cast (followed by a parenthesis)
        { pos--; continue; }

      indecl = true; dtype = code.substr(spos,pos-spos);
      dpre = dsuf = "";
      cout << "TYPE [" << dtype << "]" << endl;
      pos--; continue;
    }
    // Handle parsing in dot access (add calls to dot-access routines)
    else if (synt[pos] == '.' and synt[pos+1] == 'n')
    {
      const pt epos = pos--;
      pos = move_to_beginning(code,synt,pos);

      string exp = code.substr(pos,epos-pos), // Copy the expression being operated upon by .
         expsynt = synt.substr(pos,epos-pos); // Copy the lex of it, too
      const pt ebp = pos; // Expression's beginning position, not a register.
      pt ep = pos = epos; // Restore our original ending position.

      // Determine the member being accessed
      while (synt[++ep] == 'n');
      string member = code.substr(epos+1,ep-epos-1);

      // Determine the type of the left-hand expression
      cout << "GET TYPE OF " << exp << endl;
      onode n = exp_typeof(exp,sstack.where,slev+1,glob,obj);
      if (n.type == NULL) n.type = builtin_type__int;
      externs* ct = n.type;
      bool tf = (ct->members.find(member) != ct->members.end());
      while (!tf and ct->flags & EXTFLAG_TYPEDEF and ct->type)
        ct = ct->type, tf = (ct->members.find(member) != ct->members.end());
      if (!tf and find_in_all_ancestors_generic(ct,member))
        ct = ext_retriever_var->parent, tf = true;

      cout << exp << ": " << n.type->name << " :: " << member << " => " << tf << " _ " << level << endl;

      if (!tf) // No member by this name can be accessed
      {
        string repstr;
        string repsyn;
        if (shared_object_locals.find(member) != shared_object_locals.end())
        {
          repstr = "enigma::glaccess(int("   + exp +   "))->" + member;
          repsyn = "nnnnnnnnnnnnnnnn(ccc(" + expsynt + "))->" + string(member.length(),'n');
        }
        else
        {
          repstr = "enigma::varaccess_";
          repsyn = "nnnnnnnnnnnnnnnnnn";

          repstr += member;
          repsyn += string(member.length(),'n');

          repstr += "(int(";
          repsyn += "(ccc(";

          repstr += exp;
          repsyn += expsynt;

          repstr += "))";
          repsyn += "))";

          add_dot_accessed_local(member);
        }
        code.replace(ebp, exp.length() + 1 + member.length(), repstr);
        synt.replace(ebp, exp.length() + 1 + member.length(), repsyn);
        pos = ebp + repstr.length() - 1;
      }
      else // There is a member by this name in the type of that expression
      {
        if (n.pad > 0 or n.deref) { // The type is a pointer. This dot should be a ->
          code.replace(epos,1,"->");
          synt.replace(epos,1,"->");
          pos = epos + 1;
        }
      }
      cout << "New level: " << level << endl << "code from here: " << code.substr(pos) << endl;
      continue;
    }
    else if (synt[pos] == '[' and (!indecl or deceq))
    {
      const pt sp = move_to_beginning(code,synt,pos-1);
      const string exp = code.substr(sp,pos-sp);
      cout << "GET TYPE2 OF " << exp << endl;
      onode n = exp_typeof(exp,sstack.where,slev+1,glob,obj);
      if (n.type == enigma_type__var and !n.pad and !n.deref)
      {
        pt cp = pos;
        code[cp++] = '(';
        for (int cnt = 1; cnt; cp++)
          if (synt[cp] == '[') cnt++;
          else if (synt[cp] == ']') cnt--;
        if (synt[--cp] == ']')
          code[cp] = ')';
      }
      else if (n.pad or n.deref) // Regardless of type, as long as we have some kind of pointer to be dereferenced
      {
        const pt ep = end_of_brackets(synt,pos); // Get position of closing ']'
        code.insert(ep, 1, ')');
        synt.insert(ep, 1, ')');
        pos++; // Move after the '['
        code.insert(pos, "int(");
        synt.insert(pos, "ccc(");
      }
      level++;
    }
    else switch (synt[pos])
    {
      case '*':
        if (inbrack or deceq or !indecl)
          goto Ass;
        dpre += '*';
        break;
      case '[':
        if (!inbrack and !deceq)
          dpre += "*", inbrack++; // Just increment the ref count; Knowing how many [] there are is unnecessary.
        level++;
        break;
      case ']':
        if (inbrack)
          inbrack--;
        level--;
        break;


      case ';':
        if (level and !infor)
          cout << "ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! "
                  "ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! "
                  "ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! ERROR! "
                  "ERROR! ERROR! ERROR! THIS SHOULD NOT HAVE FUCKING HAPPENED! DAMN IT! SOMETHING IS REALLY WRONG WITH THE FOUNDATION OF "
                  "THIS SYSTEM! Or, I mean, maybe I just skipped a small block of text; point is, let me know this happened, please. Level = " << level << endl;
        if (infor) infor--;
        rhs = indecl = false;
      case ',':
        if (indecl and !inbrack)
        {
          (*sstack[slev])[dname] = dectrip(dtype,dpre,dsuf);
          cout << "DECLARE " << dtype << " " << dpre << dname << dsuf << endl;
          deceq = false;
        }
        rhs &= (level>0);
        break;
      case '=':
        deceq |= indecl;
        if (setting::use_gml_equals and (level or rhs))
        {
          if (infor != 3 and infor != 1 and synt[++pos] != '=')
            code.insert(pos,"="),
            synt.insert(pos,"=");
        }
        rhs |= !level;
        break;
      case 'n':
          if (!inbrack and !deceq) {
            const pt sp = pos;
            while (synt[++pos] == 'n');
            dname = code.substr(sp,pos-- - sp);
            break;
          }
          while (synt[++pos] == 'n');
          pos--; break;

      case '(': level++; break;
      case ')':
          if (level == 1 and infor == 1)
            infor = 0;
          if (!level) cerr << "SUPER FUCKING ERROR ALERT: UNMATCHED PARENTHESIS READ AT POS=" << pos << " [snippet `" << code.substr(pos-10,20).insert(10,"<<>>") << "`]" << endl;
          level--;
        break;
      case ':':
        if (synt[pos + 1] == '=') // EXPLICIT assign
        {
          code.erase(pos,1);
          synt.erase(pos,1);
          deceq |= indecl;
          rhs |= !level;
        }
        break;


      case '!':
          goto notAss;
      case '>': case '<':
        if (synt[pos-1] != synt[pos]) // Handles <=, >=, which are not assignment operators!
          goto notAss;
        goto Ass;
      
      case '+': case '-': 
          if (synt[pos] == synt[pos+1])
          { pos++; continue; }
      case '&': case '|': case '^': case '~': case '/':
          Ass: // Assignment operator
          if (synt[pos+1] == '=') {
            deceq |= indecl;
            rhs |= !level;
            pos++; break;
          }
          notAss: //Not an assignment operator; !=, >=, <=
            if (synt[pos+1] == '=')
              pos++; // Already know it isn't an assignment; already handled.
        break;

      case 'f': // We need to be aware of for loops.
        infor = 3;
      default:
        while (synt[pos] == synt[pos+1]) pos++;
    }
  }

  // Handle switch statements. Badly.
  if (pev) // We need to know this to deal with string hashes
  {
    static int switch_count = 0;
    int string_index = 0; // Number of strings before this statement
    for (pt pos = 0; pos < synt.length(); pos++)
    {
      if (synt[pos] != 's') {
        string_index += synt[pos] == '"' or synt[pos] == '\'';
        continue;
      }

      const pt switch_start_pos = pos;

      while (synt[++pos] == 's');
      string ss(code,switch_start_pos,pos-switch_start_pos);
      if (ss != "switch")
        continue;

      int strs_this_statement = 0;
      const pt switch_value_spos = pos;

      while (synt[pos++] != '{') { // We can skip the first char, because at this juncture, it's garanteed to be an opening parenthesis.
        string_index += synt[pos] == '"' or synt[pos] == '\''; // Increment the overall index here; we'll be skipping these strings later on.
      }
      const string
        svalue   (code, switch_value_spos, pos - switch_value_spos - 1),
        svaluelex(synt, switch_value_spos, pos - switch_value_spos - 1);

      // Some crap we need to know
      int level = 1;
      vector<lexpair> cases;
      pt default_start_pos = 0; // 0 for no default, nonzero indicates position

      while (level)
      {
        if (synt[pos] == '{')
        { level++; pos++; continue; }
        if (synt[pos] == '}')
        { level--; pos++; continue; }
        if (synt[pos] == 's')
        {
          const pt cspos = pos;
          while (synt[++pos] == 's');
          if (level != 1 or code.substr(cspos,pos-cspos) != "case ")
            continue;
          const pt espos = pos;
          const pt tstrc = string_index + strs_this_statement; // The number of strings before this case label
          int strnum = 0; // The number of strings in this case label
          while (synt[pos] != ':') {
            if (synt[pos] == '"' or synt[pos] == '\'')
              ++strs_this_statement, ++strnum;
            ++pos;
          }
          cases.push_back(lexpair(string(code,espos,pos-espos),string(synt,espos,pos-espos),cspos,pos-cspos,tstrc,strnum));
          pos++; continue;
        }
        if (synt[pos] == 'b')
        {
          const pt dspos = pos;
          while (synt[++pos] == 'b');
          if (level == 1 and code.substr(dspos, pos-dspos) == "default")
            default_start_pos = dspos;
        }
        if (synt[pos] == '"' or synt[pos] == '\'')
          strs_this_statement++;
        pos++;
      }
      int handicap = 0;
      size_t ci = 0;
      for (; ci < cases.size(); ci++)
        if (!is_integer(cases[ci]))
        { handicap = 1; break; }
      if (handicap == 1)
        for (; ci < cases.size(); ci++)
          if (!is_literal(cases[ci]))
          { handicap = 2; break; }
      if (handicap)
      {
        int delta = 0;
        code.insert(pos,"}");
        synt.insert(pos,"}");

        char cname[12];
        sprintf(cname,"%d",switch_count);
        const string switch_index_code = cname;
        const string switch_index_lexn(switch_index_code.length(), 'n'), switch_index_lexb(switch_index_code.length(), 'b');

        if (default_start_pos) {
          code.replace(default_start_pos, 7, "$s" + switch_index_code + "default");
          synt.replace(default_start_pos, 7, "bb" + switch_index_lexb + "bbbbbbb");
        }

        for (size_t i = 0; i < cases.size(); i++)
        {
          sprintf(cname,"$s%dc%d",switch_count,(int)i);
          string rep = cname, res = string(rep.length(),'b');

          code.replace(cases[i].pos + delta, cases[i].len, cases[i].mylabel = rep);
          synt.replace(cases[i].pos + delta, cases[i].len, cases[i].mylsynt = res);

          delta += int(rep.length() - cases[i].len);
        }
        sprintf(cname,"$s%dvalue",switch_count);
        string valuevar = cname;

        string icode = "{", isynt = "{";
        icode += "const variant" + valuevar + "=" + svalue + ";";
        isynt += "ttttttttttttt" + string(valuevar.length(),'n') + "=" + svaluelex + ";";
        icode += "switch(enigma::switch_hash(" + valuevar + ")){";
        isynt += "ssssss(nnnnnnnnnnnnnnnnnnn(" + string(valuevar.length(), 'n') + ")){";

        // Order hashes
        map<int,vector<int> > hashes; // hash KEY by vector of case id's VALUE
        for (size_t i = 0; i < cases.size(); i++) {
          int hash = make_hash(cases[i], pev);
          hashes[hash].push_back(i);
        }

        // This'll be tacked on whenever the default is reached
        string deflcode = "default:",
               deflsynt = "bbbbbbb:";
        string deflsufcode = default_start_pos ? "goto $s" + switch_index_code + "default;" : "break;",
               deflsufsynt = default_start_pos ? "bbbbbbb" + switch_index_lexb + "bbbbbbb;" : "bbbbb;";
        if (hashes.size() > 1 or (hashes.size() == 1 and hashes.begin()->first != -1))
           deflcode += "$s" + switch_index_code + "nohash:",
           deflsynt += "bb" + switch_index_lexb + "bbbbbb:";
        
        // Defragment string IDs and generate case labels
        if (cases.size())
        {
          const int lower_bound = cases[0].stri, upper_bound = cases[cases.size()-1].stri + cases[cases.size()-1].strc - 1;
          map<int,string> reorder;
          for (int i = lower_bound; i <= upper_bound; i++)
            reorder[i] = pev->strs[i];

          int overwrite_at = lower_bound;
          for (map<int,vector<int> >::iterator i = hashes.begin(); i != hashes.end(); i++)
          {
            if (i->first == -1) {
              icode += deflcode, isynt += deflsynt;
              deflcode = deflsynt = "";
            }
            else {
              sprintf(cname, "%d", i->first);
              string thashstr = cname;
              icode += "case " + thashstr + ":";
              isynt += "sssss" + string(thashstr.length(), '0') + ":";
            }
            for (size_t ii = 0; ii < i->second.size(); ii++)
            {
              const int casenum = i->second[ii];
              icode += "if($s" + switch_index_code + "value==" + cases[casenum].code + ")goto " + cases[casenum].mylabel + ';';
              isynt += "ss(nn" + switch_index_lexn + "nnnnn==" + cases[casenum].synt + ")bbbbb" + cases[casenum].mylsynt + ';';
              string_index += cases[casenum].strc;

              for (int iii = 0; iii < cases[casenum].strc; iii++)
              {
                int indx = cases[casenum].stri + iii;
                pev->strs[overwrite_at++] = reorder[indx];
                reorder.erase(indx);
              }
            }
            if (i->first == -1) {
              icode += deflsufcode, isynt += deflsufsynt;
              deflsufcode = deflsufsynt = "";
            }
            else {
              icode += "goto $s" + switch_index_code + "nohash;";
              isynt += "bbbbbbb" + switch_index_lexb + "bbbbbb;";
            }
          }

          for (map<int,string>::iterator i = reorder.begin(); i != reorder.end(); i++)
            pev->strs[overwrite_at++] = i->second;
        }
        icode += deflcode + deflsufcode;
        isynt += deflsynt + deflsufsynt;

        pos = switch_start_pos;
        code.replace(pos, switch_value_spos-pos + svalue.length() + 1, icode);
        synt.replace(pos, switch_value_spos-pos + svalue.length() + 1, isynt);

        pos += icode.length();
        switch_count++;
      }
      else {
       	code.replace(switch_value_spos, svalue.length(), "(int" + svalue + ')');
        synt.replace(switch_value_spos, svalue.length(), "(ccc" + svaluelex + ')');
        pos = switch_start_pos + 5;
      }
    }
  }

  while (slev) delete sstack[slev--];
  delete sstack[0];
  return -1;
}
