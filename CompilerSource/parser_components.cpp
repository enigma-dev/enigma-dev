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

#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

#include "string.h"

extern int localc;
extern map <int,std::string> localn,localt;

int __constcount=0;
map<int,string> __consts;

int __enigmaglobalcount=33;
map<int,string> __enigmaglobals;

extern std::string __codebuffer, __syntaxbuffer, *__varnames, *__varnamestars, *__varnametags, *__varnamearrays;
extern std::map<int,std::string> __stringincode,__withs,__globals,__globalstars,__globaltags,__globalarrays,__fnames;
extern int _errorlast, _error_position, __varcount,__fcount,__withcount,__globalcount,__globalscrcount;


int parser_init()
{
    /* Allocate some memory as a test and safety precaution */

    __varnames  = new string[20];
    __varnametags = new string[20];
    __varnamestars = new string[20];
    __varnamearrays = new string[20];

    if (__varnames==NULL || __varnametags==NULL || __varnamearrays==NULL)
    {
      printf("Memory is dangerously low. Parse can not continue.\r\n");
      return 1;
    }
    return 0;
}



int max(int a,int b)
{
    if (a>b) return a;
    return b;
}
int min(int a,int b)
{
    if (a<b) return a;
    return b;
}



#define ERR_NONTERMSTR     1
#define ERR_STRCATREAL     2
#define ERR_UNEXPECTED     3
#define ERR_SEMICOLON      4
#define ERR_NONTERMPAR     5
#define ERR_ASSOP          6
#define ERR_PAREXPECTED    7
#define ERR_NONTERMINATING 8
#define ERR_MALTERMINATING 9

#define is_letter(x)  ((x>='a' && x<='z') || (x>='A' && x<='Z') || (x=='_'))
  #define is_digit(x)   (x>='0' && x<='9')
  #define is_letterd(x) (is_letter(x) || is_digit(x))
  #define is_unary(x)   (x=='!' || x=='~' || x=='+' || x=='-' || x=='&' || x=='*')
  #define is_linker(x)  (x=='+' || x=='-' || x=='*' || x=='/' || x=='=' || x=='!' || x=='~' || x=='&' || x=='|' || x=='^' || x=='.')
  #define is_useless(x) (x==' ' || x=='\r' || x=='\n' || x=='\t')

int is_lettersdigits(std::string argument0,int argument1=0)
{
    argument0=string_replace_all(argument0,chr(238),"d");
    if (argument1==0)
    return ((string_replace_all(argument0,"_","u")==string_lettersdigits(string_replace_all(argument0,"_","u"))));
    else
    return ((argument0==string_lettersdigits(argument0)));
}

int is_lettersdigits(char argument0,int argument1=0)
    {
    if (argument0==(char)238)
    return 1;
    if (argument0=='_' && argument1==0)
    return 1;

    return (argument0>=65 && argument0<=90) || (argument0>=97 && argument0<=122) || (argument0>=48 && argument0<=57);
}



int is_of_digits(std::string argument0)
    {
    argument0=string_replace_all(argument0,chr(238),"1");
    argument0=string_replace_all(argument0,".","1");
    return (argument0==string_digits(argument0));
}

int is_of_digits(char argument0)
    {
    if ((unsigned char)argument0==238)
    return 1;
    if (argument0 == '.')
    return 1;
    return (argument0>='0' && argument0<='9');
}








int is_of_letters(std::string argument0)
{
  argument0=string_replace_all(argument0,"_","u");
  argument0=string_replace_all(argument0,chr(238),"d");
  argument0=string_replace_all(argument0,".","d");
  return (argument0==string_letters(argument0));
}

int is_of_letters(char argument0)
{
  if (argument0=='_'
  ||  argument0==(char)238
  ||  argument0=='.')
  return 1;

  return (argument0>=65 && argument0<=91) || (argument0>=97 && argument0<=122);
}






void parser_buffer_syntax_map()
{
    unsigned int pos;
    __syntaxbuffer.reserve(4000);

    pos=1;
    char ftype='\0';
    char letter='\0';

    //printf("subseg1\r\n");

    std::string strseg8[11];
    char strchr1[11];

    //printf("subseg2\r\n");

    while (!(pos>string_length(__codebuffer)))
    {
        //printf("subseg2.5\r\n");
        letter=__codebuffer[-1+pos];

        //printf("subseg2.75\r\n");
        if (is_lettersdigits(letter) && letter != (char)238)
        {
            //printf("subseg3\r\n");
            if (ftype=='\0'||!is_lettersdigits(ftype))
            {
                if (is_of_digits(letter))
                ftype='0';
                if (is_of_letters(letter))
                ftype='n';
                if (letter==(char)240)
                ftype=letter;
            }
            if (is_of_letters(letter)&&ftype=='0')
            ftype='n';
            __syntaxbuffer+=ftype;
        }
        else
        {
            //printf("subseg4\r\n");
            while (__codebuffer[-1+pos]=='<')
            {
               strseg8[4]=string_copy(__codebuffer,pos,4);
               strseg8[5]=string_copy(__codebuffer,pos,5);
               strseg8[6]=string_copy(__codebuffer,pos,6);
               strseg8[7]=string_copy(__codebuffer,pos,7);
               strseg8[8]=string_copy(__codebuffer,pos,8);
               strseg8[9]=string_copy(__codebuffer,pos,9);
               strseg8[10]=string_copy(__codebuffer,pos,10);
               strchr1[4]=__codebuffer[-1+pos+4];
               strchr1[5]=__codebuffer[-1+pos+5];
               strchr1[6]=__codebuffer[-1+pos+6];
               strchr1[7]=__codebuffer[-1+pos+7];
               strchr1[8]=__codebuffer[-1+pos+8];
               strchr1[9]=__codebuffer[-1+pos+9];
               strchr1[10]=__codebuffer[-1+pos+10];

               if (strseg8[4]=="<if>")        { __syntaxbuffer+=strseg8[4]; pos+=4; letter=strchr1[4]; continue; }
               if (strseg8[5]=="<for>")       { __syntaxbuffer+=strseg8[5]; pos+=5; letter=strchr1[5]; continue; }
               if (strseg8[7]=="<while>")     { __syntaxbuffer+=strseg8[7]; pos+=7; letter=strchr1[7]; continue; }
               if (strseg8[6]=="<with>")      { __syntaxbuffer+=strseg8[6]; pos+=6; letter=strchr1[6]; continue; }
               if (strseg8[8]=="<switch>")    { __syntaxbuffer+=strseg8[8]; pos+=8; letter=strchr1[8]; continue; }
               if (strseg8[8]=="<repeat>")    { __syntaxbuffer+=strseg8[8]; pos+=8; letter=strchr1[8]; continue; }
               if (strseg8[8]=="<return>")    { __syntaxbuffer+=strseg8[8]; pos+=8; letter=strchr1[8]; continue; }

               if (strseg8[5]=="<var>")       { __syntaxbuffer+=strseg8[5]; pos+=5; letter=strchr1[5]; continue; }
               if (strseg8[8]=="<double>")    { __syntaxbuffer+=strseg8[8]; pos+=8; letter=strchr1[8]; continue; }
               if (strseg8[7]=="<float>")     { __syntaxbuffer+=strseg8[7]; pos+=7; letter=strchr1[7]; continue; }
               if (strseg8[5]=="<int>")       { __syntaxbuffer+=strseg8[5]; pos+=5; letter=strchr1[5]; continue; }

               if (strseg8[6]=="<char>")      { __syntaxbuffer+=strseg8[6]; pos+=6; letter=strchr1[6]; continue; }
               if (strseg8[6]=="<bool>")      { __syntaxbuffer+=strseg8[6]; pos+=6; letter=strchr1[6]; continue; }
               if (strseg8[9]=="<cstring>")   { __syntaxbuffer+=strseg8[9]; pos+=9; letter=strchr1[9]; continue; }

               if (strseg8[6]=="<long>")      { __syntaxbuffer+=strseg8[6]; pos+=6; letter=strchr1[6]; continue; }
               if (strseg8[7]=="<short>")     { __syntaxbuffer+=strseg8[7]; pos+=7; letter=strchr1[7]; continue; }
               if (strseg8[8]=="<signed>")    { __syntaxbuffer+=strseg8[8]; pos+=8; letter=strchr1[8]; continue; }
               if (strseg8[10]=="<unsigned>") { __syntaxbuffer+=strseg8[10];pos+=10;letter=strchr1[10];continue; }

               if (strseg8[7]=="<const>")     { __syntaxbuffer+=strseg8[7]; pos+=7; letter=strchr1[7]; continue; }
               if (strseg8[8]=="<static>")    { __syntaxbuffer+=strseg8[8]; pos+=8; letter=strchr1[8]; continue; }

               if (strseg8[8]=="<localv>")    { __syntaxbuffer+=strseg8[8]; pos+=8; letter=strchr1[8]; continue; }

               if (strseg8[5]=="<new>")       { __syntaxbuffer+=strseg8[5]; pos+=5; letter=strchr1[5]; continue; }
               if (strseg8[5]=="<delete>")    { __syntaxbuffer+=strseg8[8]; pos+=8; letter=strchr1[8]; continue; }

               if (strseg8[4]=="<do>")        { __syntaxbuffer+=strseg8[4]; pos+=4; letter=strchr1[4]; continue; }
               if (strseg8[7]=="<until>")     { __syntaxbuffer+=strseg8[7]; pos+=7; letter=strchr1[7]; continue; }
               if (strseg8[6]=="<then>")      { __syntaxbuffer+=strseg8[6]; pos+=6; letter=strchr1[6]; continue; }
               if (strseg8[6]=="<case>")      { __syntaxbuffer+=strseg8[6]; pos+=6; letter=strchr1[6]; continue; }
               if (strseg8[9]=="<default>")   { __syntaxbuffer+=strseg8[9]; pos+=9; letter=strchr1[9]; continue; }
               if (strseg8[6]=="<exit>")      { __syntaxbuffer+=strseg8[6]; pos+=6; letter=strchr1[6]; continue; }

               if (strseg8[4]=="<lt>")        { __syntaxbuffer+=strseg8[4]; pos+=4; letter=strchr1[4]; continue; }
               if (strseg8[4]=="<gt>")        { __syntaxbuffer+=strseg8[4]; pos+=4; letter=strchr1[4]; continue; }
               if (strseg8[4]=="<l=>")        { __syntaxbuffer+=strseg8[4]; pos+=4; letter=strchr1[4]; continue; }
               if (strseg8[4]=="<g=>")        { __syntaxbuffer+=strseg8[4]; pos+=4; letter=strchr1[4]; continue; }
               if (strseg8[5]=="<lsh>")       { __syntaxbuffer+=strseg8[5]; pos+=5; letter=strchr1[5]; continue; }
               if (strseg8[5]=="<rsh>")       { __syntaxbuffer+=strseg8[5]; pos+=5; letter=strchr1[5]; continue; }

               break;
            }

            if (letter==(char)238)
            {
               __syntaxbuffer+='.';
            }
            else
            {
                //printf("subseg5\r\n");
                if (letter=='"')
                {
                    __syntaxbuffer+="s";
                    pos+=1;
                    do
                    {
                        __syntaxbuffer+="s";
                        pos+=1;
                    }
                    while (!(__codebuffer[-1+pos]=='\"'));
                }
                if (letter=='\'')
                {
                    __syntaxbuffer+="s";
                    pos+=1;
                    do
                    {
                        __syntaxbuffer+="s";
                        pos+=1;
                    }
                    while (!(__codebuffer[-1+pos]=='\''));
                }

                //printf("subseg6\r\n");
                if (is_lettersdigits(letter))
                {
                    if (is_of_digits(letter))
                    ftype='0';
                    if (is_of_letters(letter))
                    ftype='n';
                    if (letter==(char)240)
                    ftype=letter;
                }
                else
                {
                    ftype=letter;
                }
                __syntaxbuffer+=ftype;
            }
        }
        pos+=1;
        //printf("subseg7\r\n");
    }

    __syntaxbuffer=string_replace_all(__syntaxbuffer,"n(","f(");
    while (string_count("nf",__syntaxbuffer))
    __syntaxbuffer=string_replace(__syntaxbuffer,"nf","ff");
    while (string_count("0i",__syntaxbuffer))
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"0i","ii");
    while (string_count("ni",__syntaxbuffer))
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"ni","ii");
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"(i","ii");
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"ii","nn");
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"in","nn");
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"<nnt>","<int>");
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"<$>n","<$>H");
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"<$>0","<$>H");
    while (string_count("H0",__syntaxbuffer) || string_count("Hn",__syntaxbuffer))
    {__syntaxbuffer=string_replace_all(__syntaxbuffer,"H0","HH");
     __syntaxbuffer=string_replace_all(__syntaxbuffer,"Hn","HH"); }
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"H","0");
}



