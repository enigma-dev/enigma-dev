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

  
  int check_cast(string& code,int pos)
  {
    int posi=pos+1; 
    string n1;
    int len=code.length();
    int casted=0;
    int realnamed=0; string lastnamed="";
    
    if (eop(code,pos)==-1) return -2;
    
    while ((code[pos] != ')') && (pos<len))
    {
      pos++; 
      if (!is_letter(code[pos]))
      {
        if (code[pos]==' ' || code[pos]==')')
        {
          if (code.substr(posi,pos-posi)=="localv")
          {
            error="Cannot cast as local variable..."; return -3;
          }
          
          if (posi<pos && typeflags.find("|"+code.substr(posi,pos-posi)+"|")!=std::string::npos)
          {
            posi=pos;
            casted=1;
            lastnamed=code.substr(posi,pos-posi);
            if (realnamed) { error="Cast names additional flags after naming type"; return -3; }
          }
          else if (posi<pos && typenames.find("|"+code.substr(posi,pos-posi)+"|")!=std::string::npos)
          {
            posi=pos;
            casted=1;
            realnamed=1;
            lastnamed=code.substr(posi,pos-posi);
          }
          else if (posi<pos)
          {
            if (casted) return -1;
            return 0;
          }
          else while (code[pos]==' ') { posi++; pos++; }
          
          posi=pos;
          while (code[posi]==' ') posi++;
        }
      }
      if (!(pos<len)) return -2;
    }
    
    if (realnamed)
    return 1;
    
    if (lastnamed=="long" || lastnamed=="short") 
    return 1;
    
    error="Cast names no actual type";
    return -3;
  }
  
  
  
  
  
