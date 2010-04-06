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


#include <string>
#include <iostream>
#include <cstdio>
using namespace std;
#include "../general/darray.h"
#include "../externs/externs.h"

#include "../general/parse_basics.h"
#include "../compiler/output_locals.h"

#include "../settings.h"


typedef size_t pt; //Use size_t as our pos type; helps on 64bit systems.
unsigned int strc = 0; // Number of strings we removed from the code.
varray<string> string_in_code; // This holds strings we removed.
map<string,char> edl_tokens; // Logarithmic lookup, with token.
typedef map<string,char>::iterator tokiter;

string string_escape(string s)
{
  cout << "format " << s << endl;
  const pt il = s.length()-1;

  //allocate enough space to hold it
  char* out = new char[il*2 + 1];

  out[0] = '"';

  pt opos = 1;
  for (pt pos = 1; pos < il; pos++)
  {
    if (s[pos] == '\'' or s[pos]=='"' or s[pos]=='\\')
      out[opos++] = '\\';
    out[opos++] = s[pos];
  }

  out[opos++] = '"';

  string ret(out,opos);
  delete[] out;

  cout << "  as " << ret;
  return ret;
}

#include "../cfile_parse/macro_functions.h"


int scope_braceid = 0;
extern string tostring(int);
int dropscope()
{
  if (current_scope != &global_scope)
  current_scope = current_scope->parent;
  return 0;
}
int quickscope()
{
  externs *ne = current_scope->members["{}"+tostring(scope_braceid++)] = new externs;
  ne->name = "{}";
  ne->type = NULL;
  ne->flags = EXTFLAG_NAMESPACE;
  ne->parent = current_scope;
  current_scope = ne;
  return 0;
}
int initscope(string name)
{
  current_scope = &global_scope;
  scope_braceid = 0;

  externs *ne = current_scope->members[name] = new externs;
  ne->name = name;
  ne->type = NULL;
  ne->flags = EXTFLAG_NAMESPACE;
  ne->parent = current_scope;
  current_scope = ne;

  return 0;
}
int quicktype(unsigned flags, string name)
{
  externs *ne = current_scope->members[name] = new externs;
  ne->name = name;
  ne->type = NULL;
  ne->flags = flags | EXTFLAG_TYPENAME;
  ne->parent = current_scope;
  return 0;
}


