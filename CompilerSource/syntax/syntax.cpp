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

#include <map>
#include <string>
#include <cstdlib>

using namespace std;

#include "../general/textfile.h"
#include "../general/darray.h"

#define is_letter(x)  ((x>='a' && x<='z') || (x>='A' && x<='Z') || (x=='_'))
  #define is_digit(x)   (x>='0' && x<='9')
  #define is_letterd(x) (is_letter(x) || is_digit(x))
  #define is_unary(x)   (x=='!' || x=='~' || x=='+' || x=='-' || x=='&' || x=='*')
  #define is_linker(x)  (x=='+' || x=='-' || x=='*' || x=='/' || x=='=' || x=='!' || x=='~' || x=='&' || x=='|' || x=='^' || x=='.')
#define is_useless(x) (x==' ' || x=='\r' || x=='\n' || x=='\t')

/*
extern map<int,string> fnames;
extern int fcount;*/
#include "../cfile_parse/externs.h"

string tostring(int val);


namespace syncheck
{
  int lastln;
  string error="";
  int syntacheck(string code);
  
  void addscr(string name)
  {
    externs *ne=global_scope.members[name];
    ne->flags|=EXTFLAG_FUNCTION;
    for (int i=0;i<16;i++)
    ne->fargs[ne->fargs.size]=0;
  }
  
  /*
  
  struct function_info
  {
    string name;
    char rtype;
    char argcount; //-100 for infinite
    char atype[64]; //up to 64 args, 0 for real 1 for string 2 for variant +3 for optional 5 for WTF MAN
  };
  
  int fcount=0;
    map<int,function_info> fnames;
  int scrcount=0;
    map<int,function_info> scrnames;*/
}
/*
int get_two(int pos)
{
  printf("Accessed LN_OPERATOR at %d\n",pos);
  return 2;
}*/

namespace syncheck
{
  // Start off with some basic things 
  // we'll need during the check process
  ////////////////////////////////////////
  /*
    #include "syntax/parentheses.h"*/
    //#include "loadfnames.h"
    /*string typenames="|bool||char||int||float||double||cstring||short||long|";
    string typeflags="|lovalv||unsigned||signed||const||static||short||long|";
    #include "syntax/castchecker.h"*/
    
    #include "chkfunction.h"
    
    //Map for flexibility, dynamics, and legibility
    typedef map<int,int> array;
    
    //How high we are, stack wise
     int level=0;
    //These tell us what kind of level this is
     array levelt;
     #define LEVELTYPE_BRACE 0
     #define LEVELTYPE_IF 1
     #define LEVELTYPE_DO 2
     #define LEVELTYPE_CASE 3
     #define LEVELTYPE_SWITCH 4
     #define LEVELTYPE_SWITCH_BLOCK 5
     #define LEVELTYPE_FOR_PARAMETERS 6
     #define LEVELTYPE_GENERAL_STATEMENT 7
     
    //If we are in an if statement, we expect two expressions before else statement.
      //For example, if a=b b=c else d=e; 
      //a=b is expression one, then b=c is expression two.
      //This variable represents the number of statements.
      array statement_pad;
    
    //How high we are, parenthesis wise
     int plevel=0;
    //These tell us what kind of parentheses we're in
     array plevelt;
      #define PLT_PARENTH 0
      #define PLT_BRACKET 1
      #define PLT_FUNCTION 2
      #define PLT_FORSTATEMENT 3
      
      
    array assop; //Whether we have an assignment operator, organized per-level
    array lastnamed; //The last thing that was named
    //Values for type of last named token:
     #define LN_NOTHING 0
     #define LN_VARNAME 1
     #define LN_OPERATOR 2
     #define LN_DIGIT 3
     #define LN_FUNCTION 4
     #define LN_CLOSING_SYMBOL 5
    
    //For declarations based on a typename
     array indeclist;
     
    //either in a declaration list and plevel is zero, or it's one and plevelt is PLT_FORSTATEMENT
    //or we're in a function
      #define inlist() ((indeclist[level] && (plevel==0 || plevelt[plevel]==PLT_FORSTATEMENT)) or (plevelt[plevel]==PLT_FUNCTION))
    
    //One more precaution. This is for a=.b
    //it enables the checker to dismiss a . it comes by until next iteration,
    //where it is treated as a regular number. This makes it so when that happens,
    //you can't have .5.23
    int decimal_named=0;
    int number_of_statements=0;
 
   //Anything else the checker will use a lot
    #include "syntaxtools.h"
   
  // Now that the basics are over, we get into 
  // the specific variables we'll be working with
  //////////////////////////////////////////////////
  
