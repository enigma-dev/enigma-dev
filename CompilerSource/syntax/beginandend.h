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
 if (code.substr(pos,5)=="begin" && !is_letterd(code[pos+5]))
      {
        if (cnt==1) { error="Expected closing parenthesis before `begin' statement"; return pos; }
        else if (cnt>0) { error=(char*)(("Expected "+tostring(cnt)+" closing parentheses before `begin' statement").c_str()); return pos; }
        if ((assop && !varnamed[cnt] && !numnamed[cnt] && !fnamed[cnt])||mathop[cnt]) { error="Expected secondary expression before `begin' statement"; return pos; }
        if (mathop[cnt]) { error="Secondary expression expected before `begin' statement"; return pos; }
        
        varnamed[cnt]=0; if (!cnt) assop=0; opsep(); 
        mathop[cnt]=varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0; 
        
        lvl++; pos+=5; continue;
      }
      
      if (code.substr(pos,3)=="end" && !is_letterd(code[pos+3]))
      {
        if (cnt==1) { error="Expected closing parenthesis before `end' statement"; return pos; }
        else if (cnt>0) { error=(char*)(("Expected "+tostring(cnt)+" closing parentheses before `end' statement").c_str()); return pos; }
        if ((assop && !varnamed[cnt] && !numnamed[cnt] && !fnamed[cnt])||mathop[cnt]) { error="Expected secondary expression before `end' statement"; return pos; }
        if (mathop[cnt]) { error="Secondary expression expected before `end' statement"; return pos; }
        
        lvl--; if (lvl<0) { error="Keyword `end' used without cooresponding `begin'"; return pos; }
        pos+=3; continue;
      }
      
      
      