///Remove whitespace, unfold macros,
///And lex code into synt.
//Compatibility considerations:
//123.45 with 000.00, then .0 with 00. Do *not* replace 0. with 00
int parser_ready_input(string &code,string &synt)
{
  synt = code;
  pt pos = 0, bpos = 0;
  char last_token = ' '; //Space is actually invalid. Heh.
  #ifndef ASSUME_SAFE
    const pt len = code.length();
    #define safe (pos < len)
  #else
    #define safe 1
  #endif

  unsigned mymacroc = 0;
  darray<pt> mymacroend;
  varray<string> mymacros;

  while (pos < code.length())
  {
    if (is_letter(code[pos]))
    {
      //This is a word of some sort. Could be a keyword, a type, a macro... maybe just a varname
      const pt spos = pos;
      while (is_letterd(code[++pos]));
      const string name = code.substr(spos,pos-spos);

      maciter itm = macros.find(name);
      if (itm != macros.end())
      {
        bool recurs=0;
        for (unsigned int iii=0;iii<mymacroc;iii++)
           if (mymacros[iii] == name)
           {
             if (pos <= mymacroend[iii]) {
               recurs=1; break;
             }
             else for (int k = 0, kn = 0, ic = mymacroc; k<ic; k++)
               if (pos > mymacroend[iii])
                 mymacros[kn++] = mymacros[k], mymacroc--;
           }

        if (!recurs)
        {
          string macrostr = itm->second;

          if (itm->second.argc != -1) //Expect ()
          {
            if (!macro_function_parse(code,name,pos,macrostr,itm->second.args,itm->second.argc,itm->second.args_uat)) {
              cout << "UNEXPECTED ERROR: " << macrostr;
              cout << "\nThis error should have been reported during a previous syntax check\n";
              continue;
            }
          }
          mymacros[mymacroc] = name;
          mymacroend[mymacroc++] = pos + string(itm->second).length();
          code.insert(pos,itm->second);
          continue;
        }
      }

      char c = 'n';

      if (name == "sometype")
        cout << "LOL SOMETYPE\n";

      tokiter itt = edl_tokens.find(name);
      if (itt != edl_tokens.end()) {
        c = itt->second;
      }
      else if (find_extname(name,0xFFFFFFFF))
      {
        if (ext_retriever_var->flags & EXTFLAG_TYPENAME)
          c = 't';
      }
      else if (name == "then")
        continue; //"Then" is a truly useless keyword. I see no need to preserve it.

      if (last_token == 'n' and c == 'n')
        code[bpos] = synt[bpos] = ';', bpos++;


      for (pt i = 0; i < name.length(); i++) {
        code[bpos]   = name[i];
        synt[bpos++] = c;
      }

      //Accurately reflect newly defined types and structures
      if (c == 'n' and last_token == 'C') { //"class <name>"
        quicktype(EXTFLAG_STRUCT,name); //Add the string we used to determine if this token is 'n' as a struct
      }

      last_token = c;
      continue;
    }
    else if (is_digit(code[pos]))
    {
      if (bpos and synt[bpos-1] == '.')
        synt[bpos-1] = '0';
      while (code[pos] == '0')
        pos++;
      if (is_digit(code[pos]))
        do {
          code[bpos] = code[pos];
          synt[bpos++] = '0';
        } while (is_digit(code[++pos]));
      else
       code[bpos] = synt[bpos] = last_token = '0', bpos++;

      continue;
    }
    if (code[pos] == '"')
    {
      string str;
      const pt spos = pos;
      if (OPTION_CPP_STRINGS)
      {
        while (safe and code[++pos] != '"')
          if (code[pos] == '\\') pos++;
        str = code.substr(spos,++pos-spos);
      }
      else
      {
        while (safe and code[++pos] != '"');
        str = string_escape(code.substr(spos,++pos-spos));
      }
      string_in_code[strc++] = str;
      code[bpos] = synt[bpos] = last_token = '"', bpos++;
      continue;
    }
    if (code[pos] == '\'')
    {
      string str;
      const pt spos = pos;
      if (OPTION_CPP_STRINGS)
      {
        while (safe and code[++pos] != '\'')
          if (code[pos] == '\\') pos++;
        str = code.substr(spos,++pos-spos);
      }
      else
      {
        while (safe and code[++pos] != '\'');
        str = string_escape(code.substr(spos,++pos-spos));
      }
      string_in_code[strc++] = str;
      code[bpos] = synt[bpos] = last_token = '"', bpos++;
      continue;
    }
    if (code[pos] == '/')
    {
      if (code[++pos] == '/')
      {
        while (code[++pos] != '\n' and code[pos] != '\r' and pos < len);
        continue;
      }
      else if (code[pos] == '*')
      {
        while ((code[pos++] != '*' or code[pos] != '/') and pos < len);
        pos++; continue;
      }
      code[bpos] = synt[bpos] = last_token = '/', bpos++;
      continue;
    }

    if (code[pos] == '{')
      quickscope();
    else if (code[pos] == '}')
      dropscope();

    //Wasn't anything usable
    if (!is_useless(code[pos]))
      code[bpos] = synt[bpos] = last_token = code[pos], bpos++;

    pos++;
  }

  code.erase(bpos);
  synt.erase(bpos);

  cout << code << endl << synt << endl << endl;
  return 0;
}

//Has conditional popping based on an attribute pushed with the value
struct stackif
{
  stackif* prev;
  char value,popifc;

  stackif(void):prev(NULL) { }
  stackif(char v):prev(NULL),value(v) { }
  stackif(stackif* p,char v,char i):prev(p),value(v),popifc(i) { }

  stackif* push(char v,char i)
  {
    stackif* r = new stackif(this,v,i);
    return r;
  }
  stackif* popif(char i)
  {
    if (prev == NULL or popifc != i)
      return this;

    stackif* r = prev;
    delete this;
    return r;
  }

  operator char()
  {
    return value;
  }
};

