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

typedef size_t pt;

inline bool is_wordop(string name)
{
  switch (name[0]) {
    case 'a':
      return name == "and";
    case 'd':
      return name == "div";
    case 'm':
      return name == "mod";
    case 'n':
      return name == "not";
    case 'o':
      return name == "or";
    case 'x':
      return name == "xor";
  }
  return false;
}

inline bool is_statement(string name)
{
  switch (name[0])
  {
    case 'b':
      return name == "begin" or name == "break";
    case 'c':
      return name == "case" or name == "catch" or name == "continue";
    case 'd':
      return name == "default" or name == "do";
    case 'e':
      return name == "else" or name == "end" or name == "exit";
    case 'f':
      return name == "for";
    case 'i':
      return name == "if";
    case 'r':
      return name == "return" or name == "repeat";
    case 's':
      return name == "switch";
    case 't':
      return name == "try" or name == "then";
    case 'u':
      return name == "until";
    case 'w':
      return name == "while" or name == "with";
  }
  return false;
}

//this code assumes we are at a variable name that is not an operator
inline int statement_completed(int lt,int pos=0)
{
  if (lt==LN_CLOSING_SYMBOL)
    return 1;

  if (lt==LN_OPERATOR)
    return 0;

  if (lt==LN_VALUE ||  lt==LN_DIGIT)
    return 1;

  if (assop[level])
  if (lt==LN_VARNAME)
    return 1;

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
    if (levelt[chklvl] == leveltype)
      return 1;

    if (levelt[chklvl] == LEVELTYPE_DO)
    { return 0; }
    if (levelt[chklvl] == LEVELTYPE_SWITCH_BLOCK)
    { return 0; }
    if (levelt[chklvl] == LEVELTYPE_BRACE)
    { return 0; }

    chklvl--;
  }
  return 0;
}
inline bool check_level_breakable()
{
  int chklvl=level;
  while (chklvl>0)
  {
    cout << "ENUM " << levelt[chklvl] << " " << LEVELTYPE_SWITCH_BLOCK << endl;
    if (levelt[chklvl]==LEVELTYPE_LOOP)
      return 1;
    
    if (levelt[chklvl]==LEVELTYPE_BRACE)
      return 1;

    if (levelt[chklvl]==LEVELTYPE_DO or levelt[chklvl]==LEVELTYPE_SWITCH_BLOCK)
      return 1;

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


int close_statement(string& code,pt& pos);

enum {
  PAD_STRUCT_NOTHING,
  PAD_STRUCT_IDENTIFIER,
  PAD_STRUCT_COLON,
  PAD_STRUCT_FULL
};

inline pt handle_if_statement(string& code,string name,pt& pos)
{
  pt ret = pt(-1);
  if (lastnamed[level]==LN_OPERATOR)
  { error="Expected arithmetic expression before statement"; ret = pos; }

  if ((statement_pad[level]>0 and levelt[level]==LEVELTYPE_GENERAL_STATEMENT)
  or  (statement_pad[level]>1 and levelt[level]==LEVELTYPE_SWITCH)
  or  (statement_pad[level]>2 and levelt[level]==LEVELTYPE_IF)
  or  (statement_pad[level]>2 and levelt[level]==LEVELTYPE_DO))
  { error="Second statement given in parameters of first"; ret = pos; }

  switch (name[0])
  {
    case 'b':
        if (name == "begin") goto label_begin;
        if (name == "break") goto label_break;
      break;
    case 'c':
        if (name == "case")    goto label_case;
        if (name == "catch")   goto label_catch;
        if (name == "continue")goto label_continue;
        if (name == "class")   goto label_struct;
      break;
    case 'd':
        if (name == "default") goto label_default;
        if (name == "do")      goto label_do;
      break;
    case 'e':
        if (name == "else")    goto label_else;
        if (name == "end")     goto label_end;
        if (name == "exit")    goto label_exit;
      break;
    case 'f':
        if (name == "for")     goto label_for;
      break;
    case 'i':
        if (name == "if")      goto label_if;
      break;
    case 'r':
        if (name == "return")  goto label_general;
        if (name == "repeat")  goto label_loop;
      break;
    case 's':
        if (name == "switch")  goto label_switch;
        if (name == "struct")  goto label_struct;
      break;
    case 't':
        if (name == "try")     goto label_general;
        if (name == "then")    goto label_then;
      break;
    case 'u':
        if (name == "until")   goto label_until;
      break;
    case 'w':
        if (name == "while")   goto label_while;
        if (name == "with")    goto label_with;
  }
  error = "";
  return pt(-2);


  label_if:
      level++;
      lastnamed[level] = LN_OPERATOR;
      assop[level] = 1;
      levelt[level] = LEVELTYPE_IF;
      statement_pad[level] = 3;
    return ret;
  label_then:
    if (levelt[level] != LEVELTYPE_IF or statement_pad[level] != 2)
      { error="Unexpected `then' statement at this point"; return pos; }
    statement_pad[level]--;
    return ret;
  label_do:
      level++;
      lastnamed[level]=LN_NOTHING;
      assop[level] = 0;
      levelt[level]=LEVELTYPE_DO;
      statement_pad[level]=2;
    return ret;
  label_else:
      if (level<=0)
      { error="Unexpected `else' statement at this point"; return pos; }

      if (lower_to_level(LEVELTYPE_IF,"`else' statement") != -1)
        return pos;

      if (statement_pad[level]!=1)
      { error="Statement `else' expected two expressions after `if' statement, given "+tostring(3-statement_pad[level]); return pos; }

      levelt[level] = LEVELTYPE_ELSE;
      statement_pad[level] = 1;
      lastnamed[level]=LN_NOTHING; //end of if level, start of else level
    return ret;

  label_while:
      if ((levelt[level] == LEVELTYPE_DO and statement_pad[level] == 1) or (check_level_accessible(LEVELTYPE_DO) and get_closest_level_pad(LEVELTYPE_DO) == 2))
        goto label_until;
    goto label_loop;

  label_until:
      if (level <= 0)
        goto label_loop;

      if (lower_to_level(LEVELTYPE_DO,"`"+name+"' statement") != -1)
        return pos;
      
      if (levelt[level] != LEVELTYPE_DO) {
        cout << "shit happens, rama rama.\n";
        goto label_loop;
      }
      
      if (statement_pad[level] == 2)
        goto label_loop;
      
      if (statement_pad[level] != 1)
      { error="Statement `until' expected one expression after `do' statement, given "+tostring(2-statement_pad[level]); return pos; }

      //level--; level++;
      levelt[level]=LEVELTYPE_GENERAL_STATEMENT;
      statement_pad[level]=1;
      lastnamed[level]=LN_OPERATOR; //end of if level, start of else level
      assop[level]=1;
    return ret;

  label_switch:
      level++;
      lastnamed[level] = LN_OPERATOR;
      assop[level] = 1;
      levelt[level] = LEVELTYPE_SWITCH;
      statement_pad[level] = 2;
    return ret;

  label_for:
      level++;
      lastnamed[level] = LN_FOR;
      assop[level] = 0;
      levelt[level] = LEVELTYPE_FOR_PARAMETERS;
      statement_pad[level] = 3;
    return ret;

  label_default:
      if (levelt[level] != LEVELTYPE_SWITCH_BLOCK)
      { error="Statement `default' expected only in switch statement"; return pos; }
      pos += 7; while (is_useless(code[pos])) pos++;
      if (code[pos] == ':') pos++;
      lastnamed[level] = LN_NOTHING;
    return ret;

  label_break:
  label_continue:
      if (!check_level_breakable())
      { error = "Cannot break/continue from here: not in a loop"; return pos; }
  label_exit:
      if (levelt[level] == LEVELTYPE_FOR_PARAMETERS)
      { error="Unexpected `" + name + "' statement in `for' parameters"; return pos; }

      lastnamed[level] = LN_CLOSING_SYMBOL;
      assop[level]=1;
    return ret;

  label_catch:
    goto label_general;

  label_begin:
    {
      bool isbr=levelt[level]==LEVELTYPE_SWITCH;

      if (lastnamed[level]==LN_OPERATOR)
      { error="Unexpected brace at this point"; return pos; }

      level+=!isbr;
      levelt[level]=(isbr?LEVELTYPE_SWITCH_BLOCK:LEVELTYPE_BRACE);
      lastnamed[level]=LN_NOTHING; //This is the beginning of a glorious new level
      statement_pad[level]=-1;
      assop[level]=0;
    }
    return ret;

  label_end:
      if (level<=0)
      { error="Unexpected closing brace at this point"; return pos; }

      if (lastnamed[level]==LN_OPERATOR)
      { error="Expected secondary expression before closing brace"; return pos; }

      if (statement_completed(lastnamed[level]))
      {
        int cs = close_statement(code,pos);
        if (cs != -1) return cs;
      }

      lower_to_level(LEVELTYPE_BRACE,"`}' symbol");
      level--;
      statement_pad[level]=-1;
      lastnamed[level]=LN_CLOSING_SYMBOL;
    return ret;

  label_case:
    {
      int a=lower_to_level(LEVELTYPE_SWITCH_BLOCK,"`case' statement");
      if (a != -1) return a;
      if (levelt[level]!=LEVELTYPE_SWITCH_BLOCK) {
        error="Statement `case' expected only in switch statement";
        return pos;
      }

      level++;
      levelt[level] = LEVELTYPE_CASE;
      statement_pad[level] = 1;

      assop[level] = 1;
      lastnamed[level] = LN_OPERATOR;
    }
    return ret;

  label_general:
      level++;
      levelt[level]=LEVELTYPE_GENERAL_STATEMENT;
      statement_pad[level]=1;

      assop[level] = 1;
      lastnamed[level]=LN_OPERATOR;
    return ret;

  label_struct:
      level++;
      levelt[level]=LEVELTYPE_STRUCT;
      statement_pad[level] = PAD_STRUCT_NOTHING;

      assop[level] = 1;
      lastnamed[level]=LN_OPERATOR;
    return ret;

  label_with:
  label_loop:
      level++;
      levelt[level] = LEVELTYPE_LOOP;
      statement_pad[level] = 2; //Take a parameter, then an instruction to loop.

      assop[level] = 1;
      lastnamed[level]=LN_OPERATOR;
    return ret;

  return ret;
}


int close_statement(string& code,pt& pos)
{
  assop[level]=0;
  number_of_statements++;
  
  if (plevel > 0)
  {
    if (plevel > 1) {
      error = plevelt[plevel] == PLT_TEMPLATE_PARAMS ? "Expected at least a closing triangle bracket at this point" : plevelt[plevel] == PLT_BRACKET ? "Expected at least a closing bracket at this point" : "Expected at least one closing parenthesis at this point";
      return pos;
    }
    if (plevelt[plevel] != PLT_FORSTATEMENT) {
      error = plevelt[plevel] == PLT_TEMPLATE_PARAMS ? "Expected closing triangle bracket at this point" : plevelt[plevel] == PLT_BRACKET?"Expected closing bracket before this point" : "Expected closing parenthesis before this point";
      return pos;
    }
    // So, we're closing a for()
    const int ef = lower_to_level(LEVELTYPE_FOR_PARAMETERS,"otherwise meaningless parenthetical expression");
      if (ef != -1) return pos;
    if (statement_pad[level] > 0)
      statement_pad[level]--;
    else {
      error = "Expected ending parenthesis before this point";
      return pos;
    }
    return -1;
  }
  while (level>0)
  {
    if (statement_pad[level] > 0)
      statement_pad[level]--;
    if (levelt[level]==LEVELTYPE_SWITCH and statement_pad[level]==1)
    {
      if (code[pos]!='{')
      { error="Expected `{' following `switch' statement"; return pos; }
    }
    else if (statement_pad[level]==0) //if it's less than zero we can assume it hasn't been initialized
    {
      if (levelt[level]==LEVELTYPE_DO) { //We just learned this isn't an until statement
        error="Statement `until' or `while' expected at this point, following `do' statement";
        return pos;
      }
      level--;
      //if (levelt[level]
    }
    else break;
  }
  assop[level]=0;
  lastnamed[level]=LN_NOTHING;
  return -1;
}
