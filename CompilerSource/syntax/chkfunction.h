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

//Uses the same variables as the other code. Like copypasting, only easier to understand
  int checkfunction(externs* func,string &code,int pos,int len,int canargs = 1)
  {
    int cnt = 0, args = 0;
    bool exhausted = false;
    int argcmin=func->parameter_count_min();
    int argcmax=func->parameter_count_max();
    while (cnt > -1) //While we're still in parentheses
    {
      if (++pos >= len) { error="List of function parameters unterminated"; return pos; }
      
      if (is_useless(code[pos]))
        continue;
      
      if (!exhausted) {
        exhausted = true; 
        args++;
      }
      
      if (code[pos]=='"') {
        pos++; while (code[pos] != '"')  pos++; if (args==0) args=1; //TODO: Replace with skipstrings();
      }
      else if (code[pos]=='\'') {
        pos++; while (code[pos] != '\'') pos++; if (args==0) args=1; //TODO: Replace with skipstrings();
      }
      
      if (code[pos]=='[') { pos++; int bcnt=1; while(bcnt>0 && pos<len) { pos++; if (code[pos]=='[') bcnt++; if (code[pos]==']') bcnt--; } }
      
      if (code[pos]=='(') { cnt++; if (args==0) args=1; }
      if (code[pos]==',' && cnt==0)
      {
        exhausted = false;
        if (args > argcmax and argcmax != -1)
          return pos;
      }
      if (code[pos]==')') cnt--;
      
    }
    
    if (args < argcmin) 
    {
      error = "Too few arguments to function `"+func->name+"': Provided "+tostring(args)+", requested "+tostring(argcmin);
      return pos; 
    }
    
    return -1;
  }