//Check if semicolon is needed here
inline bool need_semi(char c1,char c2,const bool sepd,char c0,char cn1)
{
  if (c1 == c2)
    return sepd and c1 != 'r';
  if (cn1 == 'n' and c0 == c1 and (c1 == '+' or c1=='-')) //n++n => n++; n
    return 1;
  return (c1 == 'b' or c1 == 'n' or c1 == '0' or c1 == '"' or c1 == ')' or c1 == ']')
  and (is_letterd(c2) or c2=='"' or c2=='{' or c2=='}');
}


int parser_reinterpret(string &code,string &synt)
{
  
  return 0;
}

//Add semicolons
void parser_add_semicolons(string &code,string &synt)
{
  cout << synt << endl << code << endl;

  //Allocate enough memory to hold all the semicolons we'd ever need
  char *codebuf = new char[code.length()*2+1];
  char *syntbuf = new char[code.length()*2+1];
  int bufpos = 0;
  int terns = 0;

  //Add the semicolons in obvious places
  stackif *sy_semi = new stackif(';');
  for (unsigned int pos=0; pos<code.length(); pos++)
  {
    if (synt[pos]==' ')
    {
      if (need_semi(synt[pos-1],synt[pos+1],1,synt[pos-2],synt[pos-3]))
      {
        codebuf[bufpos] = *sy_semi;
        syntbuf[bufpos++] = *sy_semi;
        sy_semi=sy_semi->popif('s');
      }
      else if (bufpos and syntbuf[bufpos-1] == synt[pos+1])
      {
        codebuf[bufpos] = ' ';
        syntbuf[bufpos++] = ' ';
      }
    }
    else
    {
      if (code[pos] != ' ' or synt[pos] == 'r')
      {
        codebuf[bufpos]=code[pos];
        syntbuf[bufpos++]=synt[pos];
      }

      if (synt[pos]=='(') { if (sy_semi->prev == NULL) sy_semi=sy_semi->push(',','('); continue; }
      if (synt[pos]==')') { sy_semi=sy_semi->popif('(');    continue; }
      if (synt[pos]==';')
      {
        if (synt[pos+1] == ')') {
          bufpos--;
          continue;
        }
        if (*sy_semi != ';')
        {
          codebuf[bufpos-1] = *sy_semi;
          syntbuf[bufpos-1] = *sy_semi;
          codebuf[bufpos+0] = ';';
          syntbuf[bufpos++] = ';';
        }
        sy_semi = sy_semi->popif('s');
        continue;
      }
      if (synt[pos]==':')
      {
        if (terns)
          terns--;
        else if (*sy_semi != ';')
        {
          codebuf[bufpos-1] = *sy_semi;
          syntbuf[bufpos-1] = *sy_semi;
          codebuf[bufpos+0] = ';';
          syntbuf[bufpos++] = ';';
        }
        sy_semi=sy_semi->popif('s');
        continue;
      }

      if (need_semi(synt[pos-1],synt[pos],0,synt[pos-2],synt[pos-3]))
      {
        codebuf[bufpos-1] = *sy_semi;
        syntbuf[bufpos-1] = *sy_semi;
        codebuf[bufpos] = code[pos];
        syntbuf[bufpos++] = synt[pos];
        sy_semi=sy_semi->popif('s');
      }
    }
    if (synt[pos]=='s')
    {
      while (synt[++pos] == 's')
      {
        codebuf[bufpos] = code[pos];
        syntbuf[bufpos++] = 's';
      }

      if (synt[pos] != ' ')
        pos--;

      codebuf[bufpos] = syntbuf[bufpos] = '(', bufpos++;
      sy_semi=sy_semi->push(')','s');
    }
    else if (synt[pos] == 'f')
    {
      codebuf[bufpos] = 'o';
      syntbuf[bufpos] = 'f';
      codebuf[++bufpos] = 'r';
      syntbuf[bufpos++] = 'f';

      pos+=3;
      if (synt[pos] != ' ')
        pos--; //If there's a (, you'll be at it next iteration

      codebuf[bufpos] = syntbuf[bufpos] = '(', bufpos++;

      sy_semi=sy_semi->push(')','s');
      sy_semi=sy_semi->push(';','s');
      sy_semi=sy_semi->push(';','s');
    }
  }

  //Add a semicolon at the end if there isn't one
  if (bufpos and syntbuf[bufpos-1] != ';')
    codebuf[bufpos] = syntbuf[bufpos] = ';', bufpos++;

  code = string(codebuf,bufpos);
  synt = string(syntbuf,bufpos);

  cout << code << endl << synt << endl << endl;

  //This part's trickier; add semicolons only after do ... until and do ... while
  unsigned int len = synt.length();
  for (unsigned int pos=0; pos<len; pos++)
  {
    if (synt[pos] == 'r') //For every 'do'
    {
      //Make sure we're at do
      if ((code[pos] != 'd'  or  code[pos+1] != 'o')
      or !(code[pos+2] == ' ' or synt[pos+2] != 'r'))
      {
        while (synt[pos] == 'r' and code[pos] != ' ') pos++;
        continue;
      }

      cout << "At do\r\n>>" << code << "\r\n>>" << synt << "\r\n\r\n";

      //Begin do handling

      pos += 2;
      int lpos = pos;
      int semis = 1, dos = 0;

      while (pos < len) //looking for end of do
      {
        if (synt[pos] == ';')
          semis=0;
        else if (synt[pos]=='s') //if we're at a statement
        {
          //if the statement is while or until
          if (synt[pos+1]=='s' and ((code[pos]=='w' and code[pos+1]=='h') or code[pos]=='u'))
          {
            if (dos == 0) //If this until is our closing until
            {
              if (semis) //If they said 'do while', leaving out the semicolon
              {
                code.insert(pos,";");
                synt.insert(pos,";");
                len++; pos++;
              }

              pos+=6; //Skip to the end of this until( or while(
              int ps = 1; //Track number of parentheses to find end
              while (pos<len and ps)
              {
                if (synt[pos]=='(') ps++;
                else if (synt[pos]==')') ps--;
                pos++;
              }

              if (synt[pos] != ';')
              {
                code.insert(pos,";");
                synt.insert(pos,";");
                len++;
              }
              break;
            }
            else
            {
              dos--;
              semis = 0;
              pos += 4;
            }
          }
          else
          semis=1;
        }
        else if (synt[pos]=='r')
        {
          if (code[pos]=='d' and code[pos+1]=='o' and (code[pos+2] == ' ' or synt[pos+2] != 'r'))
          {
            dos++;
            pos+=2;
            if (code[pos] != ' ') pos--;
          }
          else while (synt[pos] == 'r' and code[pos] != ' ') pos++;
        }
        else if (synt[pos]=='{')
        {
          pos++;
          for (int bs = 1; pos<len and bs; pos++)
          {
            if (synt[pos] == '{') bs++;
            else if (synt[pos] == '}') bs--;
          }
          pos--;
          semis--;
        }
        pos++;
      }

      //Go fix any nested do's
      pos = lpos;

      cout << "<<" << code << "\r\n<<" << synt << "\r\n\r\n";
    }
  }

  //now we'll do ONE MORE pass, to take care of (())
  len = code.length();
  for (unsigned int pos = 0; pos<len; pos++)
  if (synt[pos] == '(' and synt[pos+1]=='(')
  {
    const int sp = pos;

    pos+=2;
    int pl = 1;
    while (pos<len and pl)
    {
      if (synt[pos]=='(') pl++;
      else if (synt[pos]==')') pl--;
      pos++;
    }
    if (pl==0 and code[pos]==')')
    {
      code.erase(pos,1);
      synt.erase(pos,1);
      code.erase(sp,1);
      synt.erase(sp,1);
      len-=2;
    }
    pos = sp;
  }
}


