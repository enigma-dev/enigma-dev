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


inline int is_wordop(string& code,int& pos)
{
  if (is_useless(code[pos+3]))
  if ((code.substr(pos,3)=="and")
  or  (code.substr(pos,3)=="xor")
  or  (code.substr(pos,3)=="not")
  or  (code.substr(pos,3)=="div")
  or  (code.substr(pos,3)=="mod")
  ) return 1;
  
  if (is_useless(code[pos+2]))
  if ((code.substr(pos,3)=="or")
  ) return 1;
  
  return 0;
}

inline int is_statement(string& code,int& pos,const int& len)
{
  if (pos+2>=len || !is_letterd(code[pos+2]))
  if ((code.substr(pos,2)=="if")
  or  (code.substr(pos,2)=="do")
  ) return 1;
  
  if (pos+3>=len || !is_letterd(code[pos+3]))
  if ((code.substr(pos,3)=="for")
  or  (code.substr(pos,3)=="try")
  or  (code.substr(pos,3)=="end")
  ) return 1;
  
  if (pos+4>=len || !is_letterd(code[pos+4]))
  if ((code.substr(pos,4)=="with")
  or  (code.substr(pos,4)=="exit")
  or  (code.substr(pos,4)=="case")
  or  (code.substr(pos,4)=="else")
  ) return 1;
  
  if (pos+5>=len || !is_letterd(code[pos+5]))
  if ((code.substr(pos,5)=="while")
  or  (code.substr(pos,5)=="until")
  or  (code.substr(pos,5)=="catch")
  or  (code.substr(pos,5)=="break")
  or  (code.substr(pos,5)=="begin")
  ) return 1;
  
  if (pos+6>=len || !is_letterd(code[pos+6]))
  if ((code.substr(pos,6)=="return")
  or  (code.substr(pos,6)=="switch")
  ) return 1;
  
  if (pos+7>=len || !is_letterd(code[pos+7]))
  if ((code.substr(pos,7)=="default")
  or  (code.substr(pos,7)=="switch")
  ) return 1;
  
  return 0;
}

inline int statement_completed(int lt,int pos=0) //this code assumes we are at a variable name that is not an operator
{
  if (lt==LN_CLOSING_SYMBOL)
    return 1;
  
  if (lt==LN_OPERATOR)
    return 0;
  
  if (lt==LN_FUNCTION)
    return 1;
  
  if (assop[level])
  if (lt==LN_VARNAME 
  ||  lt==LN_DIGIT
  ) return 1;
  
  return 0;
}

inline int lower_to_level(int leveltype,string sn)
{
  while (level>0)
  {
    if (levelt[level]==leveltype)
      return -1;
    //The key here is, if the call requested, say, a do statement,
    //We would have just returned, so this next part won't error.
    if (levelt[level]==LEVELTYPE_DO)
    { error="Statement `until' expected before "+sn; return 1; }
    if (levelt[level]==LEVELTYPE_SWITCH_BLOCK or levelt[level]==LEVELTYPE_BRACE)
    { error="Symbol `}' expected before "+sn; return 1; }
    level--;
    if (statement_pad[level]>0) statement_pad[level]--;
  }
  return -1;
}
inline bool check_level_accessible(int leveltype)
{
  int chklvl=level;
  while (chklvl>0)
  {
    if (levelt[chklvl]==leveltype)
      return 1;
    
    if (levelt[chklvl]==LEVELTYPE_DO)
    { return 0; }
    if (levelt[chklvl]==LEVELTYPE_SWITCH_BLOCK)
    { return 0; }
    if (levelt[chklvl]==LEVELTYPE_BRACE)
    { return 0; }
    
    chklvl--;
  }
  return 0;
}
inline int get_closest_level_pad(int leveltype)
{
  int chklvl=level;
  while (chklvl>0)
  {
    if (levelt[chklvl]==leveltype)
      return statement_pad[chklvl];
    
    if (levelt[chklvl]==LEVELTYPE_DO)
    { return 0; }
    if (levelt[chklvl]==LEVELTYPE_SWITCH_BLOCK)
    { return 0; }
    if (levelt[chklvl]==LEVELTYPE_BRACE)
    { return 0; }
    
    chklvl--;
  }
  return 0;
}

