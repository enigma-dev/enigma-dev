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
  current_scope = current_scope->members["{}"+tostring(scope_braceid++)] = new externs("{}",NULL,current_scope,EXTFLAG_NAMESPACE);
  return 0;
}
int initscope(string name)
{
  scope_braceid = 0;
  current_scope = global_scope.members[name] = new externs(name,NULL,&global_scope,EXTFLAG_NAMESPACE);
  return 0;
}
int quicktype(unsigned flags, string name)
{
  current_scope->members[name] = new externs(name,NULL,current_scope,flags | EXTFLAG_TYPENAME);
  return 0;
}

struct macpinfo {
  string name, code;
  pt pos;
  void grab(string id, string c, pt p) {
    name = id;
    code = c; pos = p;
  }
  void release(string &c, pt &p) {
    c = code; p = pos;
  }
};


///Remove whitespace, unfold macros,
///And lex code into synt.
//Compatibility considerations:
//'123.45' with '000.00', then '.0' with '00'. Do *not* replace '0.' with '00'.
int parser_ready_input(string &code,string &synt,unsigned int &strc, varray<string> &string_in_code)
{
  string codo = synt = code;
  pt pos = 0, bpos = 0;
  char last_token = ' '; //Space is actually invalid. Heh.
  
  unsigned mymacroind = 0;
  varray<macpinfo> mymacrostack;
  
  for (;;)
  { if (pos >= code.length()) {
      if (mymacroind)
        mymacrostack[--mymacroind].release(code,pos);
      else break; continue;
    }
    
    //cout << synt.substr(0,bpos) << endl;
    
    if (is_letter(code[pos]))
    {
      //This is a word of some sort. Could be a keyword, a type, a macro... maybe just a varname
      const pt spos = pos;
      while (is_letterd(code[++pos]));
      const string name = code.substr(spos,pos-spos);
      
      if (name == "div")
      {
        codo[bpos] = 'd', synt[bpos++] = '@';
        codo[bpos] = 'i', synt[bpos++] = '@';
        codo[bpos] = 'v', synt[bpos++] = '@';
        continue;
      }
      
      maciter itm = macros.find(name);
      if (itm != macros.end())
      {
        bool recurs=0;
        for (unsigned int iii = 0; iii < mymacroind; iii++)
           if (mymacrostack[iii].name == name) {
             recurs=1; break;
           }
        
        if (!recurs)
        {
          string macrostr = itm->second;

          if (itm->second.argc != -1) //Expect ()
          {
            pt cwp = pos;
            while (is_useless(code[cwp])) cwp++;
            if (code[cwp] != '(')
              goto out_of_here;
            if (!macro_function_parse(code.c_str(),code.length(),name,pos,macrostr,itm->second.args,itm->second.argc,itm->second.args_uat)) {
              cout << "UNEXPECTED ERROR: " << macrostr;
              cout << "\nThis error should have been reported during a previous syntax check\n";
              continue;
            }
          }
          mymacrostack[mymacroind++].grab(name,code,pos);
          code = macrostr; pos = 0;
          codo.append(macrostr.length(),' ');
          synt.append(macrostr.length(),' ');
          continue;
        }
      }
      
      out_of_here:
      char c = 'n';
      
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
      
      if (last_token == c)
      {
        if (c != 'r' and c != 't')
          codo[bpos] = synt[bpos] = ' ', bpos++;
        else {
          codo[bpos] = ' ';
          synt[bpos++] = c;
        }
      }
      
      //Copy the identifier and its token over
      for (pt i = 0; i < name.length(); i++) {
        codo[bpos]   = name[i];
        synt[bpos++] = c;
      }
      
      //Accurately reflect newly defined types and structures
      //These will be added as types now, but their innards will be ignored until ENIGMA "link"
      if (c == 'n' and last_token == 'C') { //"class <name>"
        quicktype(EXTFLAG_STRUCT,name); //Add the string we used to determine if this token is 'n' as a struct
      }
      
      last_token = c;
      continue;
    }
    else if (is_digit(code[pos]))
    {
      if (code[pos] == '0' and code[pos + 1] == 'x')
        { pos++; goto HEXADECIMAL_LABEL; }
      if (bpos and synt[bpos-1] == '.')
        synt[bpos-1] = '0';
      else //We don't want to remove significant zeroes, only octal-inducing ones
        while (code[pos] == '0')
          pos++;
      if (is_digit(code[pos]))
        do {
          codo[bpos] = code[pos];
          synt[bpos++] = '0';
        } while (is_digit(code[++pos]));
      else
       codo[bpos] = synt[bpos] = last_token = '0', bpos++;
      
      continue;
    }
    if (code[pos] == '"')
    {
      string str;
      const pt spos = pos;
      if (OPTION_CPP_STRINGS)
      {
        while (code[++pos] != '"')
          if (code[pos] == '\\') pos++;
        str = code.substr(spos,++pos-spos);
      }
      else
      {
        while (code[++pos] != '"');
        str = string_escape(code.substr(spos,++pos-spos));
      }
      string_in_code[strc++] = str;
      cout << "\n\n\nCut string " << str << "\n\n\n";
      codo[bpos] = synt[bpos] = last_token = '"', bpos++;
      continue;
    }
    if (code[pos] == '\'')
    {
      string str;
      const pt spos = pos;
      if (OPTION_CPP_STRINGS)
      {
        while (code[++pos] != '\'')
          if (code[pos] == '\\') pos++;
        str = code.substr(spos,++pos-spos);
      }
      else
      {
        while (code[++pos] != '\'');
        str = string_escape(code.substr(spos,++pos-spos));
      }
      string_in_code[strc++] = str;
      codo[bpos] = synt[bpos] = last_token = '"', bpos++;
      continue;
    }
    if (code[pos] == '/')
    {
      if (code[++pos] == '/')
      {
        while (pos < code.length() and code[++pos] != '\n' and code[pos] != '\r');
        continue;
      }
      else if (code[pos] == '*')
      {
        while (pos < code.length() and (code[pos++] != '*' or code[pos] != '/'));
        pos++; continue;
      }
      else if (code[pos] != '=')
      {
        codo[bpos] = synt[bpos] = last_token = '/', bpos++;
        codo += "(double)";
        synt += "(double)";
        for (int i = 0; "(double)"[i]; i++)
          codo[bpos] = "(double)"[i], synt[bpos] = 'c', bpos++;
        continue;
      }
      codo[bpos] = synt[bpos] = last_token = '/', bpos++;
      continue;
    }
    
    if (code[pos] == '$')
    {
      HEXADECIMAL_LABEL:
      codo.append(1,' ');
      synt.append(1,' ');
      codo[bpos] = synt[bpos] = '0', bpos++;
      codo[bpos] = 'x', synt[bpos] = '0', bpos++;
      while (is_hexdigit(code[++pos]))
        codo[bpos] = code[pos], synt[bpos] = '0', bpos++;
      continue;
    }
    
    if (code[pos] == '{')
      quickscope();
    else if (code[pos] == '}')
      dropscope();
    
    //Wasn't anything usable
    if (!is_useless(code[pos]))
      codo[bpos] = synt[bpos] = last_token = code[pos], bpos++;
    
    pos++;
  }
  
  code = codo;
  
  code.erase(bpos);
  synt.erase(bpos);
  
  //cout << code << endl << synt << endl << endl;
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
inline bool need_semi(char c1,char c2)
{
  if (c1 == c2) return 0; //if the two tokens are the same, we assume they are one word; if they are
  return (c1 == 'b' or c1 == 'n' or c1 == '0' or c1 == '"' or c1 == ')' or c1 == ']')
  and (is_letterd(c2) or c2=='"' or c2=='{' or c2=='}');
}
inline bool need_semi_sepd(char c1,char c2)
{
  if (c1 == c2) return 1; //if the two tokens are the same, we assume they are one word; if they are
  return (c1 == 'b' or c1 == 'n' or c1 == '0' or c1 == '"' or c1 == ')' or c1 == ']')
  and (is_letterd(c2) or c2=='"' or c2=='{' or c2=='}');
}


int parser_reinterpret(string &code,string &synt)
{
  cout << "Second pass...";
  for (pt pos = 1; pos < code.length(); pos++)
  {
    if (synt[pos] == '0' and synt[pos-1] == '.')
      synt[pos-1] = '0';
    else if (synt[pos] == 't')
    {
      if (synt[pos-1] == '(')
      {
        const pt sp = pos-1;
        while (synt[++pos] == 't');
        if (synt[pos] == ')')
          for (pt i = sp; i <= pos; i++)
            synt[i] = 'c';
      }
      //else if (synt[pos+1]  == '(') // This case doesn't need handled. ttt() is fine as-is.
    }
    else if(synt[pos] == '<' and synt[pos-1] == 't')
    {
      synt[pos++] = 't';
      for (int pc = 1; pc > 0; synt[pos++] = 't')
        if (synt[pos] == '>') pc--;
        else if (synt[pos] == '<') pc++;
    }
  }
  //cout << "done. " << synt << endl << endl;
  return 0;
}

//Add semicolons
void parser_add_semicolons(string &code,string &synt)
{
  //Allocate enough memory to hold all the semicolons we'd ever need
  char *codebuf = new char[code.length()*2+1];
  char *syntbuf = new char[code.length()*2+1];
  int bufpos = 0;
  
  //Add the semicolons in obvious places
  stackif *sy_semi = new stackif(';');
  for (pt pos=0; pos<code.length(); pos++)
  {
    if (synt[pos]==' ') // Automatic semicolon
    {
      codebuf[bufpos] = *sy_semi;
      syntbuf[bufpos++] = *sy_semi;
      sy_semi=sy_semi->popif('s');
    }
    else
    {
      codebuf[bufpos]=code[pos];
      syntbuf[bufpos++]=synt[pos];
      
      if (synt[pos]=='(') { if (sy_semi->prev == NULL) sy_semi=sy_semi->push(',','('); continue; }
      if (synt[pos]==')') { sy_semi=sy_semi->popif('(');    continue; }
      if (synt[pos]==';')
      {
        /*if (synt[pos+1] == ')') {
          bufpos--;
          continue;
        }*/
        while (*sy_semi != ';')
        {
          codebuf[bufpos-1] = *sy_semi;
          syntbuf[bufpos-1] = *sy_semi;
          codebuf[bufpos+0] = ';';
          syntbuf[bufpos++] = ';';
          sy_semi = sy_semi->popif('s');
        }
        sy_semi = sy_semi->popif('s');
        continue;
      }
      if (synt[pos]==':')
      {
        if (*sy_semi != ';' and *sy_semi != ':')
        {
          codebuf[bufpos-1] = *sy_semi;
          syntbuf[bufpos-1] = *sy_semi;
          codebuf[bufpos+0] = ';';
          syntbuf[bufpos++] = ';';
          sy_semi = sy_semi->popif('s');
        }
        sy_semi=sy_semi->popif('s');
        continue;
      }
      
      if (synt[pos]=='?')
      {
        sy_semi=sy_semi->push(':','s');
        continue;
      }

      if (pos and need_semi(synt[pos-1],synt[pos]))
      {
        codebuf[bufpos-1] = *sy_semi;
        syntbuf[bufpos-1] = *sy_semi;
        codebuf[bufpos  ] = code[pos];
        syntbuf[bufpos++] = synt[pos];
        sy_semi=sy_semi->popif('s');
      }
      if((pos>2 and synt[pos] == '+' and synt[pos-1] == '+' and synt[pos-2] == 'n' and need_semi_sepd('n',synt[pos+1]))
      or (pos>2 and synt[pos] == '-' and synt[pos-1] == '-' and synt[pos-2] == 'n' and need_semi_sepd('n',synt[pos+1])))
      {
        codebuf[bufpos  ] = *sy_semi;
        syntbuf[bufpos++] = *sy_semi;
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

  //Dump the semicolon stack at the end.
  do codebuf[bufpos] = syntbuf[bufpos] = *sy_semi, bufpos++;
  while (sy_semi->prev and (sy_semi = sy_semi->prev));

  code = string(codebuf,bufpos);
  synt = string(syntbuf,bufpos);

  //cout << code << endl << synt << endl << endl;
  //cout << "cp1"; fflush(stdout);
  
  //This part's trickier; add semicolons only after do ... until and do ... while
  pt len = synt.length();
  for (pt pos=0; pos<len; pos++)
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
              if (semis) { // If they said 'do while', leaving out the semicolon
                pos++; continue; // We let it go.
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
    }
  }
  
  //cout << "cp2"; fflush(stdout);

  //now we'll do ONE MORE pass, to take care of (())
  len = code.length();
  for (pt pos = 0; pos<len; pos++)
  if (synt[pos] == '(' and synt[pos+1]=='(')
  {
    const pt sp = pos;

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
  cout << "done. "; fflush(stdout);
}


/**
  This part outputs somewhat well-formatted code.

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
  if (c == '-' and d == '>')
    return 0;
  if (c == '.' or d == '.')
    return 0;
  return 1;
}

#include <fstream>

const char * indent_chars   =  "\n                                \
                                                                  \
                                                                  \
                                                                  \
                                                                  ";
static inline string string_settings_escape(string n)
{
  for (size_t pos = 0; pos < n.length(); pos++)
  {
    switch (n[pos])
    {
      case '\\':
          if (n[pos+1] == '#') // Backslashes can only escape number signs
            n.erase(pos,1); // It happens to be escaping this one
          else
            n.insert(pos++,1,'\\'); // Ordinary backslash; escape it for C++.
        break;
      case '#':
          n[pos] = 'n'; // Newlines are expressed with #
          n.insert(pos++,1,'\\');
        break;
      case '\n': // Newlines are allowed in strings in GML, but not in C
          n[pos] = 'n';
          n.insert(pos++,1,'\\');
        break;
      case '\r': // Handle odd cases
          n[pos] = '\\';
          if (n[++pos] == '\n')
            n[pos] = 'n';
          else n.insert(pos,1,'n');
        break;
    } 
  }
  return n;
}
void print_to_file(string code,string synt,unsigned int &strc, varray<string> &string_in_code,int indentmin_b4,ofstream &of)
{
  //FILE* of = fopen("/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt","w+b");
  //FILE* of = fopen("C:/Users/Josh/Desktop/parseout.txt","w+b");
  //if (of == NULL) return;

  const int indentmin = indentmin_b4 + 1;
  int indc = 0,tind = 0,pars = 0;
  unsigned str_ind = 0;

  const pt len = code.length();
  for (pt pos = 0; pos < len; pos++)
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
            if (str_ind >= strc) cout << "What the crap.\n";
            of << string_settings_escape(string_in_code[str_ind]).c_str();
            str_ind++;
            if (synt[pos+1] == '+' and synt[pos+2] == '"')
              synt[pos+1] = code[pos+1] = ' ';
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

int parser_fix_templates(string &code,pt pos,pt spos,string *synt)
{
  cout << "qass: " << pos << " <" << ((synt && code.length()) == (synt && synt->length()) ? "equivalent" : "UNEQUAL") << "> [" << (pos > code.length()) << "]";
  pt epos = pos;
  int a2i = 0;
  
  if (code[--epos] == '>')
  {
    --epos;
    int ac = 0; bool ga = false; // argument count, given argument = whether or not anything other than whitespace was specified for the arg type
    for (int tbc = 1; tbc > 0; epos--) // track triangle bracket count 
    {
      if (code[epos] == '<')
        { tbc--; continue; }
      tbc += (code[epos] == '>');
      ac +=  (code[epos] == ',') and ga and (tbc == 1);
      ga |=  (code[epos] != '<'  and code[epos] != '>' and !is_useless(code[epos]));
      ga &=  (code[epos] != ',');
    }
    a2i = ac + ga;
  }
  
  pt sp2 = spos;
  while (code[sp2] != '<' and sp2 < epos)
    if (code[sp2++] == ' ') spos = sp2;
  
  cout << " <" << ((synt && code.length()) == (synt && synt->length()) ? "equivalent" : "UNEQUAL") << "> [" << (pos > code.length()) << "]";
  cout << "ass: " << spos << ", " << epos << ": " << code.length() << endl;
  string ptname = code.substr(spos,epos-spos+1); // Isolate the potential template's name
  bool fnd = find_extname(ptname,0xFFFFFFFF);
  if (!fnd) return 0;
  
  externs *a = ext_retriever_var;
  if (a->flags & EXTFLAG_TEMPLATE)
  {
    int tmc = a->tempargs.size - 1;
    for (int i = tmc; i >= 0; i--)
      if (a->tempargs[i]->flags & EXTFLAG_DEFAULTED) tmc = i;
    a2i = tmc - a2i;
    string iseg;
    for (int i = 0; i < a2i;)
      iseg += (++i < a2i) ? "variant," : "variant";
    if (code[pos-1] == '>')
    {
      if (code[pos-2] == ',' or code[pos-2] == '<')
      {
        if (iseg.length())
        {
          code.insert(pos-1, iseg);
          synt && (synt->insert(pos-1, string(iseg.length(),'t')),   true);
          return iseg.length();
        }
        else if (code[pos-2] == ',')
        {
          code.erase(pos-2,1);
          synt->erase(pos-2,1);
          return -1;
        }
      }
      else if (iseg.length())
      {
        code.insert(pos-1, ","+iseg),
        synt && (synt->insert(pos-1, string(iseg.length()+1,'t')), true);
        return iseg.length() + 1;
      }
    }
    else
    {
      code.insert(pos, "<"+iseg+">"),
      synt && (synt->insert(pos,   string(iseg.length()+2,'t')),   true);
      return iseg.length() + 2;
    }
  }
  return 0;
}

#include <stack>

// Return whether or not the left hand side of a dot requires an ENIGMA access function
bool parse_dot(string exp)
{
  enum { DT_BRACKET, DT_PAR, DT_LEVEL };
  stack<int> dts;
  int level = 0;
  return !level;
}