///int gmlparse_get_varnames()
///  Expects properly formatted code (semicolon-wise)
///  @return Returns zero on success.
int gmlparse_get_varnames(string code, string synt)
{
  //This operates in its own pass, sadly.
  //During this pass, we will look for two tokens: 'n' and 't'
  //We will also acknowledge these tokens: '{' '}' ';'

  //Store length for quickness
  const register pt len = code.length();

  //Keep track of a few things while we read
  string decl_t; //Know what we're declaring things as.
  bool in_decl = false; //Know if we're actually declaring anything.
  bool suspend_t = false; //type a = b, we don't want to declare b as int. So we suspend until comma.

  for (register pt pos = 0; pos < len; pos++)
  {
    if (synt[pos] == 't')
    {
      const pt spos = pos;
      while (synt[++pos] == 't');
      decl_t = code.substr(spos,pos-spos+1);
      suspend_t = false;
      in_decl = true;
    }
    else if (synt[pos] == 'n')
    {
      const pt spos = pos;
      while (synt[++pos] == 'n');
      string name = code.substr(spos,pos-spos+1);
      string *res = &(parsed_varnames[name]);
      if (*res == "")
        *res = decl_t;
    }
    else if (synt[pos] == ';')
      decl_t = "";
  }
  return 0;
}