/*
void parser_buffer_UNCcode(int textfileread)
{
    int stron=0, bufpos=1;
    std::string crln="";
    std::string concat;
    int iter;
    while (!file_text_eof(textfileread))
    {
        crln=file_text_read_string(textfileread);
        __codebuffer+=crln;
        file_text_readln(textfileread);
        while (!(bufpos>string_length(__codebuffer)))
        {
            if (stron==0)
            {
                if (__codebuffer[-1+bufpos]=='"')
                {
                    stron=1;
                }
                else if (__codebuffer[-1+bufpos]=='\'')
                {
                    stron=2;
                }
            }
            else
            {
                if (stron==1&&__codebuffer[-1+bufpos]=='"')
                {
                    stron=0;
                }
                if (stron==2&&__codebuffer[-1+bufpos]=='\'')
                {
                    stron=0;
                }
            }
            if (string_copy(__codebuffer,bufpos,2)=="//"&&stron==0)
            {
                __codebuffer=string_delete(__codebuffer,bufpos,string_length(__codebuffer));
            }
            bufpos+=1;
        }
        __codebuffer+=" ";
    }
    __codebuffer+=" ";
}
*/




int parser_end_of_braces(unsigned int pos)
{
    int par,e;
    par=0;
    e=0;
    do
    {
        char strchr7=__codebuffer[-1+pos];
        if (strchr7=='{')
            {
                par+=1;
                e=1;
            }
        if (strchr7=='}')
        {
            par-=1;
        }
        if (par==0&&e==1)
        return (pos);
        pos+=1;
    }
    while (!(pos>string_length(__codebuffer)));
    return 0;
}




int parser_end_of_brackets(unsigned int pos)
{
    int par,e;
    par=0;
    e=0;
    do
    {
        char strchr8=__codebuffer[-1+pos];
        if (strchr8=='[')
        {
            par+=1;
            e=1;
        }
        if (strchr8==']')
        {
            par-=1;
        }
        if (par==0&&e==1)
        return (pos);
        pos+=1;
    }
    while (!(pos>string_length(__codebuffer)));
    return 0;
}




int parser_end_of_parenths(unsigned int pos)
{
    int par,e;
    par=0;
    e=0;

    do
    {
        char strchr9=__codebuffer[-1+pos];

        if (strchr9=='(')
        {
            par+=1;
            e=1;
        }
        if (strchr9==')')
        {
            par-=1;
        }
        if (par==0&&e==1)
        return (pos);
        pos+=1;
    }
    while (!(pos>string_length(__codebuffer)));
    return 0;
}





/*
void parser_fix_typecasts()
{
    unsigned int pos;
    std::string a;
    int putting; //inside cast parenthesis
    putting=0;
    for (pos=0;pos<string_length(__codebuffer);pos+=1)
    {
        a=string_copy(__codebuffer,pos,14);               //copy the segment
        if (a=="__EGMTYPECAST_")                          //if it's a cast indicator
        {
            __codebuffer = string_delete( __codebuffer, pos,14);         //delete it
            __syntaxbuffer=string_delete(__syntaxbuffer,pos,14);         //all of it
            if (!putting)   //if we're NOT already inside a cast
            {
                __codebuffer = string_insert("(", __codebuffer, pos); //put the starting
                __syntaxbuffer=string_insert("(",__syntaxbuffer,pos); //parenthesis
                putting=1; //indicate we are now inside a cast.
            }

            do pos+=1;              //I added ordinal 238 after each of these casts.
            while (!((unsigned char)__codebuffer[-1+pos]==238));          //Find it.

            //Now replace it with a space and get ready to repeat the loop.
            __codebuffer=string_insert(" ",string_delete(__codebuffer,pos,1),pos);
            __syntaxbuffer=string_insert(" ",string_delete(__syntaxbuffer,pos,1),pos);
        }
        else if (putting)  //if there is no cast indicator, if you're inside a cast.
        {                  //   (That means the cast has ended; we need to show it.)
            //Indicate no longer in cast
            putting=0;

            //Add the terminating parenthesis
            __codebuffer = string_insert(") ",string_delete( __codebuffer, pos-1,1),pos-1);
            __syntaxbuffer=string_insert(") ",string_delete(__syntaxbuffer,pos-1,1),pos-1);
        }
    }
}


*/



int parser_potential_error(int argument0, int argument1=0, int argument2=0)
{
  if (_errorlast != 0)
  return 0;
  if (argument2)
  {
    _errorlast=argument0;
    _error_position=argument1;
  }
  return 1;
}

/*
void parser_recast_unify()
{
  unsigned int pos,endpos;
  pos=0;
  while (pos<__codebuffer.length())
  {
    if ((__codebuffer[pos]=='('))
    {
       pos++;
       endpos=parser_end_of_parenths(pos)-1;
       if (__codebuffer[pos]=='<'&&__codebuffer[endpos-1]=='>')
       {
          __codebuffer.replace(pos-1,2,"__EGMTYPECAST_");
          pos+=14; while (__codebuffer[pos]!='>') pos++;
          __codebuffer.replace(pos,1,"\xEE(");
          pos++;
          if (__codebuffer[pos+1]==')')
          __codebuffer.erase(pos,2);
       }
    }
    pos+=1;
  }
  return;
}*/


void parser_recode_repeat()
{
    int a=string_pos((char*)"<repeat>(",__codebuffer);
    while (a)
    {
        __codebuffer=string_insert(" <for>(<int>__ENIGMA_incThis<=>0;__ENIGMA_incThis<lt>",string_delete(__codebuffer,a,8),a);
        __syntaxbuffer=string_insert(" <for>(<int>ffffffffffffffff<=>0;ffffffffffffffff<lt>",string_delete(__syntaxbuffer,a,8),a);
        a+=46;
        a=parser_end_of_parenths(a)+1;
        __codebuffer=string_insert(";__ENIGMA_incThis<+=> 1)",__codebuffer,a);
        __syntaxbuffer=string_insert(";ffffffffffffffff<+=> 1)",__syntaxbuffer,a);
        a=string_pos((char*)"<repeat>(",__codebuffer);
    }
    __codebuffer=string_replace_all(__codebuffer,"  "," ");
    __syntaxbuffer=string_replace_all(__syntaxbuffer,"  "," ");
}



void parser_remove_for_newlines()
{
    int pos,pos2;
    pos=string_pos((char*)"<for>",__codebuffer);
    for (int __ENIGMA_incThis=0;__ENIGMA_incThis<(string_count("<for>",__codebuffer));__ENIGMA_incThis+=1)
    {
        do pos+=1;
        while (!(__codebuffer[-1+pos]=='('));
        pos2=parser_end_of_parenths(pos);
        __codebuffer=string_copy(__codebuffer,1,pos)+string_replace_all(string_copy(__codebuffer,pos+1,pos2-pos-1)," ","")+string_copy(__codebuffer,pos2,string_length(__codebuffer));
        __syntaxbuffer=string_copy(__syntaxbuffer,1,pos)+string_replace_all(string_copy(__syntaxbuffer,pos+1,pos2-pos-1)," ","")+string_copy(__syntaxbuffer,pos2,string_length(__syntaxbuffer));
        pos+=string_pos((char*)"<for>",string_copy(__codebuffer,pos,string_length(__codebuffer)))-1;
    }
}




void string_replace_stack(std::string argument0, std::string argument1)
{
    while (string_count(argument0,__codebuffer))
    __codebuffer=string_replace(__codebuffer,argument0,argument1);
    while (string_count(argument0,__syntaxbuffer))
    __syntaxbuffer=string_replace(__syntaxbuffer,argument0,argument1);
}





void parser_remove_spaces()
{
    string_replace_stack("  "," ");
    string_replace_stack(" +","+");
    string_replace_stack("+ ","+");
    string_replace_stack(" -","-");
    string_replace_stack("- ","-");
    string_replace_stack(" *","*");
    string_replace_stack("* ","*");
    string_replace_stack(" /","/");
    string_replace_stack("/ ","/");
    string_replace_stack(" ,",",");
    string_replace_stack(", ",",");
    string_replace_stack(" <","<");
    string_replace_stack("< ","<");
    string_replace_stack(" >",">");
    string_replace_stack("> ",">");
    string_replace_stack(" =","=");
    string_replace_stack("= ","=");
    string_replace_stack(" !","!");
    string_replace_stack("! ","!");
    string_replace_stack(" ;",";");
    string_replace_stack("; ",";");
    //std::cout << "Checkpoint 1 \r\n" ;
    __codebuffer=string_replace_all(__codebuffer,";","; ");
    //std::cout << "Checkpoint 2 \r\n" ;
    string_replace_stack(" (","(");
    string_replace_stack("( ","(");
    string_replace_stack(" )",")");
    string_replace_stack(" {","{");
    //std::cout << "Checkpoint 3 \r\n" ;
    string_replace_stack("{ ","{");
    //std::cout << "Checkpoint 4 \r\n" ;
    __codebuffer=string_replace_all(__codebuffer,"{"," { ");
    //std::cout << "Checkpoint 5 \r\n" ;
    string_replace_stack(" }","}");
    //std::cout << "Checkpoint 6 \r\n" ;
    string_replace_stack("} ","}");
    //std::cout << "Checkpoint 7 \r\n" ;
    __codebuffer=string_replace_all(__codebuffer,"}"," } ");
    //std::cout << "Checkpoint 8, done \r\n" ;
}








