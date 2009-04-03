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

if (code[pos]=='+' || code[pos]=='-')
      {
        int pos2=pos+1; while (is_useless(code[pos2])) pos2++;
        if (!is_letterd(code[pos2]) && !is_unary(code[pos2]) && code[pos2]!='(' && code[pos2]!='\'' && code[pos2]!='"' && code[pos+1]!='=' && code[pos2]!='.')
        { error="Expected expression following `+' or `-' symbol"; return pos;}

        if (!assop && cnt<1 && !infunction[cnt] && code[pos+1] != '=')
        { error="Assignment operator expected"; return pos; }

        varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0; pos++;
        mathop[cnt]=1; continue;
      }

      if (code[pos]=='*')
      {
        int posa=pos+1; while (is_useless(code[posa])) posa++;
        if (!(is_letterd(code[posa]) || code[posa]=='(' || is_unary(code[posa]))  //Something like that must always follow multiplication and dereferencing
         && code[posa]!='\'' && code[posa]!='"' && code[pos+1]!='=' && code[posa]!='.')
        { error="Expected secondary expression after `*' symbol"; return pos; }

        else
        {
          int posb=pos-1; while (is_useless(code[posb])) posb--;

          if (!(varnamed[cnt] || numnamed[cnt] || fnamed[cnt])) //If the previous thing isn't an expression, assume it's unary
          {
            if (!is_letter(code[posa]) && code[posa]!='(') //We know it's a letter or digit, but we need to be sure it's a letter, cuz this is a dereferencer
            {
              if (code[posa]!='*')
              { error="Expected variable name or temporary object after unary `*' symbol"; return pos; }
              else { varnamed[cnt]=numnamed[cnt]=0; pos++; continue; }
            }
          }
          else
          {
            int pos2=pos+1; while (is_useless(code[pos2])) pos2++;
            if (!is_letterd(code[pos2]) && !is_unary(code[pos2]) && code[pos2]!='(' && code[pos+1]!='='  && code[pos2]!='.')
            { error="Expected expression following `*' symbol"; return pos;}

            if (!assop && cnt<1 && !infunction[cnt] && code[pos+1] != '=')
            { error="Assignment operator expected"; return pos;}
          }
        }

        varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0;
        mathop[cnt]=1; pos++; continue;
      }


      if (code[pos]=='/' || code[pos]=='%')
      {
        if (mathop[cnt]) { error="Unexpected symbol in expression"; return pos; }
        if (!(varnamed[cnt] || numnamed[cnt] || fnamed[cnt])) { error="Expected primary expression before operator"; return pos; }

        if (!(assop || cnt || infunction[cnt]) && code[pos+1]!='=') { error="Assignment operator expected"; return pos; }

        mathop[cnt]=1; varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0;
        pos++; continue;
      }

      if (code[pos]=='>' ||  code[pos]=='<' ||  code[pos]=='^' ||  code[pos]=='|' ||  code[pos]=='&')
      {
        if (mathop[cnt]) { error="Unexpected symbol in expression"; return pos; }
        if (!(varnamed[cnt] || numnamed[cnt] || fnamed[cnt])) { error="Expected primary expression before operator"; return pos; }

        if ((code[pos]==code[pos+1]) || ((code[pos]=='<') && (code[pos+1]=='>'))) pos++;
        if (ford[lvl]==3) assop=1;
        if (!(assop || cnt || infunction[cnt]) && code[pos+1]!='=') { error="Assignment operator expected"; return pos; }

        mathop[cnt]=1; varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0;
        pos++; continue;
      }

      if (code[pos]=='!' || code[pos]=='~')
      {
        if (varnamed[cnt] || numnamed[cnt] || fnamed[cnt])
        {
          if (code[pos+1]=='=' && code[pos]=='!')
          {
            mathop[cnt]=1;
            varnamed[cnt]=fnamed[cnt]=0;
            pos++; continue;
          }
          else { error="Unary operator given where assignment or mathematical operator expected"; return pos; }
        }
        int posa=pos+1; while (is_useless(code[posa])) posa++;
        if (is_unary(code[posa]) && !(is_letterd(code[posa])))
        { error="Unary operator is performed on what now?"; return pos; }
        pos++; continue;
      }
