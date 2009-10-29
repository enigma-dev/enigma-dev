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
#include <iostream>
using namespace std;
#include "../general/darray.h"
#include "../externs/externs.h"

#include "../general/parse_basics.h"

unsigned int strc = 0;
varray<string> stringincode;
map<string,char> edl_tokens; //logarithmic lookup, with token

string string_escape(string s)
{
  const int il = s.length();
  
  //allocate enough space to hold it
  char* out = new char[il*2 + 1];
  
  unsigned int opos = 0;
  for (int pos=0; pos<il; pos++)
  {
    if (s[pos] == '\'' or s[pos]=='"' or s[pos]=='\\')
      out[opos++] = '\\';
    out[opos++] = s[pos];
  }
  
  string ret(out,opos);
  delete[] out;
  return ret;
}

//Also removes comments and strings
void parser_remove_whitespace(string& code,const int use_cpp_strings)
{
  //Since we will only be shortening the current code,
  //instead of allocating a new string or buffer, we'll
  //just overwrite parts of the one we're using
  
  int pos, streampos = 0;
  const int len = code.length();
  
  for (pos=0; pos<len; pos++)
  {
    if (is_useless(code[pos]))
    {
      if (streampos == 0 or code[streampos-1] != ' ')
      code[streampos++] = ' ';
      while (is_useless(code[pos])) //0 is not useless.
        pos++;
      pos--;
      continue;
    }
    if (code[pos] == '/')
    {
      if (code[pos+1] == '/')
      {
        pos+=2;
        while (pos<len and code[pos] != '\r' and code[pos] != '\n')
          pos++;
        pos--;
        continue;
      }
      else if (code[pos+1] == '*')
      {
        pos+=2;
        while (pos<len and (code[pos] != '/' or code[pos-1] != '*'))
          pos++;
        continue;
      }
    }
    if (code[pos] == '"')
    {
      code[streampos++] = '"';
      pos++;
      if (use_cpp_strings)
      {
        int sp = pos;
        while (code[pos] != '"') //Stops dead on the next double quote
        {
          if (code[pos] == '\\')
            pos++;
          pos++;
        }
        stringincode[strc++] = code.substr(sp,pos-sp);
      }
      else
      {
        int sp = pos;
        while (code[++pos] != '"'); //Stops dead on the next double quote
        stringincode[strc++] = string_escape(code.substr(sp,pos-sp));
      }
      continue;
    }
    if (code[pos] == '\'')
    {
      code[streampos++] = '"'; //This remains double quote intentionally.
      pos++;
      if (use_cpp_strings)
      {
        int sp = pos;
        while (code[pos] != '\'') //Stops dead on the next quote
        {
          if (code[pos] == '\\')
            pos++;
          pos++;
        }
        stringincode[strc++] = code.substr(sp,pos-sp);
      }
      else
      {
        int sp = pos;
        while (code[++pos] != '\''); //Stops dead on the next quote
        stringincode[strc++] = string_escape(code.substr(sp,pos-sp));
      }
      continue;
    }
    if (is_letter(code[pos]))
    {
      const int sp = pos;
      const int ssp = streampos;
      
      while (is_letterd(code[pos]))
        code[streampos++] = code[pos++];
      
      if (pos-sp == 5) if (code.substr(sp,5) == "begin") {
        code[streampos = ssp] = '{';
        streampos++; } else;
      else if (pos-sp == 4) if (code.substr(sp,4) == "then")
        streampos = ssp; else;
      else if (pos-sp == 3) if (code.substr(sp,3) == "end") {
        code[streampos = ssp] = '}';
        streampos++; }
      
      pos--;
      continue;
    }
    code[streampos++] = code[pos];
  }
  code.erase(streampos);
  return;
}


