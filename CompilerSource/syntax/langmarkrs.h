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

      if (code.substr(pos,3)=="cpp" && !is_letterd(code[pos+3])) 
      {
        must_be_out_of_parenthesis("`cpp' directive")
        assignment_must_be_closed("s`cpp' directive")
        error_if_previous_operator("`cpp' directive")
        declaration_must_be_closed("`cpp' directive")
        error_if_fragment()
        close_all_open()
        
        pos+=3;
        
        while (is_useless(code[pos])) pos++;
        if (code[pos]!='{') { error="Symbol `{' expected following `cpp' marker"; return pos; }
        
        int ps=pos; pos++;
        int bcnt=1;
        while (pos<len && bcnt>0) { if (code[pos]=='{') bcnt++; if (code[pos]=='}') bcnt--; pos++; }
        
        if (bcnt>0) {error="Non-terminating C++ code block"; return ps; }
      }
      
      if (code.substr(pos,3)=="asm" && !is_letterd(code[pos+3])) 
      {
        must_be_out_of_parenthesis("`asm' directive")
        assignment_must_be_closed("`asm' directive")
        error_if_previous_operator("`asm' directive")
        declaration_must_be_closed("`asm' directive")
        error_if_fragment()
        close_all_open()
        
        pos+=3;
        
        while (is_useless(code[pos])) pos++;
        if (code[pos]!='{') { error="Symbol `{' expected following `asm' marker"; return pos; }
        
        int ps=pos; pos++;
        int bcnt=1;
        while (pos<len && bcnt>0) { if (code[pos]=='{') bcnt++; if (code[pos]=='}') bcnt--; pos++; }
        
        if (bcnt>0) {error="Non-terminating assembly code block"; return ps; }
      }