inline int handle_statement(string& code,int& pos)
{
  if (lastnamed[level]==LN_OPERATOR)
  { error="Expected secondary expression before statement"; return pos; }
  
  if ((statement_pad[level]>0 and levelt[level]==LEVELTYPE_GENERAL_STATEMENT)
  or  (statement_pad[level]>1 and levelt[level]==LEVELTYPE_SWITCH)
  or  (statement_pad[level]>2 and levelt[level]==LEVELTYPE_IF)
  or  (statement_pad[level]>2 and levelt[level]==LEVELTYPE_DO))
  { error="Second statement given in parameters of first"; return pos; }
  
  if (code.substr(pos,2)=="if")
  {
    level++; 
    lastnamed[level]=LN_OPERATOR;
    assop[level] = 1;
    levelt[level]=LEVELTYPE_IF;
    statement_pad[level]=3;
    pos+=2;
  }
  if (code.substr(pos,2)=="do")
  {
    level++; 
    lastnamed[level]=LN_NOTHING;
    assop[level] = 0;
    levelt[level]=LEVELTYPE_DO;
    statement_pad[level]=2;
    pos+=2;
  }
  else if (code.substr(pos,4)=="else")
  {
    if (level<=0)
    { error="Unexpected `else' statement at this point"; return pos; }
    
    if (lower_to_level(LEVELTYPE_IF,"`else' statement")!=-1)
    return pos;
    
    if (statement_pad[level]!=1)
    { error="Statement `else' expected two expressions after `if' statement, given "+tostring(3-statement_pad[level]); return pos; }
    
    levelt[level]=LEVELTYPE_GENERAL_STATEMENT;
    statement_pad[level]=1;
    lastnamed[level]=LN_NOTHING; //end of if level, start of else level
    pos+=4;
  }
  else if (code.substr(pos,5)=="until" or (code.substr(pos,5)=="while" and check_level_accessible(LEVELTYPE_DO) and get_closest_level_pad(LEVELTYPE_DO)==1))
  {
    if (level<=0)
    { error="Unexpected `until' statement at this point"; return pos; }
    
    if (lower_to_level(LEVELTYPE_DO,"`until' statement")!=-1)
    return pos;
    
    if (statement_pad[level]!=1)
    { error="Statement `until' expected one expression after `do' statement, given "+tostring(2-statement_pad[level]); return pos; }
    
    //level--; level++;
    levelt[level]=LEVELTYPE_GENERAL_STATEMENT;
    statement_pad[level]=1;
    lastnamed[level]=LN_OPERATOR; //end of if level, start of else level
    assop[level]=1;
    pos+=5;
  }
  else if (code.substr(pos,6)=="switch")
  {
    level++; 
    lastnamed[level]=LN_OPERATOR;
    assop[level] = 1;
    levelt[level]=LEVELTYPE_SWITCH;
    statement_pad[level]=2;
    pos+=6;
  }
  else  if (code.substr(pos,7)=="default")
  {
    if (levelt[level]!=LEVELTYPE_SWITCH_BLOCK)
    { error="Statement `default' expected only in switch statement"; return pos; }
    pos+=7; while (is_useless(code[pos])) pos++;
    if (code[pos]==':') pos++;
    lastnamed[level]=LN_NOTHING;
  }
  else  if (code.substr(pos,5)=="break" or code.substr(pos,4)=="exit")
  {
    if (levelt[level]==LEVELTYPE_FOR_PARAMETERS)
    { error="Unexpected `break' statement in `for' parameters"; return pos; }
    
    /*bool fnd; int fs=level;
    while ((fs--)>0) if (levelt[fs]==LEVELTYPE_BRACE) { fnd=1; break; }
    if (levelt[level]==LEVELTYPE_FOR_PARAMETERS)
    { error="Statement `break' expected only inside loops and switch statements"; return pos; }*/
    
    lastnamed[level]=LN_CLOSING_SYMBOL;
    assop[level]=1;
    pos+=4+(code[pos]=='b');
  }
  else
  {
    if (code.substr(pos,4)=="case")
    {
      int a=lower_to_level(LEVELTYPE_SWITCH_BLOCK,"`case' statement");
      if (a != -1) return a;
      if (levelt[level]!=LEVELTYPE_SWITCH_BLOCK)
      {
        error="Statement `case' expected only in switch statement"; return pos; }
      level++; 
      levelt[level]=LEVELTYPE_CASE;
      statement_pad[level]=1;
    }
    else
    {
      level++; 
      levelt[level]=LEVELTYPE_GENERAL_STATEMENT;
      statement_pad[level]=1;
    }
    assop[level] = 1;
    lastnamed[level]=LN_OPERATOR;
    while (is_letterd(code[pos])) pos++;
  }
   
  return -1;
}


int close_statement(string& code,int& pos)
{
  assop[level]=0;
  number_of_statements++;
  while (level>0)
  {
    if (statement_pad[level]>=0) statement_pad[level]--;
    if (levelt[level]==LEVELTYPE_SWITCH and statement_pad[level]==1)
    {
      if (code[pos]!='{')
      { error="Expected `{' following `switch' statement"; return pos; }
    }
    else if (statement_pad[level]==0) //if it's less than zero we can assume it hasn't been initialized
    {
      if (levelt[level]==LEVELTYPE_DO) //We just learned this isn't an until statement
      {
        error="Statement `until' or `while' expected at this point, following `do' statement";
        return pos;
      }
      level--;
    }
    else break;
  }
  assop[level]=0;
  lastnamed[level]=LN_NOTHING;
  return -1;
}