void parser_buffer_syntax_map(string &code,string &syntax,const int use_cpp_numbers)
{
  syntax = code;
  externs* nscope = NULL;
  const int len = syntax.length();
  for (int pos = 0; pos < len; pos++)
  {
    if (is_letter(syntax[pos]))
    {
      int spos = pos;
      while (is_letterd(syntax[pos]))
      {
        syntax[pos] = 'n';
        pos++;
      }
      
      string lword = code.substr(spos,pos-spos);
      map<string,char>::iterator token = edl_tokens.find(lword);
      
      char c = 0;
      bool istemplate = 0;
      if (token != edl_tokens.end())
      {
        nscope = NULL;
        c = token->second;
        if (c == 't')
        {
          c = 'd';
          istemplate = 1;
        }
        
        for(int i = spos; i<pos; i++)
          syntax[i] = c;
      }
      else
      {
        extiter sit;
        if (nscope == NULL) sit = scope_find_member(lword);
        else sit = nscope->members.find(lword);
        
        if (sit != global_scope.members.end())
        {
          if (sit->second->flags & EXTFLAG_TYPENAME) c = 'd';
          //else if (sit->second->flags & EXTFLAG_NAMESPACE) c = 'N';
          else c = 'g';
          for(int i = spos; i<pos; i++)
            syntax[i] = c;
          istemplate = sit->second->flags & EXTFLAG_TEMPLATE;
        }
      }
      if (c == 'd')
      {
        if (code[pos] == ' ')
          syntax[pos++] = 'd';
        if (code[pos] == '<' and istemplate)
        {
          syntax[pos++] = 'd';
          for (int tb = 1; tb and pos<len; pos++)
          {
            if (syntax[pos] == '<') tb++;
            else if (syntax[pos] == '>') tb--;
            syntax[pos] = 'd';
          }
        }
        if (code[pos] == ' ')
          syntax[pos++] = 'd';
        
        if (spos>0 and syntax[spos] == '(' and syntax[pos] == ')' and syntax[spos] == 'd')
        {
          for(int i = spos-1; i<pos; i++)
            syntax[i] = 'c';
        }
      }
      pos--;
      continue;
    }
    if (is_digit(syntax[pos]))
    {
      nscope = NULL;
      if (pos and syntax[pos-1] == '.') syntax[pos-1] = '0';
      if (use_cpp_numbers)
      {
        while (is_letterd(syntax[pos]))
        {
          syntax[pos] = '0';
          pos++;
        }
      }
      else while (is_digit(syntax[pos]))
        syntax[pos++] = '0';
      pos--;
      continue;
    }
    //Scope access
    if (syntax[pos]==':' and syntax[pos+1]==':' and pos)
    {
      const char c = syntax[pos-1];
      if (c == 'n' or c == 'g' or c == 'd')
      {
        unsigned int epos = pos, i;
        for (i = pos-1; i and syntax[i] == c; i--) if (code[i] == '<') epos = i;
        const string aname = code.substr(i,epos-i);
        
        if (nscope == NULL)
          nscope = scope_find_member(aname)->second;
        else
          nscope = nscope->members.find(aname)->second;
      }
    }
    else if (syntax[pos]=='$')
    {
      code[pos] = '0';
      syntax[pos++]='0';
      code[pos] = 'x';
      syntax[pos++]='0';
      
      while ((code[pos] >= 'a' and code[pos]<='f') or (code[pos] >= 'A' and code[pos]<='F') or is_digit(code[pos]))
      {
        code[pos] = code[pos];
        syntax[pos++] = '0';
      }
    }
  }
}

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
        if (synt[pos+1] == ')')
        {
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
        sy_semi=sy_semi->popif('s');
        continue;
      }
      if (synt[pos]==':')
      {
        if (terns) terns--;
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
      
        codebuf[bufpos]='(';
        syntbuf[bufpos++]='(';
      sy_semi=sy_semi->push(')','s');
    }
    else if (synt[pos]=='f')
    {
      codebuf[bufpos+0]='o';
      codebuf[bufpos+1]='r';
      syntbuf[bufpos++]='f';
      syntbuf[bufpos+0]='f';
      
      pos+=3;
      if (synt[pos] != ' ')
        pos--; //If there's a (, you'll be at it next iteration
      
      codebuf[bufpos]='(';
      syntbuf[bufpos++]='(';
      
      sy_semi=sy_semi->push(')','s');
      sy_semi=sy_semi->push(';','s');
      sy_semi=sy_semi->push(';','s');
    }
  }
  
  //Add a semicolon at the end if there isn't one
  if (syntbuf[bufpos-1] != ';')
  {
    codebuf[bufpos]=';';
    syntbuf[bufpos++]=';';
  }
  
  code = string(codebuf,bufpos);
  synt = string(syntbuf,bufpos);
  
  cout << ":: " << code << "\r\n:: " << synt << "\r\n\r\n";
  
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

void print_the_fucker(string code,string synt)
{
  FILE* of = fopen("/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt","w+b");
  if (of == NULL) return;
  
  const char* indent = "\r\n\
                          \
                          \
                          \
                          \
                          \
                          \
                          \
                          \
                          \
                      ";
  const int indentmin=2;
  int indc = 0,tind = 0,pars = 0,str = 0;
  
  const unsigned int len = code.length();
  for (unsigned int pos = 0; pos < len; pos++)
  {
    switch (synt[pos])
    {
      case '{':
          tind = 0;
          fwrite(indent,1,indentmin+indc,of);
          if (indc < 256) indc+=2;
          fputc('{',of);
          fwrite(indent,1,indentmin+indc,of);
        break;
      case '}':
          tind = 0;
          indc -= 2;
          {
            fseek(of,-2,SEEK_CUR);
            bool ds = 1;
            ds &= fgetc(of) == ' ';
            ds &= fgetc(of) == ' ';
            if (ds) fseek(of,-2,SEEK_CUR);
          }
          fputc('}',of);
          fwrite(indent,1,indentmin+indc,of);
        break;
      case ';':
        if (pars)
        {
          fputc(code[pos],of);
          break;
        }
        if (tind) tind = 0;
        case ':':
          fputc(code[pos],of);
          fwrite(indent,1,indentmin+indc+tind,of);
        break;
      case '(':
          if (pars) pars++;
          fputc('(',of);
        break;
      case ')':
          if (pars) pars--;
          fputc(')',of);
          if (pars == 1)
          {
            pars = 0;
            if (synt[pos+1] != '{' and synt[pos+1] != ';')
            {
              tind+=2;
              fwrite(indent,1,indentmin+indc+tind,of);
            }
          }
        break;
      case '"':
          if (pars) pars--;
          fputc('"',of);
          for (int c = 1; c; c--)
          {
            fwrite(stringincode[str].c_str(),1,stringincode[str].length(),of);
            str++;
            if (synt[pos+1] == '+' and synt[pos+2] == '"')
            {
              pos+=2;
              c++;
            }
          }
          fputc('"',of);
        break;
      case 's':
      case 'f':
          pars = 1;
          fputc(code[pos],of);
        break;
      
      default:
          fputc(code[pos],of);
          if (likesaspace(synt[pos],synt[pos+1]))
            fputc(' ',of);
        break;
    }
  }
  fclose(of);
  if (system("\"/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt\""))
  if (system("gedit \"/media/HP_PAVILION/Documents and Settings/HP_Owner/Desktop/parseout.txt\""))
    printf("zomg fnf\r\n");
}