int parser_remove_strings()
{
  int strcount=0;
  unsigned int pos=0,len=__codebuffer.length();
  std::string str;

  while (pos<len)
  {
    if (__codebuffer[pos]=='"')
    {
      str=""; int poss=pos; pos++;
      while (pos<len && __codebuffer[pos]!='"')
      {
        if (__codebuffer[pos]=='\\') { str+="\\\\"; pos++; continue; }
        if (__codebuffer[pos]=='\"') { str+="\\\""; pos++; continue; }
        if (__codebuffer[pos]=='\'') { str+="\\\'"; pos++; continue; }
        if (__codebuffer[pos]=='\r') { str+="\\r";  pos++; continue; }
        if (__codebuffer[pos]=='\n') { str+="\\n";  pos++; continue; }
        if (__codebuffer[pos]=='\t') { str+="\\t";  pos++; continue; }
        str+=__codebuffer[pos];
        pos++;
      }
      __codebuffer.erase(poss,pos-poss+1);
      pos=poss;
      __codebuffer.insert(pos,chr(240));

      __stringincode[strcount]=str; strcount++;
      len=__codebuffer.length();
    }
    if (__codebuffer[pos]=='\'')
    {
      str=""; int poss=pos; pos++;
      while (pos<len && __codebuffer[pos]!='\'')
      {
        if (__codebuffer[pos]=='\\') { str+="\\\\"; pos++; continue; }
        if (__codebuffer[pos]=='\"') { str+="\\\""; pos++; continue; }
        if (__codebuffer[pos]=='\'') { str+="\\\'"; pos++; continue; }
        if (__codebuffer[pos]=='\r') { str+="\\r";  pos++; continue; }
        if (__codebuffer[pos]=='\n') { str+="\\n";  pos++; continue; }
        if (__codebuffer[pos]=='\t') { str+="\\t";  pos++; continue; }
        str+=__codebuffer[pos];
        pos++;
      }
      __codebuffer.erase(poss,pos-poss+1);
      pos=poss;
      __codebuffer.insert(pos,chr(240));

      __stringincode[strcount]=str; strcount++;
      len=__codebuffer.length();
    }
    pos++;
  }
  return 0;
}

unsigned int findnotstr(std::string findthis,unsigned int poss)
{
  unsigned int pos=poss,len=__codebuffer.length(),lenn=findthis.length();
  while (pos<len)
  {
   if (__codebuffer[pos]=='"')  { pos++; while (pos<len && __codebuffer[pos]!='"')  pos++; pos++; continue; }
   if (__codebuffer[pos]=='\'') { pos++; while (pos<len && __codebuffer[pos]!='\'') pos++; pos++; continue; }

   if (__codebuffer.substr(pos,lenn)==findthis) return pos;
   pos++;
  }
  return string::npos;
}



std::string parse_asm(std::string code)
{
   unsigned int pos,len,placed1st=0; pos=len=code.length();
   int varcount=0; std::map<std::string,int> varnames;

   for (int i=0;i<3;i++)
   { pos--; while (pos>0 && code[pos]!=':') pos--; }

   if (code[pos]!=':') { puts("Three colons expected in asm to parse...\r\n"); return "/* Invalid ASM block */"; }

   for (int i=0;i<3;i++)
   {
     pos++;
     while (pos<len && code[pos]!=':')
     {
       if (code[pos]=='(')
       {
         pos++;
         int posi=pos;
         while (pos<len && code[pos]!=')') pos++;

         std::string nvar=code.substr(posi,pos-posi);

         for (unsigned int ii=0;ii<nvar.length();ii++)
         { if (!is_letterd(nvar[ii])) { nvar.erase(ii,1); ii--; } }

         int used=0;
         for (std::map<std::string,int>::iterator ii=varnames.begin();ii!=varnames.end();ii++)
         { if ((*ii).first==nvar) { used=1; break; } }

         if (!used) varnames[nvar]=varcount; varcount++;
         pos++;
       }
       pos++;
     }
   }

   int vc=0;
   for(std::map<std::string,int>::iterator i=varnames.begin();i != varnames.end();i++)
   { printf("%s\r\n",(*i).first.c_str()); vc++; }
   printf("endl, %d total\r\n\r\n",vc);

   code.erase(0,1);

   code="asm(\r\n\".intel_syntax\";\r\n"+code;
   pos=std::string("asm(\r\n\".intel_syntax\";\r\n").length();
   placed1st=0;

   while (pos<(len=code.length()))
   {
     int ispos=pos;
     while (pos<len && (code[pos]==' ' || code[pos]=='\t')) pos++;

     if (code[pos]==':')
     { code.insert(pos,"\".att_syntax\";\r\n"+code.substr(ispos,pos-ispos)); break; }

     if (code[pos]!='"') { code.insert(pos,"\""); placed1st=pos; } pos++;
     while (pos<len && (code[pos]==' ' || code[pos]=='\t')) pos++;

     while (pos<len && is_letterd(code[pos])) pos++;
     while (pos<len && (code[pos]==' ' || code[pos]=='\t')) pos++;

     while (pos<len && (code[pos]!='\r' && code[pos]!='\n'))
     {
         if is_letter(code[pos])
         {
           int poss=pos;
           while (pos<len && is_letterd(code[pos])) pos++;
           std::string varstr=code.substr(poss,pos-poss);
           if (varnames.find(varstr)!=varnames.end())
           {
             code.erase(poss,varstr.length());
             code.insert(poss,"%"+tostring(varnames[varstr]));
             pos=poss+1; while (pos<len && is_digit(code[pos])) pos++;
           }
         }
         else pos++;
     }

     unsigned int posm=pos-1;
     while (code[posm]==' ' || code[posm]=='\t') posm--;

     if (code[posm]!='"' && code[posm]!=';') posm++;

     if (code[posm]==';') { posm--; while (code[posm]==' ' || code[posm]=='\t') posm--; }

     if (placed1st==posm) posm++;

     if (placed1st) if (code[posm]!='"')
     { code.insert(posm,"\""); pos++; } posm++;

     if (placed1st) if (code[posm]!=';')
     { code.insert(posm,";"); pos++; }

     placed1st=0;

     pos+=2;
   }

   code.erase(code.length()-2,1);
   code+=");";

   return code;
}



std::string language_random;
std::map<int,std::string> languages;
int langcount;
void parser_store_other_languages()
{
  unsigned int pos;

  langcount=0;
  char lolwut[17];
  for (int i=0;i<16;i++)
  { if (rand()%5<=1) lolwut[i]=rand()%25+'a'; else if (rand()%5==4) lolwut[i]=rand()%9+'0'; else lolwut[i]=rand()%25+'A'; }
  lolwut[16]=0;
  language_random="otro_lingua_"; language_random+=lolwut; language_random+="();";

  pos=0;
  while ((pos=findnotstr("cpp",pos)) != string::npos)
  {
    int spos=pos;

    pos+=3; char a=__codebuffer[pos];
    if ((a>='a' && a<='z') || (a>='A' && a<='Z') || a=='_') continue;

    while (is_useless(__codebuffer[pos])) pos++;
    if (__codebuffer[pos] != '{') { parser_potential_error(100,1,1); return; }

    int scpos=pos; pos=parser_end_of_braces(pos);
    languages[langcount++]=__codebuffer.substr(scpos,pos-scpos+1);

    __codebuffer.erase(spos,pos-spos+1);
    __codebuffer.insert(spos,language_random);

    pos=spos;
  }

  pos=0;
  while ((pos=findnotstr("asm",pos)) != string::npos)
  {
    int spos=pos;

    pos+=3; char a=__codebuffer[pos];
    if ((a>='a' && a<='z') || (a>='A' && a<='Z') || a=='_') continue;

    while (is_useless(__codebuffer[pos])) pos++;
    if (__codebuffer[pos] != '{') { parser_potential_error(100,1,1); return; }

    int scpos=pos; pos=parser_end_of_braces(pos);
    std::string asmcode=__codebuffer.substr(scpos,pos-scpos+1);
    languages[langcount++]=parse_asm(asmcode);

    __codebuffer.erase(spos,pos-spos+1);
    __codebuffer.insert(spos,language_random);

    pos=spos;
  }
}



void parser_replace(std::string argument0,std::string argument1)
{
__codebuffer=string_replace_all(__codebuffer,argument0,argument1);
__syntaxbuffer=string_replace_all(__syntaxbuffer,argument0,argument1);
}



void parser_restore_strings()
{
  int snum=0;
  while (string_count(chr(240),__codebuffer))
  {
    __codebuffer=string_replace(__codebuffer,chr(240),"\""+string_replace_all(__stringincode[snum],chr(239)+"r"+chr(239)+"n","\\r\\n")+"\"");
    snum+=1;
  }
}
void parser_concatenate_strings()
{
  unsigned int pos=0,len=__codebuffer.length();
  while (pos<len)
  {
    if (__codebuffer[pos]=='"')
    {
      pos++; while (__codebuffer[pos]!='"' || __codebuffer[pos-1]=='\\') pos++; pos++;
      if (__codebuffer[pos]=='+' && __codebuffer[pos+1]=='"')
      __codebuffer.erase(pos-1,3);
    }
    pos++;
  }
}




void parser_restore_languages()
{
  unsigned int pos=0,len=language_random.length(); int num=0;
  while ((pos=__codebuffer.find(language_random,pos))!=std::string::npos)
  {
    __codebuffer.erase(pos,len);
    __codebuffer.insert(pos,languages[num]);
    num++;
  }
}





int parser_start_of_braces(unsigned int pos)
{
    int par,e;
    par=0;
    e=0;
    do
    {
        char strchr4=__codebuffer[-1+pos];
        if (strchr4=='{')
            {
                par+=1;
                e=1;
            }
        if (strchr4=='}')
        {
            par-=1;
        }
        if (par==0&&e==1)
        return (pos);
        pos-=1;
    }
    while (!(pos>string_length(__codebuffer)));
    return -1;
}


int parser_start_of_brackets(unsigned int pos)
{
    int par,e;
    par=0;
    e=0;
    do
    {
        char strchr5=__codebuffer[-1+pos];
        if (strchr5=='[')
        {
            par+=1;
            e=1;
        }
        if (strchr5==']')
        {
            par-=1;
        }
        if (par==0&&e==1)
        return (pos);
        pos-=1;
    }
    while (!(pos>string_length(__codebuffer)));
    return -1;
}


int parser_start_of_parenths(unsigned int pos)
{
    int par,e;
    par=0;
    e=0;
    do
    {
        char strchr6=__codebuffer[-1+pos];
        if (strchr6=='(')
        {
            par+=1;
            e=1;
        }
        if (strchr6==')')
        {
            par-=1;
        }
        if (par==0&&e==1)
        return (pos);
        pos-=1;
    }
    while (!(pos>string_length(__codebuffer)));
    return -1;
}