/**
This part outputs well-formatted code.

@func likesaspace(char c, char d)
  @return returns whether or not the two characters (as seen
  in syntax string) @param c and @param d should be separated
  by a space in the outputted code.

@func print_the_fucker(string code,string synt)
  @summary Outputs the code in a well-formatted fashion.

**/

inline bool likesaspace(char c,char d)
{
  if (c==d)
    return 0;
  if (c==' ' or c=='{' or c=='}' or c==':' or c==';')
    return 0;
  if (d==' ' or d=='{' or d=='}' or d==':' or d==';' or d==',')
    return 0;
  if (c=='=' and !is_letterd(d))
    return 0;
  if (d=='=' and !is_letterd(c))
    return 0;
  if ((c=='(' or c=='[') and is_letterd(d))
    return 0;
  if ((d=='(' or d=='[' or d==']' or d==')') and is_letterd(c))
    return 0;
  return 1;
}

#include <fstream>

const char * indent_chars   =  "\n                                \
                                                                  \
                                                                  \
                                                                  \
                                                                  ";

void print_to_file(string code,string synt,int indentmin_b4,ofstream &of)
{
  //FILE* of = fopen("/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt","w+b");
  //FILE* of = fopen("C:/Users/Josh/Desktop/parseout.txt","w+b");
  //if (of == NULL) return;
  
  const int indentmin = indentmin_b4 + 1;
  int indc = 0,tind = 0,pars = 0,str = 0;

  const unsigned int len = code.length();
  for (unsigned int pos = 0; pos < len; pos++)
  {
    switch (synt[pos])
    {
      case '{':
          tind = 0;
          of.write(indent_chars,indentmin+indc);
          if (indc < 256) indc+=2;
          of << '{';
          of.write(indent_chars,indentmin+indc);
        break;
      case '}':
          tind = 0;
          indc -= 2;
          of.write(indent_chars,indentmin+indc);
          of << '}';
          of.write(indent_chars,indentmin+indc);
        break;
      case ';':
        if (pars)
        {
          of << code[pos];
          of << ' ';
          break;
        }
        if (tind) tind = 0;
        case ':':
          of << code[pos];
          of.write(indent_chars,indentmin+indc+tind);
        break;
      case '(':
          if (pars) pars++;
          of << '(';
        break;
      case ')':
          if (pars) pars--;
          of << ')';
          if (pars == 1)
          {
            pars = 0;
            if (synt[pos+1] != '{' and synt[pos+1] != ';')
            {
              tind+=2;
              of.write(indent_chars,indentmin+indc+tind);
            }
          }
        break;
      case '"':
          if (pars) pars--;
          for (int c = 1; c; c--)
          {
            of.write(string_in_code[str].c_str(),string_in_code[str].length());
            str++;
            if (synt[pos+1] == '+' and synt[pos+2] == '"')
            {
              pos+=2;
              c++;
            }
          }
        break;
      case 's':
      case 'f':
          pars = 1;
          of << code[pos];
        break;

      default:
          of << code[pos];
          if (likesaspace(synt[pos],synt[pos+1]))
            of << ' ';
        break;
    }
  }
  /*if (system("\"/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt\""))
  if (system("gedit \"/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt\""))
    printf("zomg fnf\r\n");*/
}
