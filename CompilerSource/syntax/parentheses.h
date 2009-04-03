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

int eoq(string& code,int pos)
  {
    char t=code[pos];
    pos++; int len=code.length();
    while (code[pos] != t) { pos++; if (!(pos<len)) return -1; }
    return pos;
  }
  
  int eop(string& code,int pos)
  {
    int cnt=0,len=code.length();
    while (pos<len)
    {
      if (code[pos]=='(')
      { cnt++; }
      if (code[pos]==')')
      { cnt--; if (cnt<=0) return pos; }
      if (code[pos]=='\'' || code[pos]=='"') { pos=eoq(code,pos);  if (pos==-1) return -1; }
      pos++;
    }
    return -1;
  }
  
  int eob(string& code,int pos)
  {
    int cnt=0,len=code.length();
    while (pos<len)
    {
      if (code[pos]=='{')
      { cnt++; }
      if (code[pos]=='}')
      { cnt--; if (cnt<=0) return pos; }
      if (code[pos]=='\'' || code[pos]=='"') { pos=eoq(code,pos);  if (pos==-1) return -1; }
      pos++;
    }
    return -1;
  }
  
  
  int par_empty(string& code,int pos)
  {
    int cnt=0,len=code.length(); int empty=1;
    while (pos<len)
    {
      if (code[pos]=='(')
      { cnt++; }
      if (code[pos]==')')
      { cnt--; if (cnt<=0) return empty; }
      if (code[pos]=='\'' || code[pos]=='"') { pos=eoq(code,pos); empty=0; if (pos==-1) return -1; }
      if is_letterd(code[pos]) empty=0;
      pos++;
    }
    return -1;
  }