int string_isnt_operator(int argument0,int argument1)
{
    std::string strseg13[5];
    strseg13[3]=string_copy(__codebuffer,argument0+argument1*2,3);
    strseg13[4]=string_copy(__codebuffer,argument0+argument1*3,4);
    strseg13[5]=string_copy(__codebuffer,argument0+argument1*4,5);
    if (!(strseg13[4]=="<lt>")
      &&!(strseg13[4]=="<gt>")
      &&!(strseg13[4]=="<l=>")
      &&!(strseg13[4]=="<g=>")
      &&!(strseg13[4]=="<lt>")
      &&!(strseg13[4]=="<==>")
      &&!(strseg13[4]=="<!=>")
      &&!(strseg13[3]=="<=>")
      &&!(strseg13[4]=="<&&>")
      &&!(strseg13[4]=="<||>")
      &&!(strseg13[5]=="<lsh>")
      &&!(strseg13[5]=="<rsh>")
      &&!(strseg13[3]=="<!>")
      &&!(strseg13[3]=="<%>")
      &&!(strseg13[5]=="<div>")
      &&!(strseg13[4]=="<^^>"))
    return (1);
    return (0);
}






void parser_convert_switch(unsigned int pos,int argument1)
{
    int in;                                                //012345678901234567890123456789
    __codebuffer = string_replace(__codebuffer,  "<switch>"," <repeat>(1) { enigma::switchv");
    __syntaxbuffer=string_replace(__syntaxbuffer,"<switch>"," <repeat>(0) { fffffffffffffff");
    in=0;

    pos+=29;
    do pos+=1; while (!(__codebuffer[-1+pos]=='{'||pos>string_length(__codebuffer)));
    unsigned int endpos=parser_end_of_braces(pos);
    __codebuffer.erase(pos-1,1); __syntaxbuffer.erase(pos-1,1);

    pos+=1;
    while ((pos<endpos))
    {
        if (string_copy(__codebuffer,pos,1)=="{")
        {
            pos=parser_end_of_braces(pos);
        }
        if (string_copy(__codebuffer,pos,6)=="<case>")
        {
            __codebuffer = string_insert("(enigma::casev",string_delete(__codebuffer,pos,6),pos);
            __syntaxbuffer=string_insert("(fffffffffffff",string_delete(__syntaxbuffer,pos,6),pos);
            endpos+=8;
            __codebuffer=string_insert(")",__codebuffer,parser_end_of_parenths(pos+1));
            __syntaxbuffer=string_insert(")",__syntaxbuffer,parser_end_of_parenths(pos+1));
            endpos+=1;
            if (in)
            {
                __codebuffer = string_insert(" } ",__codebuffer,pos);
                __syntaxbuffer=string_insert(" } ",__syntaxbuffer,pos);
                pos+=3;
                endpos+=3;
            }
            __codebuffer = string_insert("<if>",__codebuffer,pos);
            __syntaxbuffer=string_insert("<if>",__syntaxbuffer,pos);
            pos+=4;
            endpos+=4;
            pos+=15;
            __codebuffer = string_insert(        tostring(argument1)            +              ",",__codebuffer,  pos);
            __syntaxbuffer=string_insert(string_repeat("0",string_length(tostring(argument1)))+",",__syntaxbuffer,pos);
            endpos+=string_length(tostring(argument1))+1;
            pos-=15+string_length(tostring(argument1))+1;
            pos=parser_end_of_parenths(pos);
            pos+=1;
            __codebuffer = string_insert(" { ",__codebuffer,pos);
            __syntaxbuffer=string_insert(" { ",__syntaxbuffer,pos);
            pos+=2;
            endpos+=3;
            //ifd=1;
            in=1;
        }
        if (string_copy(__codebuffer,pos,9)=="<default>")
        {
           __codebuffer = string_delete(__codebuffer,  pos,9);
           __syntaxbuffer=string_delete(__syntaxbuffer,pos,9);
           endpos-=9;
           if (in)
           {
                __codebuffer = string_insert(" } ",__codebuffer,  pos);
                __syntaxbuffer=string_insert(" } ",__syntaxbuffer,pos);
                pos+=3;
                endpos+=3;
           }
           __codebuffer = string_insert("<if>(enigma::defaultv("+tostring(argument1)+")) { ",__codebuffer,pos);
           __syntaxbuffer=string_insert("<if>(ffffffffffffffff("+string_repeat("0",string_length(tostring(argument1)))+")) { ",__syntaxbuffer,pos);
           pos   +=27+string_length(tostring(argument1));
           endpos+=27+string_length(tostring(argument1));
           //ifd=0;
           in=1;
        }
        pos+=1;
    }
    if (in)
    {
        __codebuffer = string_insert(" } ",__codebuffer,  pos);
        __syntaxbuffer=string_insert(" } ",__syntaxbuffer,pos);
        endpos+=3;
    }
}








void parser_check_for_loop(unsigned int strpos,int argument1=0,int argument2=0)
{
    int temppos,parcount,sec,opt;
    std::string strseg9[3], strseg10, strseg11;
    temppos=strpos;
    opt=0;
    parcount=0;
    do
    {
        strpos+=1;
    }
    while (!(__codebuffer[-1+strpos]=='('));
    parcount+=1;
    sec=0;
    do
    {
        strseg9[1]=__codebuffer[-1+strpos];
        strseg9[2]=string_copy(__codebuffer,strpos,2);
        if (strseg9[1]==";"||strseg9[2]==" "||strseg9[2]=="0n")
        {
            sec+=1;
            opt=0;
        }
        strseg10=string_copy(__codebuffer,strpos,4);
        if (strseg10=="<==>"||strseg10=="<!=>"||strseg10=="<g=>"||strseg10=="<l=>"||strseg10=="<lt>"||strseg10=="<gt>")
        {
            if (opt==0)
            opt=2;
            sec+=1;
        }
        strseg11=string_copy(__codebuffer,strpos,4);
        if (strseg11=="<+=>"||strseg11=="<-=>"||strseg11=="<*=>"||strseg11=="</=>")
        {
            if (opt==0)
            opt=1;
            sec+=1;
        }
        if (string_copy(__codebuffer,strpos,3)=="<=>")
        {
            if (opt==0)
            opt=1;
            if (sec==1)
            {
                switch(argument2)
                {
                    case 2:
                        break;
                    case 1:
                        parser_potential_error(strpos,1);
                        break;
                    case 0:
                        __codebuffer=string_insert("=",__codebuffer,strpos+1);
                        __syntaxbuffer=string_insert("=",__syntaxbuffer,strpos+1);
                        strpos+=1;
                        break;
                }
            }
        }
        strpos+=1;
    }
    while (!(parcount==0||strpos>string_length(__codebuffer)));
}




/* Add semicolons to the parsing code. It's that simple.
 ********************************************************/

