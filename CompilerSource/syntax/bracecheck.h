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

      if (code[pos]=='{')
      {
        must_be_out_of_parenthesis("`;' symbol")
        assignment_must_be_closed("`;' symbol")
        error_if_previous_operator("`;' symbol")
        error_if_fragment()
        
        terminate_declaration_list()
        close_all_open()
        
        int chkcnt=0;
        for (int i=pos;i<len;i++) 
        {
          if (code[i]=='{') chkcnt++; 
          else if (code[i]=='}') chkcnt--;
          if (chkcnt<=0) break;
        }
        if (chkcnt>0) { error="Nonterminating brace at this point"; return pos; }
        
        lvl++; ifd[lvl][iflvl[lvl]]=dod[lvl][iflvl[lvl]]=ford[lvl]=stated[lvl]=0;
        pos++; continue;
      }
      
      if (code[pos]=='}')
      {
        must_be_out_of_parenthesis("`}' symbol")
        assignment_must_be_closed("`}' symbol")
        error_if_previous_operator("`}' symbol")
        declaration_must_be_closed("`}' symbol")
        
        close_all_open()
        
        lvl--; if (lvl<0) { error="Unnecessary closing bracket"; return pos; }
        ifd[lvl][iflvl[lvl]]--; dod[lvl][iflvl[lvl]]--; stated[lvl]--; ford[lvl]--; pos++; continue;
      }