    int errorpos=-1;
  
  // Onto the actual syntax checker code
  ////////////////////////////////////////
      
  int syntacheck(string code) //string for substr() member
  {
    error="";
    errorpos=-1;
    
    int pos=0;
    const int len=code.length();
    
    level=0; plevel=0;
    assop[level]=0;
    lastnamed[level]=LN_NOTHING;
    
    int last_identifier_start = 0;
    
    statement_pad[level]=0;
    
    while (pos<len)
    {
      if (is_letter(code[pos])) //we're at an identifier, a statement, or a word-based operator
      {
        if (!is_wordop(code,pos))
        {
          //Close any previously completed statements, since this isn't an operator
          //Also handle else and until statements
          if (statement_completed(lastnamed[level]))
          {
            int cs=close_statement(code,pos);
            if (cs != -1) return cs;
          }
          else
          {
            if (lastnamed[level]==LN_VARNAME or lastnamed[level]==LN_FUNCTION)
              { error="Operator expected at this point"; return pos; }
          }
          
          if (is_statement(code,pos,len)) //Check if it's a statement, handle it if it is.
          {
            int hs=handle_statement(code,pos); //This does NOT handle until and else. See above.
            if (hs!=-1) return hs;
          }
          else //just an identifier; ie, a variable or function name
          {
            last_identifier_start = pos;
            while (is_letterd(code[pos])) pos++;
            lastnamed[level]=LN_VARNAME;
          }
        }
        else //this is actually a word-based operator, such as `and' and `or'
        {
          if (code[pos]=='n') //not
          {
            if (!assop[level] or lastnamed[level]!=LN_OPERATOR) //has to be preceeded by operator
            {
              error="Expected operator before unary `not' operator";
              return pos; 
            }
          }
          else /*if((code[pos]=='a') //and 
          or (code[pos]=='o') //or
          or (code[pos]=='x') //xor
          or (code[pos]=='d') //div
          or (code[pos]=='m')) //mod*/
          {
            if (!assop[level] && !(plevel>0)) //randomly placed 'varname and varname'
            { error="Assignment operator expected before this point"; return pos; }
            if (lastnamed[level]!=LN_VARNAME)
            { error="Expected primary expression before operator"; return pos; }
          }
          while (is_letter(code[pos])) pos++;
          lastnamed[level] = LN_OPERATOR;
        }
      }
      else if (is_digit(code[pos])) //We're at a number
      {
        //Should be a previous operator, should have been an assignment operator
        int introuble=0;
        if (!(plevel>0))
        {
          if (!assop[level])
          introuble=1;//{ error="Assignment operator expected before numeric constant"; return pos; }
          if (lastnamed[level] != LN_OPERATOR && !(inlist() and lastnamed[level]==LN_NOTHING))
          introuble=2;//{ error="Operator expected before numeric constant"; return pos; }
        }
        
        while (is_digit(code[pos])) pos++;
        while (is_useless(code[pos])) pos++;
        if (!decimal_named)
        if (code[pos]=='.')
        {
          int cpos=pos+1;
          while (is_useless(code[cpos])) cpos++;
          if (is_digit(code[cpos]))
          {
            pos=cpos;
            while (is_digit(code[pos])) pos++;
          }
          else if (introuble>0)
          {
            if (is_letter(code[cpos]))
            {
              if (statement_completed(lastnamed[level]))
              close_statement(code,pos);
            }
            else
            {
              if (introuble==1)
              { error="Assignment operator expected before numeric constant"; return pos; }
              if (introuble==2)
              { error="Operator expected before numeric constant"; return pos; }
            }
          }
        }
        if (introuble>0)
        {
          int cpos=pos;
          while (is_useless(code[cpos])) cpos++;
          if (code[cpos]=='.')
          {
            if (statement_completed(lastnamed[level]))
            close_statement(code,cpos);
          }
          else
          {
            if (introuble==1)
            { error="Assignment operator expected before numeric constant.."; return pos; }
            if (introuble==2)
            { error="Operator expected before numeric constant.."; return pos; }
          }
        }
        decimal_named=0; //reset this, it serves only one purpose
        lastnamed[level]=LN_DIGIT;
      }
      else //It's a symbol: Not a letter or digit
      {
        //the two most common symbols in the language are up first
        if (is_useless(code[pos]))
        {
          pos++;
          while (is_useless(code[pos]))
          pos++;
          continue;
        }
        
        if (code[pos]==';')
        {
          if (plevel>0 and plevelt[plevel] != PLT_FORSTATEMENT)
            { error="Semicolon does not belong inside set of parentheses"; return pos; }
          if (lastnamed[level]==LN_OPERATOR)
            { error="Secondary expression expected before semicolon"; return pos; }
          indeclist[level]=0;
          lastnamed[level]=LN_CLOSING_SYMBOL;
          pos++;
          continue;
        }
        
        
        if (code[pos]==':')
        {
          if (plevel>0)
            { error="Unexpected colon at this point"; return pos; }
          if (level>0 and levelt[level]==LEVELTYPE_CASE)
          {
            if (statement_pad[level]!=1 || lastnamed[level]==LN_OPERATOR)
            { error="Expression expected before `:' symbol"; return pos; }
            lastnamed[level]=LN_CLOSING_SYMBOL;
          }
          else if (lastnamed[level]==LN_VARNAME and !assop[level])
          {
            { error="Goto labels not yet supported"; return pos; }
            lastnamed[level]=LN_NOTHING; //Pretend you never saw the identifier
          }
          else
          {
            error="Unexpected symbol `:' at this point"; 
            return pos;
          }
          pos++;
          continue;
        }
        
        
        if (code[pos]==',')
        {
          if (!inlist())
           { error="Comma outside of function parameters or declarations"; return pos; }
          if (lastnamed[level]==LN_OPERATOR)
            { error="Secondary expression expected before comma"; return pos; }
          
          //The beauty: if we have unterminated parentheses before the comma, the first error is true.
          //There does not need to be an assignment operator in either case.
          
          lastnamed[level]=LN_NOTHING;
          pos++;
          continue;
        }
        
        
        //Moving on...
        //first priority is to check for a bracket of any sort; () [] {}
        if (code[pos]=='(')
        {
          plevel++;
          if (lastnamed[level]==LN_VARNAME)
          {
            plevelt[plevel]=PLT_FUNCTION;
            int cf=checkfunction(code,pos,len,last_identifier_start);
            if (cf!=-1) return cf;
            lastnamed[level]=LN_NOTHING;
          }
          else
          {
            if (lastnamed[level]!=LN_OPERATOR)
            {
              int cpos=pos+1,qc=1;
              while (qc>0)
              {
                if (code[cpos]=='(') qc++; if (code[cpos]==')') qc--; 
                if (cpos>=len) break; //Ensure we don't overflow
                cpos++;
              }
              
              if (qc>0)
              { error="Unterminated parenthetical expression at this point"; return pos; }
              
              while (is_useless(code[cpos])) cpos++;
              if (code[cpos]!='.')
              { error="Unexpected parenthetical expression at this point"; return pos; }
              
              if (statement_completed(lastnamed[level]))
              close_statement(code,pos);
            }
            else if (!assop[level] && !(plevel>0))
            { error="Parenthetical expression outside of assignment operator"; return pos; }
            plevelt[plevel]=PLT_PARENTH;
            //lastnamed[level] should remain the same... it should be an operator.
          }
          pos++;
          continue;
        }
        else if (code[pos]==')')
        {
          if (plevelt[plevel]!=PLT_PARENTH and plevelt[plevel]!=PLT_FUNCTION)
          { error="Unexpected closing parenthesis at this point"; return pos; }
          if (plevelt[plevel]==PLT_FUNCTION)
            lastnamed[level]=LN_FUNCTION;
          else
            lastnamed[level]=LN_DIGIT;
          plevel--;
          pos++;
          continue;
        }
        
        
        if (code[pos]=='[')
        {
          if (lastnamed[level] != LN_VARNAME)
          { error="Invalid use of `[' symbol"; return pos; }
          plevel++;
          plevelt[plevel]=PLT_BRACKET;
          lastnamed[level]=LN_OPERATOR;
          pos++;
          continue;
        }
        else if (code[pos]==']')
        {
          if (plevelt[plevel] != PLT_BRACKET)
          { error="Unexpected closing bracket at this point"; return pos; }
          lastnamed[level]=LN_VARNAME;
          plevel--;
          pos++;
          continue;
        }
        
        
        if (code[pos]=='{')
        {
          /*int einfo;
          if (levelt[level]==LEVELTYPE_SWITCH) einfo=LEXTRAINFO_SWITCHBLOCK;
          if (levelt[level]==LEVELTYPE_DO) einfo=LEXTRAINFO_LOOPBLOCK;
          if (levelt[level]==LEVELTYPE_FOR_PARAMETERS) einfo=LEXTRAINFO_LOOPBLOCK;
          if (levelt[level]==LEVELTYPE_GENERAL_STATEMENT) 
          {
            if (levelei[level]==LEXTRAINFO_WHILE) einfo=LEXTRAINFO_LOOPBLOCK;
            if (levelei[level]==LEXTRAINFO_WHILE) einfo=LEXTRAINFO_LOOPBLOCK;
          }*/
          bool isbr=levelt[level]==LEVELTYPE_SWITCH;
          
          if (lastnamed[level]==LN_OPERATOR)
          { error="Unexpected brace at this point"; return pos; }
          
          level+=!isbr; 
          levelt[level]=(isbr?LEVELTYPE_SWITCH_BLOCK:LEVELTYPE_BRACE);
          lastnamed[level]=LN_NOTHING; //This is the beginning of a glorious new level
          statement_pad[level]=-1;
          assop[level]=0;
          
          pos++;
          continue;
        }
        else if (code[pos]=='}')
        {
          if (level<=0)
          { error="Unexpected closing brace at this point"; return pos; }
          
          if (lastnamed[level]==LN_OPERATOR)
          { error="Expected secondary expression before closing brace"; return pos; }
          
          if (statement_completed(lastnamed[level]))
          {
            int cs=close_statement(code,pos);
            if (cs != -1) return cs;
          }
          
          lower_to_level(LEVELTYPE_BRACE,"`}' symbol");
          level--;
          statement_pad[level]=-1;
          lastnamed[level]=LN_CLOSING_SYMBOL; //Group is like one statement for this level
          //this will invoke the next statement to close the current statement
          
          pos++;
          continue;
        }
        
        
        
        
        if (code[pos]=='"')
        {
          if (!assop[level] && !(plevel>0))
          { error="Assignment operator expected before string constant"; return pos; }
          if (lastnamed[level] != LN_OPERATOR && !(inlist() and lastnamed[level]==LN_NOTHING))
          { error="Operator expected before string constant"; return pos; }
          pos++;
          while (code[pos]!='"') pos++;
          pos++;
          lastnamed[level]=LN_DIGIT;
          continue;
        }
        else if (code[pos]=='\'')
        {
          if (!assop[level] && !(plevel>0))
          { error="Assignment operator expected before string constant"; return pos; }
          if (lastnamed[level] != LN_OPERATOR && !(inlist() and lastnamed[level]==LN_NOTHING))
          { error="Operator expected before string constant"; return pos; }
          pos++;
          while (code[pos]!='\'') pos++;
          pos++;
          lastnamed[level]=LN_DIGIT;
          continue;
        }
        
        
        
        if (code[pos]=='.')
        {
          if (lastnamed[level] == LN_DIGIT || lastnamed[level]==LN_VARNAME || lastnamed[level]==LN_FUNCTION)
          {
            pos++;
            while (is_useless(code[pos])) pos++;
            if (!is_letter(code[pos])) { error="Identifier expected following `.' symbol"; return pos; }
            lastnamed[level]=LN_OPERATOR;
            continue;
          }
          else
          {
            //in this case, there could be a number following
            //Syntax checking on the positioning of the number will be handled next iteration
            pos++;
            while (is_useless(code[pos+1])) pos++;
            if (is_digit(code[pos]))
            { decimal_named=1; continue; }
            else
            { error="Unexpected symbol `.' in expression"; return pos; }
          }
        }
        
        
        //Now we can assume it's supposed to be an operator
        if (code[pos]=='=') //handle = and == first
        {
          if (code[pos+1]=='=') //it's operator==
          {
            if (!assop[level] && !(plevel>0))
            { error="Assignment operator expected before comparison operator"; return pos; }
            if (lastnamed[level]==LN_CLOSING_SYMBOL || lastnamed[level]==LN_OPERATOR || lastnamed[level]==LN_NOTHING)
            { error="Expected primary expression before comparison operator"; return pos; }
            pos+=2; lastnamed[level]=LN_OPERATOR;
          }
          else //operator=
          {
            if (lastnamed[level]==LN_FUNCTION && !assop[level])
            { error="Cannot assign to a function, use == to test equality"; return pos; }
            if (lastnamed[level]!=LN_VARNAME && !assop[level])
            { error="Variable name expected before assignment operator"; return pos; }
            assop[level]=1;
            lastnamed[level]=LN_OPERATOR;
            pos+=1;
          }
          continue;
        }
        
        //Now we check if it's an operator... last chance
        ///////////////////////////////////////////////////
        
        if ((code[pos+1]=='=') or (code[pos+1]==code[pos] && code[pos+2]=='=')) //is an assignment operator (or potentially a comparison)
        {
          if (code[pos+1] == '=') //one character base operator, followed by =
          {
            switch (code[pos])
            {
              case '+': 
              case '-': 
              case '*': 
              case '/': 
              case '&': 
              case '|': 
              case '^': //Same code as two below follwing case '<':
                if (lastnamed[level]==LN_FUNCTION && !assop[level])
                { error="Cannot assign to a function, use == to test equality"; return pos; }
                if (lastnamed[level]!=LN_VARNAME && !assop[level])
                { error="Variable name expected before assignment operator"; return pos; }
                assop[level]=1;
                lastnamed[level]=LN_OPERATOR;
                break; 
              
              case '>':
              case '<':
              case '!':
                if (!assop[level] && !(plevel>0))
                { error="Assignment operator expected before comparison operator"; return pos; }
                if (lastnamed[level]==LN_CLOSING_SYMBOL || lastnamed[level]==LN_OPERATOR || lastnamed[level]==LN_NOTHING)
                { error="Expected primary expression before comparison operator"; return pos; }
                break;
              
              default:
                error="Invalid operator at this point"; 
                return pos;
            }
            pos+=2; 
            continue;
          }
          else //two character base operator, followed by =
          {
            switch (code[pos])
            {
              case '>':
              case '<': //same code as above following case '^':
                if (lastnamed[level]!=LN_VARNAME)
                { error="Variable name expected before assignment operator"; return pos; }
                if (lastnamed[level]==LN_FUNCTION)
                { error="Cannot assign to a function, use == to test equality"; return pos; }
                assop[level]=1;
                lastnamed[level]=LN_OPERATOR;
                break;
              
              default:
                error="Invalid operator at this point"; 
                return pos;
            }
            pos+=3; 
            continue;
          }
        }
        else //is not an assignment operator
        {
          switch (code[pos])
          {
            case '+':
            case '-':
            case '*':
            case '&':
              if (!assop[level] && !(plevel>0))
              { error="Assignment operator expected before any secondary operators"; return pos; }
              //Since +, - and * can also be unary, we do not have to check for previous identifier
              lastnamed[level]=LN_OPERATOR;
              while (code[pos]=='+' or code[pos]=='-' or code[pos]=='*' or code[pos]=='&' or is_useless(code[pos])) 
              pos++;
              break;
            
            case '/':
            case '%':
            case '|':
            case '^':
            case '>':
            case '<':
              if (!assop[level] && !(plevel>0))
              { error="Assignment operator expected before any secondary operators"; return pos; }
              if (lastnamed[level]!=LN_VARNAME && lastnamed[level]!=LN_DIGIT)
              { error="Primary expression expected before operator"; return pos; }
              lastnamed[level]=LN_OPERATOR;
              if (code[pos+1]==code[pos])
              {
                if (code[pos]!='^' && code[pos]!='|' && code[pos]!='^' && code[pos]!='<' && code[pos]!='>')
                { error="Invalid operator at this point"; return pos; }
                pos++;
              }
              if (code[pos]=='<' and code[pos+1]=='>')
              pos++;
              
              pos++;
              break;
            
            case '!': //we already know the next symbol is not '=', so this is unary
            case '~':
              if (lastnamed[level]!=LN_OPERATOR)
              { error="Unexpected unary operator following value"; return pos; }
              pos++;
              break; //lastnamed is already operator, so just break
            
            default:
              error="Unexpected symbol reached";
              return pos;
          }
          continue;
        }
      }
    }
              
    //We're now at the end of the code. Perform a couple checks...
    if (plevel>1)
     { error="Code ends with "+tostring(plevel)+" unterminated parentheses"; return pos; }
    if (plevel==1)
     { error="Code ends with one unterminated parenthesis"; return pos; }
     
    if (lastnamed[level]==LN_VARNAME and !assop[level])
     { error="Assignment operator expected before end of code"; return pos; }
     
    
    if (statement_completed(lastnamed[level]))
      close_statement(code,pos);
    
    if (lastnamed[level]==LN_OPERATOR)
     { error="Expected secondary expression before end of code"; return pos; }
    
    if (statement_pad[level]>(levelt[level]==LEVELTYPE_IF))
    { error="Unexpected end of code reached ("+tostring(level)+","+tostring(statement_pad[level]); return pos; }
    
    
    while (level>0)
    {
      if (levelt[level]==LEVELTYPE_BRACE or levelt[level]==LEVELTYPE_SWITCH_BLOCK)
        { error="Symbol `}' expected before end of code"; return pos; }
      if (levelt[level]==LEVELTYPE_DO)
        { error="Statement `until' expected before end of code"; return pos; }
      else level--;
    }
    
    return -1;
  }
}