void parser_add_semicolons()
{
  if (__codebuffer[0]==' ')
  { __codebuffer.erase(0,1); __syntaxbuffer.erase(0,1); }
  __codebuffer+=" "; __syntaxbuffer+=" ";

  string_replace_stack("  "," ");
  string_replace_stack("{ ","{");
  string_replace_stack(" {","{");
  string_replace_stack(" }","}");
  string_replace_stack(")}",");}");
  string_replace_stack("){",");{");
  string_replace_stack("]}","];}");
  string_replace_stack("]{","];{");
  string_replace_stack("} ","}");
  string_replace_stack("> ",">");
  string_replace_stack(" ;",";");
  string_replace_stack("; ",";");
  string_replace_stack(" ",";");



  for (unsigned int i=0; (i=__syntaxbuffer.find("0n",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0f",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find(")n",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("]n",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find(")f",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("]f",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("n}",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0}",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0}",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("n{",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0{",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0{",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find(chr(240)+"f",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find(chr(240)+"n",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }

  for (unsigned int i=0; (i=__syntaxbuffer.find("0<if>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<with>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<for>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<while>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<switch>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<else>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<repeat>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<var>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<double>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<float>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<int>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<char>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<bool>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<cstrnng>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<long>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<short>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<signed>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<unsigned>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<const>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<static>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<localv>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<new>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<delete>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }

  for (unsigned int i=0; (i=__syntaxbuffer.find("0<do>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<until>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<then>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<break>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<begin>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<end>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<continue>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<case>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<default>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<exit>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("0<return>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }



  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<if>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<with>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<for>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<while>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<switch>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<else>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<repeat>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<var>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<double>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<float>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<int>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<char>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<bool>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<cstring>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<long>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<short>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<signed>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<unsigned>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<const>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<static>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<localv>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<new>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<delete>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }

  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<do>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<until>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<then>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<break>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<begin>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<end>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<continue>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<case>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<default>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<exit>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }
  for (unsigned int i=0; (i=__syntaxbuffer.find("\xF0<return>",i))!=std::string::npos; i++)
    { __codebuffer.insert(i+1,";"); __syntaxbuffer.insert(i+1,";"); }



  for (unsigned int i=0; (i=__syntaxbuffer.find("<if>",i))!=std::string::npos; i++)
  { while (__codebuffer[i]!='>') i++; i=parser_end_of_parenths(i+2);
    if (__codebuffer[i]==';') { __codebuffer[i]=' ';  __syntaxbuffer[i]=' '; } }
  for (unsigned int i=0; (i=__syntaxbuffer.find("<with>",i))!=std::string::npos; i++)
  { while (__codebuffer[i]!='>') i++; i=parser_end_of_parenths(i+2);
    if (__codebuffer[i]==';') { __codebuffer[i]=' ';  __syntaxbuffer[i]=' '; } }
  for (unsigned int i=0; (i=__syntaxbuffer.find("<for>",i))!=std::string::npos; i++)
  { while (__codebuffer[i]!='>') i++; i=parser_end_of_parenths(i+2);
    if (__codebuffer[i]==';') { __codebuffer[i]=' ';  __syntaxbuffer[i]=' '; } }
  for (unsigned int i=0; (i=__syntaxbuffer.find("<while>",i))!=std::string::npos; i++)
  { while (__codebuffer[i]!='>') i++; i=parser_end_of_parenths(i+2);
    if (__codebuffer[i]==';') { __codebuffer[i]=' ';  __syntaxbuffer[i]=' '; } }
  for (unsigned int i=0; (i=__syntaxbuffer.find("<repeat>",i))!=std::string::npos; i++)
  { while (__codebuffer[i]!='>') i++; i=parser_end_of_parenths(i+2);
    if (__codebuffer[i]==';') { __codebuffer[i]=' ';  __syntaxbuffer[i]=' '; } }
  for (unsigned int i=0; (i=__syntaxbuffer.find("<until>",i))!=std::string::npos; i++)
  { while (__codebuffer[i]!='>') i++; i=parser_end_of_parenths(i+2);
    if (__codebuffer[i]==';') { __codebuffer[i]=' ';  __syntaxbuffer[i]=' '; } }
  for (unsigned int i=0; (i=__syntaxbuffer.find("<switch>",i))!=std::string::npos; i++)
  { while (__codebuffer[i]!='>') i++; i=parser_end_of_parenths(i+2);
    if (__codebuffer[i]==';') { __codebuffer[i]=' ';  __syntaxbuffer[i]=' '; } }
  for (unsigned int i=0; (i=__syntaxbuffer.find("<case>",i))!=std::string::npos; i++)
  { while (__codebuffer[i]!='>') i++; i=parser_end_of_parenths(i+2);
    if (__codebuffer[i]==';') { __codebuffer[i]=' ';  __syntaxbuffer[i]=' '; } }



  parser_replace(">);",">)");


  string_replace_stack(";;",";");
}









void parser_identify_statement(std::string argument0)
{
    unsigned int pos=0,offset=0;
    std::string cpy=__codebuffer;

    //parser_potential_error(ERR_UNEXPECTED,string_pos("<"+argument0+">",__codebuffer),string_count("<"+argument0+">",__codebuffer));

    int sc=string_count(argument0,cpy);
    for (int zzz=0;zzz<sc;zzz+=1)
    {
        pos=string_pos(argument0,cpy);

        if (pos==0)
        break;
        if ((!is_lettersdigits(cpy[-1+pos-1])
           ||is_of_digits(cpy[-1+pos-1])
           ||pos==1)
           &&(!is_lettersdigits(cpy[-1+pos+string_length(argument0)])
           ||pos+string_length(argument0)>=string_length(__codebuffer)))
        {
            __codebuffer=
            string_copy(__codebuffer,1,pos-1)+
            "<"+argument0+">"+
            string_copy(__codebuffer,pos+string_length(argument0),string_length(__codebuffer));
        }

        cpy=
        string_repeat(" ",pos)+
        string_repeat(" ",string_length("<"+argument0+">"))+
        string_copy(__codebuffer,pos+3+string_length(argument0),string_length(cpy));

        offset=0;
    }
    string_replace_stack("  "," ");
    parser_replace("<"+argument0+"> ","<"+argument0+">");
    parser_replace("<"+argument0+">"," <"+argument0+">");
}












void parser_infinite_arguments(std::string argument0)
{
    int pos,pos2,pend,ccount,lcomma=0,pnext;
    pos=string_pos(argument0,__codebuffer);
    while ((pos>0))
    {
        pos+=string_length(argument0)-1;
        if (__syntaxbuffer[-1+pos-string_length(argument0)]!='f')
        {
            pos2=pos+1;
            pend=parser_end_of_parenths(pos);
            ccount=0;
            while (pos2<pend)
            {
                if (__codebuffer[-1+pos2]==',')
                {
                    ccount+=1;
                    lcomma=pos2;
                }
                if (__codebuffer[-1+pos2]=='(')
                pos2=parser_end_of_parenths(pos2);
                if (__codebuffer[-1+pos2]=='[')
                pos2=parser_end_of_brackets(pos2);
                pos2+=1;
            }
            if ((ccount>1))
            {
                __codebuffer=string_insert(")",__codebuffer,lcomma);
                __syntaxbuffer=string_insert(")",__syntaxbuffer,lcomma);
                __codebuffer=string_insert(argument0,__codebuffer,pos+1);
                __syntaxbuffer=string_insert(argument0,__syntaxbuffer,pos+1);
            }
        }
        pnext=string_pos(argument0,string_copy(__codebuffer,pos,string_length(__codebuffer)));
        pos+=pnext-1;
        if ((pnext==0))
        pos=0;
    }
}







void parser_num_arguments(std::string argument0)
{
    int pos,pos2,pend,ccount,lcomma,pnext;
    pos=string_pos(argument0,__codebuffer);
    while ((pos>0))
    {
        pos+=string_length(argument0)-1;
        if ((__syntaxbuffer[-1+pos-string_length(argument0)]!='f'))
        {
            pos2=pos+1;
            pend=parser_end_of_parenths(pos);
            ccount=0;
            while ((pos2<pend))
            {
                if ((__codebuffer[-1+pos2]==','))
                {
                    ccount+=1;
                    lcomma=pos2;
                }
                if (__codebuffer[-1+pos2]=='(')
                pos2=parser_end_of_parenths(pos2);
                if ((__codebuffer[-1+pos2]=='['))
                pos2=parser_end_of_brackets(pos2);
                pos2+=1;
            }
            __codebuffer=string_insert(tostring(ccount+1)+",",__codebuffer,pos+1);
            __syntaxbuffer=string_insert(argument0,__syntaxbuffer,pos+1);
        }
        pnext=string_pos(argument0,string_copy(__codebuffer,pos,string_length(__codebuffer)));
        pos+=pnext-1;
        if ((pnext==0))
        pos=0;
    }
}



void parser_parse_dots()
{
    unsigned int pos;
    std::string chkthat;
    pos=string_pos(chr(238),__codebuffer);
    while ((pos>0))
    {
        if ((__syntaxbuffer[-1+pos+1]=='0'))
        {
            __codebuffer=string_replace(__codebuffer,chr(238),".");
            pos=string_pos(chr(238),__codebuffer);
            continue;
        }
        __codebuffer=string_insert(").",string_delete(__codebuffer,pos,1),pos);
        __syntaxbuffer=string_insert(").",string_delete(__syntaxbuffer,pos,1),pos);

        int poseov=pos+1; char nextsymb=__syntaxbuffer[poseov];
        while (__syntaxbuffer[poseov]==nextsymb) poseov++;
        __codebuffer . insert(poseov,")");
        __syntaxbuffer.insert(poseov,")");

        pos-=1;
        while ((pos>0))
        {
            if (__codebuffer[-1+pos]==')')
            {
                pos=parser_start_of_parenths(pos);
                if (__syntaxbuffer[-1+pos-1]!='f')
                break;
                pos-=1;
            }
            if (__codebuffer[-1+pos]=='>')
            if (__codebuffer[-1+pos-1]!='-')
            {
                pos+=1;
                break;
            }
            chkthat=string_copy(__syntaxbuffer,pos-1,2);
            if (chkthat=="0n"||chkthat=="0f"||chkthat==chr(240)+"n"||chkthat==chr(240)+"f"||chkthat==")0"||chkthat=="0("||chkthat==")n"||chkthat=="n("||chkthat==")f"||chkthat==")(")
            {
                break;
            }
            if ((__syntaxbuffer[-1+pos]!='f')
              &&(__syntaxbuffer[-1+pos]!='n')
              &&(__syntaxbuffer[-1+pos]!='0')
              &&(__syntaxbuffer[-1+pos]!='>')
              &&(__syntaxbuffer[-1+pos]!='.')
              &&!(__syntaxbuffer[-1+pos]=='-'
              &&(__syntaxbuffer[-1+pos-1]==' ' || __syntaxbuffer[-1+pos-1]=='\r' || pos-1<1)))
            {
                pos+=1;
                break;
            }
            pos-=1;
        }
        __codebuffer = string_insert("(*enigma::int2object(", __codebuffer, pos);
        __syntaxbuffer=string_insert("(*ffffffffffffffffff(",__syntaxbuffer,pos);
        pos=string_pos(chr(238),__codebuffer);
    }
}






void parser_parse_multidimensional(int argument0)
{
  unsigned int _pos,pos,endpos=0;
  _pos=pos=argument0;
  int par,e;
  par=0;
  e=0;
  int needmulti=0;

  while (!(pos>string_length(__codebuffer)))
  {
    char strchr8=__codebuffer[-1+pos];
    if (strchr8=='[')
    {
        par+=1;
        e=1;
    }
    if (strchr8==']')
    {
        par-=1;
    }
    if (strchr8==',' && par==1 && e==1)
    needmulti=1;
    if (par==0&&e==1)
    { endpos=pos; break; }
    pos+=1;
  }


  if (needmulti)
  {
    __codebuffer=string_insert("(",string_delete(__codebuffer,_pos,1),_pos);
    __syntaxbuffer=string_insert("(",string_delete(__syntaxbuffer,_pos,1),_pos);
    __codebuffer=string_insert(")",string_delete(__codebuffer,endpos,1),endpos);
    __syntaxbuffer=string_insert(")",string_delete(__syntaxbuffer,endpos,1),endpos);
  }
}




void parser_manage_multidimensional()
{
    unsigned int pos=1;
    do
    {
        if (__codebuffer[-1+pos]=='[')
        {
            parser_parse_multidimensional(pos);
        }
        pos+=1;
    }
    while (!(pos>string_length(__codebuffer)));
}







int parser_parse_statement(int argument0,int argument2)
{
    unsigned int spos,isif;
    spos=argument0;
    if ((string_copy(__codebuffer,spos,4)=="<if>"))
    isif=1;
    else isif=0;

    while (!(__codebuffer[-1+spos]=='>'))
    spos+=1;

    __codebuffer=string_insert("(",__codebuffer,spos+1);
    __syntaxbuffer=string_insert("(",__syntaxbuffer,spos+1);

    while (spos<=string_length(__codebuffer)+1)
    {
        //std::cout << "parser_parse_statement checkpoint 3 \r\n"; system("PAUSE");
        if (__codebuffer[-1+spos]==';')
        {
            //std::cout << "parser_parse_statement checkpoint 4 <com";
            __codebuffer=string_delete(__codebuffer,spos,1);
            __syntaxbuffer=string_delete(__syntaxbuffer,spos,1);
            //std::cout << "plete> \r\n"; system("PAUSE");
        }
        if (__syntaxbuffer[-1+spos]==' '
            ||__syntaxbuffer[-1+spos]=='{'
            ||string_copy(__syntaxbuffer,spos,6)=="<then>"
            ||spos>string_length(__codebuffer))
            {
                //std::cout << "parser_parse_statement checkpoint 5 <com";
                parser_potential_error(ERR_UNEXPECTED,spos,(((string_copy(__codebuffer,spos,6)=="<then>")&&!(isif))));
                if (!(__codebuffer[-1+spos]==' '))
                {
                    __codebuffer=string_insert(") ",__codebuffer,spos);
                    __syntaxbuffer=string_insert(") ",__syntaxbuffer,spos);
                }
                else
                {
                    __codebuffer=string_insert(")",__codebuffer,spos);
                    __syntaxbuffer=string_insert(")",__syntaxbuffer,spos);
                }
                //std::cout << "plete> \r\n"; system("PAUSE");
                return (spos+1);
            }
            std::string strseg12;
            strseg12=string_copy(__syntaxbuffer,spos,2);
            if (strseg12=="0n"
            ||  strseg12=="0f"
            ||  strseg12==chr(240)+"n"
            ||  strseg12==chr(240)+"f"
            ||  strseg12==")0"
            ||  strseg12=="0("
            ||  strseg12==")n"
            ||  strseg12=="n("
            ||  strseg12==")f"
            ||  strseg12==")("
            ||  strseg12=="]n"
            ||  strseg12=="]f")
            {
                //std::cout << "parser_parse_statement checkpoint 6 <com";
                spos+=1;
                __codebuffer=string_insert(") ",__codebuffer,spos);
                __syntaxbuffer=string_insert(") ",__syntaxbuffer,spos);
                //std::cout << "plete> \r\n"; system("PAUSE");
                return (spos+1);
            }
            if (strseg12==")n")
            {
                //std::cout << "parser_parse_statement checkpoint 7 <com";
                spos+=1;
                __codebuffer=string_insert(") ",__codebuffer,spos);
                __syntaxbuffer=string_insert(") ",__syntaxbuffer,spos);
                //std::cout << "plete> \r\n"; system("PAUSE");
                return (spos+1);
            }
            if (string_copy(__codebuffer,spos,3)=="<=>")
            {
                switch(argument2)
                {
                    case 2:
                        break;

                    case 1:
                        //parser_potential_error(string_pos("<=>",argument0-1+string_pos("<=>",endcode)));
                        break;
                    case 0:
                        __codebuffer=string_insert("=",__codebuffer,spos+1);
                        __syntaxbuffer=string_insert("=",__syntaxbuffer,spos+1);
                        spos+=1;
                        break;

                }
            }
            spos+=1;
        }
      return string::npos;
}


int parser_patch_inconsistency(std::string argument0, std::string argument1)
{
    parser_potential_error(ERR_UNEXPECTED,string_pos(argument1,__codebuffer),string_count(argument1,__codebuffer));
    for (unsigned int pos=0;pos<__codebuffer.length();pos++)
    {
        pos=__codebuffer.find(argument0,pos);
        if (pos==std::string::npos) break;

        pos++;

        if ((!is_lettersdigits(__codebuffer[-1+pos-1])
           || is_of_digits(__codebuffer[-1+pos-1])||pos==1)
           &&!is_lettersdigits(__codebuffer[-1+pos+string_length(argument0)]))
        {
            __codebuffer=string_copy(__codebuffer,1,max(0,pos-1))+"<"+argument1+">"+string_copy(__codebuffer,pos+string_length(argument0),string_length(__codebuffer));
        }
    }
    __codebuffer=string_replace_all(__codebuffer," <"+argument1+">","<"+argument1+">");
    __codebuffer=string_replace_all(__codebuffer,"<"+argument1+"> ","<"+argument1+">");
    return string::npos;
}




/*
void parser_export(std::string argument0)
{
while (string_count(chr(13)+chr(13),__codebuffer))
__codebuffer=string_replace_all(__codebuffer,chr(13)+chr(13),chr(13));

__codebuffer=string_replace_all(__codebuffer,chr(13),"\n");

std::ofstream dump(argument0.c_str());
dump << __codebuffer;
free(dump);
}*/







void parser_replace_until()
{
     unsigned int a=__codebuffer.find("<until>((");
     while (a != string::npos)
     {
           __codebuffer.insert(parser_end_of_parenths(a+8),";");
           __codebuffer.erase(a,9);
           __codebuffer.insert(a,"while (!(");
           a=__codebuffer.find("<until>((");
     }
}




void parser_handle_new()
{
    int len=__codebuffer.length();
    for (unsigned int pos=0;(pos=__codebuffer.find("<new>",pos)) != std::string::npos;pos++)
    {
        int epos=pos;

        while (__codebuffer[epos] == '<')
        { while (epos<len && __codebuffer[epos]!='>') epos++; epos++; }

        __codebuffer.insert(epos,")");
        __codebuffer.insert(pos,"(");
        len+=2;
        pos++;
    }

    len=__codebuffer.length();
    for (unsigned int pos=0;(pos=__codebuffer.find("<delete>",pos)) != std::string::npos;pos++)
    {
        int epos=pos;

        while (epos<len && __codebuffer[epos]!='>') epos++; epos++;

        __codebuffer.insert(epos,")");
        __codebuffer.insert(pos,"(");
        len+=2;
        pos++;
    }
}

void parser_unhandle_new()
{
    int len=__codebuffer.length();
    for (unsigned int pos=0;(pos=__codebuffer.find("(new ",pos)) != std::string::npos;pos++)
    {
        int epos=parser_end_of_parenths(pos+1)-1;
        __codebuffer.erase(epos,2);
        __codebuffer.erase(pos,1);
        len-=2;
    }

    len=__codebuffer.length();
    for (unsigned int pos=0;(pos=__codebuffer.find("(delete)",pos)) != std::string::npos;pos++)
    {
        int epos=pos+7;
        __codebuffer.erase(epos,1);
        __codebuffer.erase(pos,1);
        len-=2;
    }
}





void parser_remove_newlines()
{
    for (unsigned int i=0; i<__codebuffer.length(); i++)
    if (__codebuffer[i]=='\r' || __codebuffer[i]=='\n')
    __codebuffer[i]=' ';
}



void parser_destroy_who()
{
  unsigned int pos=0;
  while ((pos=__codebuffer.find("instance_destroy",pos)) != string::npos)
  {
    if ((pos==0 || __syntaxbuffer[pos-1]) && __syntaxbuffer[pos]=='f' && __syntaxbuffer[pos+16]!='f')
    {
      pos+=15; while (__codebuffer[pos]!='(') pos++;
      int nspc=1; pos++;
      while (__codebuffer[pos]!=')') { if (__codebuffer[pos]!=' ') { nspc=0; break; } pos++; }
      if (nspc)
      {
        __codebuffer . insert(pos,"id");
        __syntaxbuffer.insert(pos,"nn");
      }
    }
  }
}



void parser_get_varnamelist(string& code,string& synt,string*& vns,string*& vnss,string*& vnts,string*& vnas,int &vnc);

void parser_develop_token_stream()
{
  //printf("seg-5, input:\r\n%s\r\n",__codebuffer.c_str());
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_store_other_languages();
  parser_remove_strings();
  parser_remove_newlines();



  //printf("seg-4\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  __codebuffer=string_replace_all(__codebuffer,"\r"," ");
  __codebuffer=string_replace_all(__codebuffer,"\n"," ");

  //printf("seg-3\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_remove_spaces();

  //printf("seg-2\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  string_replace_stack(" [","[");
  string_replace_stack("[ ","[");
  string_replace_stack(" ]","]");
  string_replace_stack("] ","]");
  string_replace_stack(". ",".");
  string_replace_stack(" .",".");
  parser_replace(".",chr(238));


  //printf("seg-1\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  while (string_count("++",__codebuffer)||string_count("++",__codebuffer)||string_count("+-",__codebuffer)||string_count("-+",__codebuffer))
  {
      __codebuffer=string_replace_all(string_replace_all(string_replace_all(string_replace_all(__codebuffer,"++","+"),"--","+"),"+-","-"),"-+","-");
  }
  //printf("seg0\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_replace("<","<lt>");
  parser_replace(">","<gt>");
  parser_replace("<lt<gt>","<lt>");
  parser_replace("<lt><gt>","<!=>");
  parser_replace("<lt>=","<l=>");
  parser_replace("<gt>=","<g=>");
  parser_replace("<lt><lt>","<lsh>");
  parser_replace("<gt><gt>","<rsh>");
  parser_replace("<lt><l=>","<ls=>");
  parser_replace("<gt><g=>","<rs=>");

  //printf("seg1\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_patch_inconsistency("and","<&&>");
  parser_patch_inconsistency("or","<||>");
  parser_patch_inconsistency("not","<!>");
  parser_patch_inconsistency("mod","<%>");
  parser_patch_inconsistency("div","<div>");
  parser_patch_inconsistency("xor","<^^>");

  //printf("seg2\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_replace("&&","<&&>");
  parser_replace("||","<||>");
  parser_replace("!","<!>");
  parser_replace("~","<~>");
  parser_replace("%","<%>");
  parser_replace("^^","<^^>");
  parser_replace("&","<&>");
  parser_replace("|","<|>");
  parser_replace("^","<^>");
  parser_replace("~","<~>");
  parser_replace("$","<$>");
  parser_replace("==","<==>");
  parser_replace(":=","<=>");
  parser_replace("+=","<+=>");
  parser_replace("-=","<-=>");
  parser_replace("*=","<*=>");
  parser_replace("/=","</=>");
  parser_replace("=","<=>");
  parser_replace("<=><=>","<==>");
  parser_replace("<!><=>","<!=>");
  parser_replace("<&><&>","<&&>");
  parser_replace("<|><|>","<||>");
  parser_replace("<^><^>","<^^>");
  parser_replace("<+<=>>","<+=>");
  parser_replace("<-<=>>","<-=>");
  parser_replace("<*<=>>","<*=>");
  parser_replace("</<=>>","</=>");
  parser_replace("<^><=>","<^=>");
  parser_replace("<&><=>","<&=>");
  parser_replace("<|><=>","<|=>");
  parser_replace("<<==>>","<==>");
  parser_replace("<l<=>","<l=>");
  parser_replace("<g<=>","<g=>");


  //printf("seg3\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_identify_statement("if");
  parser_identify_statement("with");
  parser_identify_statement("for");
  parser_identify_statement("while");
  parser_identify_statement("switch");
  parser_identify_statement("else");
  parser_identify_statement("repeat");
  parser_identify_statement("var");
  parser_identify_statement("double");
  parser_identify_statement("float");
  parser_identify_statement("int");
  parser_identify_statement("char");
  parser_identify_statement("bool");
  parser_identify_statement("cstring");
  parser_identify_statement("long");
  parser_identify_statement("short");
  parser_identify_statement("signed");
  parser_identify_statement("unsigned");
  parser_identify_statement("const");
  parser_identify_statement("static");
  parser_identify_statement("localv");
  parser_identify_statement("new");
  parser_identify_statement("delete");
  parser_replace("<delete>[]","<delete>");

  //printf("seg4\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  string_replace_stack("<var> ","<var>");
  string_replace_stack("<double> ","<double>");
  string_replace_stack("<float> ","<float>");
  string_replace_stack("<int> ","<int>");
  string_replace_stack("<char> ","<char>");
  string_replace_stack("<bool> ","<bool>");
  string_replace_stack("<cstring> ","<cstring>");
  string_replace_stack("<long> ","<long>");
  string_replace_stack("<short> ","<short>");
  string_replace_stack("<signed> ","<signed>");
  string_replace_stack("<unsigned> ","<unsigned>");
  string_replace_stack("<const> ","<const>");
  string_replace_stack("<static> ","<static>");
  string_replace_stack("<localv> ","<localv>");
  string_replace_stack("<new> ","<new>");
  string_replace_stack("<delete> ","<delete>");
  string_replace_stack(" <var>","<var>");
  string_replace_stack(" <double>","<double>");
  string_replace_stack(" <float>","<float>");
  string_replace_stack(" <int>","<int>");
  string_replace_stack(" <char>","<char>");
  string_replace_stack(" <bool>","<bool>");
  string_replace_stack(" <cstring>","<cstring>");
  string_replace_stack(" <long>","<long>");
  string_replace_stack(" <short>","<short>");
  string_replace_stack(" <signed>","<signed>");
  string_replace_stack(" <unsigned>","<unsigned>");
  string_replace_stack(" <const>","<const>");
  string_replace_stack(" <static>","<static>");
  string_replace_stack(" <localv>","<localv>");
  string_replace_stack("(<var>) ","(<var>)");
  string_replace_stack("(<double>) ","(<double>)");
  string_replace_stack("(<float>) ","(<float>)");
  string_replace_stack("(<int>) ","(<int>)");
  string_replace_stack("(<char>) ","(<char>)");
  string_replace_stack("(<bool>) ","(<bool>)");
  string_replace_stack("(<cstring>) ","(<cstring>)");
  string_replace_stack("(<long>) ","(<long>)");
  string_replace_stack("(<short>) ","(<short>)");
  string_replace_stack("(<signed>) ","(<signed>)");
  string_replace_stack("(<unsigned>) ","(<unsigned>)");
  string_replace_stack("(<const>) ","(<const>)");
  string_replace_stack("(<static>) ","(<static>)");
  string_replace_stack("(<localv>) ","(<localv>)");




  //printf("seg6\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_identify_statement("do");
  parser_identify_statement("until");
  parser_identify_statement("then");
  parser_identify_statement("break");
  parser_identify_statement("begin");
  parser_identify_statement("end");
  parser_identify_statement("continue");
  parser_identify_statement("case");
  parser_identify_statement("default");
  parser_identify_statement("exit");
  parser_identify_statement("return");

  //printf("seg7\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  //std::cout << "check 1";
  string_replace_stack("<<","<");
  //std::cout << "check 2";
  string_replace_stack(">>",">");
  //std::cout << "check 3";
  string_replace_stack(" <","<");
  //std::cout << "check 4";
  string_replace_stack("> ",">");
  //std::cout << "check 5";
  string_replace_stack(";;",";");
  //std::cout << "check 6";
  string_replace_stack(" ;",";");

  //printf("seg8\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_replace("<break>;","<break>");
  parser_replace("<continue>;","<continue>");
  parser_replace("<exit>;","<exit>");
  parser_replace("<begin>;","<begin>");
  parser_replace("<end>;","<end>");
  parser_replace("<begin>"," { ");
  parser_replace("<end>"," } ");
  parser_replace("<if>"," <if>");
  parser_replace("<with>"," <with>");
  parser_replace("<for>"," <for>");
  parser_replace("<while>"," <while>");
  parser_replace("<switch>"," <switch>");
  parser_replace("<repeat>"," <repeat>");
  parser_replace("<var>"," <var>");
  parser_replace("<do>"," <do>");
  parser_replace("<until>"," <until>");
  parser_replace("<then>"," <then>");
  parser_replace("<break>"," <break>");
  parser_replace("<continue>"," <continue>");
  parser_replace("<case>"," <case>");
  parser_replace("<default>"," <default>");
  parser_replace("<else>"," <else>");
  parser_replace("<exit>"," <exit>");
  parser_replace("<return>"," <return> ");

  //printf("seg9\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  string_replace_stack("  "," ");
  string_replace_stack("<return> ","<return>");



  //printf("seg5\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  //parser_handle_new();
  //parser_recast_unify();


  //printf("seg10\r\n");
  //std::cout << __codebuffer << "\r\n"; system("PAUSE");

  parser_buffer_syntax_map();



  /* Now that we have a syntax buffer, we can easily list all the variable names.
  |* They are all represented by a string of 'n' characters in the syntax buffer.
  **********************************************************************************/
                  //list(string& code,string& synt, string* vns,  string* vnss, string* vnts,   string* vnas,  int &vnc
  parser_get_varnamelist(__codebuffer,__syntaxbuffer,__varnames,__varnamestars,__varnametags,__varnamearrays,__varcount);
}



void parser_buffer_remove_linecomments(std::string code)
{
  unsigned int len=code.length();
  unsigned int pos=0;

  while (pos<len)
  {
    if (code[pos]=='/' && code[pos+1]=='/')
    { while (code[pos]!='\r' && code[pos]!='\n') pos++; continue; }

    if (code[pos]=='/' && code[pos+1]=='*')
    { pos++; while (code[pos]!='*' || code[pos+1]!='/') pos++; pos+=2; continue; }

    if (code[pos]=='"')
    { __codebuffer+=code[pos]; pos++; while (code[pos]!='"') { __codebuffer+=code[pos]; pos++; } __codebuffer+=code[pos]; pos++; continue; }

    if (code[pos]=='\'')
    { __codebuffer+=code[pos]; pos++; while (code[pos]!='\'') { __codebuffer+=code[pos]; pos++; } __codebuffer+=code[pos]; pos++; continue; }

    __codebuffer+=code[pos];
    pos++;
  }

  __codebuffer+=" ";
}








void parser_get_varnamelist(string& code,string& synt,string*& vns,string*& vnss,string*& vnts,string*& vnas,int &vnc)
{
   std::map<int,string> names,nametags,namestars,namearrays,locals;
   std::map<int,bool>   dots;

   unsigned int pos=0,spos,epos,cpos;
   int ncount=0,lcount=0,used,isntlocal,dotted;
   string checker;

   while ((pos=synt.find("<localv>",pos))!=string::npos)
   {
     code.erase(pos,8);
     synt.erase(pos,8);
     int start_position=pos;
     int posi=pos;

     //Get the type of the local
     while (code[pos]=='<') { while (code[pos]!='>') pos++; pos++; }
     string typetoadd=code.substr(posi,pos-posi);

     //make that into C++ (detokenize it)
     {
       unsigned int detokpos=0;
       while (detokpos<typetoadd.length())
       {
         if (typetoadd[detokpos]=='<') typetoadd.erase(detokpos,1);
         else if (typetoadd[detokpos]=='>') { typetoadd[detokpos]=' '; }
         else detokpos++;
       }
     }

     //remove the type names
     code.erase(posi,pos-posi);
     synt.erase(posi,pos-posi);

     //Revert back to where name was
     pos=posi;

     //Until the end of the list
     while (code[pos]!=';')
     {
       //Be safe.
       pos=posi;

       //reset the unary
       unsigned int unarypos;
       while ((unarypos=typetoadd.find("*",0)) != string::npos) { typetoadd.erase(unarypos,1); }

       //remember that * is a valid character, as well as (
       namestars[ncount]="";
       while (pos<code.length() && (code[pos]=='*' || code[pos]=='('))
       {
         namestars[ncount]+=code[pos];
         code.erase(pos,1);
         synt.erase(pos,1);
       }

       //Not seeing a variable name...
       if (synt[pos]!='n')
       {
         //Clean up and leave
         parser_potential_error(ERR_UNEXPECTED,0,synt[pos]!=';');
         code.erase(posi,pos-posi+1);
         synt.erase(posi,pos-posi+1);
         printf("Error in getting varnames\r\n");
         return;
       }

       //Pretending like the above thing never happened now.

       //move to end of varname
       while (synt[pos]=='n') pos++;

       //Bother to write the varname down as a local...
       {
         int a=0;
         std::string check=code.substr(posi,pos-posi);
         for (int i=0;i<ncount;i++) if (names[i]==check) { a=1; break; }
         if (!a)
         {
         names[ncount]=check;
         //array indexes recorded below
         nametags[ncount]=typetoadd;
         //increment ncount then
         }
       }


       //See if we have an array.
       int posi2=pos,pos2=pos;
       while (code[pos2]==')' || code[pos2]=='[')
       { pos2++; if (code[pos2-1]=='[') pos2=parser_end_of_brackets(pos2); }
       if (pos2>posi2) namearrays[ncount]=code.substr(posi2,pos2-posi2);
       else            namearrays[ncount]="";

       //now increment ncount
       ncount++;

       //Move to end of that
       pos=pos2;


       //if the next token is not an = sign
       if (code.substr(pos,3)!="<=>")
       {
         //Start of by deleting that varname then, otherwise it'll just be in the way.
         code.erase(posi,pos-posi);
         synt.erase(posi,pos-posi);
         pos=posi;

         //This means the item isn't used in declaration...
         //Check if the next thing is a comma.
         if (code[pos] != ',')
         {
            //It was not. So it better be a semicolon.
            if (code[pos] != ';')
            { puts("LOCALV declaration terminates incorrectly, error not caught by syntax checker"); return; }
            else break;
         }
         else
         {
            code.erase(pos,1);
            synt.erase(pos,1);
         }
       }
       else
       {
         while (pos<code.length() && !(code[pos]==',' || code[pos]==';' || code[pos]==' ')) pos++;
         if (code[pos]==',')
         {
           code[pos]=';';
           synt[pos]=';';
           pos++;
           code.insert(pos," ");
           synt.insert(pos," ");
           pos++; posi=pos;
         }
         continue;
       }
     }
     if (code[pos]==';')
     {
       code.erase(pos,1);
       synt.erase(pos,1);
     }
     code.insert(pos," } ");
     synt.insert(pos," } ");

     code.insert(start_position," { ");
     synt.insert(start_position," { ");
   }




   pos=0;
   while (pos<synt.length())
   {
      pos=synt.find("n",pos);

      if (pos==string::npos)
        break;


      spos=pos;
      while (synt[spos]=='n') spos--;
      //Skip any tokens
      if ((synt[spos]>'a' && synt[spos]<'z' && synt[spos] != 'n')
      ||  (synt[spos]>'A' && synt[spos]<'Z')
      ||  (synt[spos] == '_')
      ||  (synt[spos] == '<'))
      {
          pos++;
          while (synt[pos]=='n') pos++;
          continue;
      }
      spos++;



      epos=pos;
      while (synt[epos]=='n') epos++;
      //Skip any tokens
      if ((synt[epos]>'a' && synt[epos]<'z' && synt[spos] != 'n')
      ||  (synt[epos]>'A' && synt[epos]<'Z')
      ||  (synt[epos] == '_')
      ||  (synt[epos] == '>'))
      {
          pos++;
          while (synt[pos]=='n') pos++;
          continue;
      }

      pos=epos+1;
      checker=code.substr(spos,epos-spos);
      spos--;

      /* See if it's referenced outside this instance.
      *************************************************/
      dotted=(synt[spos]=='.');

      /* Check that it's on neither list
      ************************************/
      used=0;

      //Check that it isn't on the list already, but if it is,
      //make sure it's global if we ever found it dotted.
      for (int i=0; i<ncount; i++)
      if (names[i]==checker) { used=1; if (dotted) dots[i]=1; }
      if (used) continue;

      //exit if its on the locals list, but only if it isn't dotted.
      //(Because then it'd be global anyway)
      if (!dotted)
      for (int i=0; i<lcount; i++)
      if (locals[i]==checker) used=1;
      if (used) continue;



      /* This is obviously the first time this variable appears in the code.
      |* Just make sure this isn't its declaration.
      *****************************************************************************/

      /* Check that it isn't in a list of declarations. */
      isntlocal=0;

      //We don't need to know how many stars there are. Think:
      //They aren't declaring it local here, or it'd be in the
      //localv section above.
      while (code[spos]=='*' || code[spos]=='(') spos--;

      if (synt[spos]==',')
      {
         for (cpos=spos;cpos>0;cpos--)
         {
             while (synt[cpos]==')' || synt[cpos]==']' || synt[cpos]=='}')
             {
                while (synt[cpos]==')') { cpos=parser_start_of_parenths(cpos+1)-2; }
                while (synt[cpos]==']') { cpos=parser_start_of_brackets(cpos+1)-2; }
                while (synt[cpos]=='}') { cpos=parser_start_of_braces(cpos+1)-2; }
             }

             if (synt[cpos]=='(' || synt[cpos]=='[' || synt[cpos]=='{' || synt[cpos]==';')
             { isntlocal=1; break; }
             else if (synt[cpos]=='>')
             {
                std::string seg[5];
                seg[0]=seg[1]=seg[2]=seg[3]=seg[4]="";

                if (cpos>=4)
                {
                  seg[0]=code.substr(cpos-4,5);
                  if (cpos>=5)
                  {
                    seg[1]=code.substr(cpos-5,6);
                    if (cpos>=6)
                    {
                      seg[2]=code.substr(cpos-6,7);
                      if (cpos>=7)
                      {
                        seg[3]=code.substr(cpos-7,8);
                        if (cpos>=8)
                        {
                          seg[4]=code.substr(cpos-8,9);
                        }
                      }
                    }
                  }
               }

               if(seg[0]=="<int>"
               || seg[0]=="<var>"
               || seg[1]=="<char>"
               || seg[1]=="<bool>"
               || seg[1]=="<long>"
               || seg[2]=="<float>"
               || seg[2]=="<short>"
               || seg[3]=="<double>"
               || seg[4]=="<cstring>")
               {
                 isntlocal=0;
                 break;
               }
             }
         }
         spos=cpos;
      }

      //If this was set to true, we can be sure it is not in a list, because it
      //was interupted by some other symbol.
      if (isntlocal)
      {
        names[ncount]=checker;
        namestars[ncount]="";
        nametags[ncount]="";
        namearrays[ncount]="";
        dots[ncount]=dotted;
        ncount++;
        continue;
      }

      //There is a good chance that segment of code was not executed, since it is
      //not perfectly likely that the variable in question was declared in a list
      //of other declarations.

      //Either way, at this point the only way this is a local variable is if the
      //symbol we're at now marks the end of a type name token. Let's check that.

      if (synt[spos]=='>')
      {
         //addition 4-11

         //Seek to the beginning of the typenames
         int bpos1,bpos2;
         bpos1=bpos2=spos;
         while (1)
         {
           while (bpos1>=0 && synt[bpos1]!='<') bpos1--;
           if (bpos1<0) return;
           if (synt[bpos1-1]!='>')
           { spos=bpos2; break; }
           else
           { bpos1--; bpos2=bpos1; }
         }

         //Repaired 4-10-08
         std::string seg[5];
         if (spos>=4)
         {
           seg[0]=code.substr(spos-4,4);
           if (spos>=5)
           {
             seg[1]=code.substr(spos-5,5);
             if (spos>=6)
             {
               seg[2]=code.substr(spos-6,6);
               if (spos>=7)
               {
                 seg[3]=code.substr(spos-7,7);
                 if (spos>=8)
                 {
                   seg[4]=code.substr(spos-8,8);
                 }
               }
             }
           }
         }

         if(seg[0]=="<int"         //It would be a good idea to make sure
         || seg[0]=="<var"         //the token we are at does indeed mark
         || seg[1]=="<char"        //a type name.
         || seg[1]=="<bool"
         || seg[1]=="<long"
         || seg[2]=="<float"
         || seg[2]=="<short"
         || seg[3]=="<double"
         || seg[4]=="<cstring")
         {
            locals[lcount]=checker;
            if (dotted) {}               //syntax error here
            lcount+=1;
         }
         else
         {
               names[ncount]=checker;       //This would be a great place to put
               namestars[ncount]="";        //a syntax error alert. parser_potential_error style.
               nametags[ncount]="";
               namearrays[ncount]="";
               dots[ncount]=dotted;
               ncount+=1;
         }
      }
      else
      {
         names[ncount]=checker;          //This is natural, and likely. It wasn't
         namestars[ncount]="";           //declared in this script; add it to the list.
         nametags[ncount]="";
         namearrays[ncount]="";
         dots[ncount]=dotted;
         ncount+=1;
      }
   }

    delete[] vns;
    delete[] vnss;
    delete[] vnts;
    delete[] vnas;

    vns=new string[ncount+1];
    vnss=new string[ncount+1];
    vnts=new string[ncount+1];
    vnas=new string[ncount+1];
    vnc=0;

    for (int i=0;i<ncount;i++)
    {
       vns[vnc]=names[i];
       vnss[vnc]=namestars[i];
       vnts[vnc]=nametags[i];
       vnas[vnc]=namearrays[i];
       vnc++;

       if (dots[i]!=0)
       {
          int on=0;
          for (int ii=0;ii<__globalcount;ii++)
          if (__globals[ii]==names[i]) { on=1; break; }

          if (!on) { __globals[__globalcount]=names[i];
                     __globaltags[__globalcount]="";
                     __globalstars[__globalcount]="";
                     __globalarrays[__globalcount]="";
                     __globalcount++; }
       }
    }
}

/*

void parser_get_withvars()
{
   printf("SUICIDE!!\n");
   unsigned int pos=0,withpos=0,len=__codebuffer.length(),spos,epos;
   while ((withpos=__codebuffer.find("<with>",withpos)) != string::npos)
   {
      spos=withpos;
      while (__codebuffer[spos] != '(') spos++;
      epos=spos=parser_end_of_parenths(spos);

      if (__codebuffer[epos]=='{')
      epos=parser_end_of_braces(epos-1)-1;
      else if (__codebuffer[epos+1]=='{')
      epos=parser_end_of_braces(epos)-1;
      else while (__codebuffer[epos] != ';') epos++;

      std::string
      withsyntax=__syntaxbuffer.substr(spos,epos-spos+1),
      withcode = __codebuffer . substr(spos,epos-spos+1);

      withpos=epos;

      parser_get_varnamelist(withcode,withsyntax,1);
   }
   printf("JUST KIDDING!\n");
}

*/


int varname_onlists(std::string vname)
{
    /*for (int i=0; i<__globalcount;i++)  //May not be a good idea.
    if (vname==__globals[i]) return 1;
    */
    for (int i=0; i<__constcount;i++)
    if (vname==__consts[i]) return 1;

    for (int i=0; i<localc;i++)
    if (vname==localn[i]) return 1;

    for (int i=0; i<__enigmaglobalcount;i++)
    if (vname==__enigmaglobals[i]) return 1;

    return 0;
}

int varname_oninheriteds(std::string vname)
{
    for (int i=0; i<localc;i++)
    if (vname==localn[i]) return 1;

    for (int i=0; i<__constcount;i++)
    if (vname==__consts[i]) return 1;

    for (int i=0; i<__enigmaglobalcount;i++)
    if (vname==__enigmaglobals[i]) return 1;

    return 0;
}




int varname_onnotinheriteds(std::string vname)
{
    for (int i=0; i<__constcount;i++)
    if (vname==__consts[i]) return 1;

    for (int i=0; i<__enigmaglobalcount;i++)
    if (vname==__enigmaglobals[i]) return 1;

    return 0;
}




void parser_get_withcodes()
{
   unsigned int pos=0,wnum=0;
   while ((pos=__codebuffer.find("<with>",pos)) != string::npos)
   {
      string tnc,tns;
      tnc=tostring(wnum);
      tns=string_repeat("0",tnc.length());

      int spos=pos+6; while (__codebuffer[spos]!='(') spos++;
      __codebuffer . replace(pos,spos-pos+1,"{<for>(<withiter>ENIGMA_WITHiter=enigma::pushwith("+tnc+",");
      __syntaxbuffer.replace(pos,spos-pos+1,"{<for>(<withiter>000000000000000=ffffffffffffffff("+tns+",");
                                           /*1234567890123456789012345678901234567890123456789*/

      wnum++;
      pos+=49;
      pos=parser_end_of_parenths(pos);
      if(pos==string::npos) { printf("C++ is being a total piece of poo. Dropping out. \n%s\n",__codebuffer.c_str()); exit(0xFEE15BAD); }

      __codebuffer . insert(pos,";ENIGMA_WITHiter.stillgoing;ENIGMA_WITHiter++) <if>(*enigma::withcur["+tnc+"].id==enigma::withid["+tnc+"]<||>*enigma::withcur["+tnc+"].object_index==enigma::withid["+tnc+"])");
      __syntaxbuffer.insert(pos,";00000000000000000000000000;000000000000000++) <if>(*fffffffffffffff["+tns+"].nn==nnnnnnnnnnnnnn["+tns+"]<||>*nnnnnnnnnnnnnnn["+tns+"].nnnnnnnnnnnn==nnnnnnnnnnnnnn["+tns+"])");
                               //123456789012345678901234567890123456789012345678901
      pos+=51; pos=parser_end_of_parenths(pos);
      if(pos==string::npos) { printf("C++ is STILL being a total piece of poo. Dropping out. \n%s\n",__codebuffer.c_str()); exit(0xFEE15BAD); }




      if (__codebuffer[pos]==' ') pos++;

      int wcstart=pos;

      if (__codebuffer[pos]=='{')
      pos=parser_end_of_braces(pos-1)-1;
      else if (__codebuffer[pos+1]=='{')
      pos=parser_end_of_braces(pos)-1;
      else while (__codebuffer[pos] != ';')
      {
        pos++; if (__codebuffer[pos]=='{') pos=parser_end_of_braces(pos+1);
      }

      string wc=__codebuffer . substr(wcstart,pos-wcstart+1);
      string ws=__syntaxbuffer.substr(wcstart,pos-wcstart+1);

      __codebuffer . insert(pos+1," enigma::popOtherObject();}");
      __syntaxbuffer.insert(pos+1," ffffffffffffffffffffff();}");

      string *wvn=new string[2],*wvs=new string[2],*wvt=new string[2],*wva=new string[2];
      int wvc;

      parser_get_varnamelist(wc,ws,wvn,wvs,wvt,wva,wvc);

      for (unsigned int i=0;i<wc.length();i++)
      {
        if (ws[i]=='n')
        {
          int iii=i;
          while (ws[iii]=='n') iii++; if (ws[iii]=='>' || (ws[iii]>='a'&&ws[iii]<='z')||(ws[iii]>='A'&&ws[iii]<='Z') || ws[iii]=='_') { i=iii; continue; }
          int ii=i;
          while (ws[ii]=='n') ii--; if (ws[ii]=='<' || (ws[ii]>='a'&&ws[ii]<='z')||(ws[ii]>='A'&&ws[ii]<='Z') || ws[ii]=='_') { i=iii; continue; }

          string viq=wc.substr(ii+1,iii-ii-1);

          int isl=0;
          for (int iv=0;iv<wvc;iv++)
          if (wvn[iv]==viq) { isl=1; break; }
          if (!isl || varname_onnotinheriteds(viq)) continue;
                     /*12345678901234567*/
          wc.insert(i,"*enigma::withcur["+tnc+"].");
          ws.insert(i,"*nnnnnnnnnnnnnnn["+tns+"]\xEE");
          i+=17;

          while (ws[i]!=']') i++;
          i+=2; char c=ws[i];
          while (ws[i]==c) i++;
        }
      }

      __codebuffer . replace(wcstart,pos-wcstart+1,wc);
      __syntaxbuffer.replace(wcstart,pos-wcstart+1,ws);
   }
}


