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

      if (is_letter(code[pos]))
      {
        separate_statement_outside_lists()
        error_if_previous_value()
        
        treat_as_varname()
        
        while (is_letterd(code[pos])) pos++;
        continue;
      }
      
      if (is_digit(code[pos]))
      {
        error_if_not_expecting_value()
        error_if_previous_value()
        
        treat_as_number()
        hexnum=0;
        
        while (pos<len && is_digit(code[pos])) pos++;
        continue;
      }
      
      if (code[pos]=='.')
      {
        no_out_of_class_declaration()
        
        if (!varnamed[cnt]) { if (hexnum) { error="Unexpected decimal in hexadecimal constant"; return pos; }
                              else { numnamed[cnt]=0; pos++; continue; } }
        else
        {
          int pos2=pos+1;
          while (is_useless(code[pos2])) pos2++;
          if (!is_letter(code[pos2])) { error="Variable name expected following `.' symbol"; return pos2; }
          varnamed[cnt]=fnamed[cnt]=0; pos++; continue;
        }
      }
      
      if (code[pos]=='$')
      {
        error_if_not_expecting_value()
        error_if_previous_value()
        
        treat_as_number()
        hexnum=1;
        
        pos++;
        while (pos<len && (is_digit(code[pos])
              ||          (code[pos]>='A' && code[pos]<='F') 
              ||          (code[pos]>='a' && code[pos]<='f'))
               ) pos++;
        
        continue;
      }
