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

      if((code.substr(pos,2)=="if"     && !is_letterd(code[pos+2]))
      || (code.substr(pos,4)=="with"   && !is_letterd(code[pos+4]))
      || (code.substr(pos,5)=="while"  && !is_letterd(code[pos+5]))
      || (code.substr(pos,5)=="until"  && !is_letterd(code[pos+5]))
      || (code.substr(pos,6)=="switch" && !is_letterd(code[pos+6]))
      || (code.substr(pos,6)=="repeat" && !is_letterd(code[pos+6]))
      || (code.substr(pos,6)=="return" && !is_letterd(code[pos+6])))
      {
        must_be_out_of_parenthesis("statement")
        assignment_must_be_closed("statement")
        error_if_previous_operator("statement")
        declaration_must_be_closed("statement")
        error_if_fragment()
        close_all_open()
        
        doesnt_belong_in_if("Statement")
        
        
        if((code[pos]=='w')
        || (code[pos]=='s')
        || ((code[pos]=='r') && (code.substr(pos,6)=="repeat")))
        iflvl[lvl]++; //move up an iflevel where necessary, so do {} until doesn't seize
        
        
        
        if (code.substr(pos,2)=="if") { dod[lvl][iflvl[lvl]]--; ifd[lvl][++iflvl[lvl]]=3; }
        else stated[lvl]=1;
        
        if (code[pos]=='u') 
        { 
          //printf("Statement `until' encountered where dod[%d][%d]=%d and ifd=%d\r\n",lvl,iflvl[lvl],dod[lvl][iflvl[lvl]],ifd[lvl][iflvl[lvl]]);
          if (ifd[lvl][iflvl[lvl]]>0) { ifd[lvl][iflvl[lvl]]=0; iflvl[lvl]--; }
          if (dod[lvl][iflvl[lvl]]!=1) { error="Statement `until' expected one expression after `do' statement"; return pos; }
          iflvl[lvl]--;
        }
        
        while (is_letterd(code[pos])) pos++;
        while (is_useless(code[pos]) && (pos<len)) pos++;
        
        if (!(pos<len)) { error="Expression expected following statement"; return pos; }
        if (!is_letterd(code[pos]) && code[pos]!='(' && code[pos]!='\'' && code[pos]!='"' && !is_unary(code[pos])) { error="Expression expected following statement"; return pos; }
        
        assop=1; mathop[cnt]=varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0; continue;
      }
      
      if (code.substr(pos,4)=="case" && !is_letterd(code[pos+4]))
      {
        must_be_out_of_parenthesis("`case' statement")
        assignment_must_be_closed("`case' statement")
        error_if_previous_operator("`case' statement")
        declaration_must_be_closed("`case' statement")
        error_if_fragment()
        close_all_open()
        
        doesnt_belong_in_if("Statement `case'")
        
        while (is_letterd(code[pos])) pos++;
        while (is_useless(code[pos]) && (pos<len)) pos++;
        
        if (!(pos<len)) { error="Expression expected following statement"; return pos; }
        if (!is_letterd(code[pos]) && code[pos]!='(' && code[pos]!='\'' && code[pos]!='"'  && !is_unary(code[pos])) { error="Expression expected following statement"; return pos; }
        
        int pos2=pos;
        while (pos2<len && code[pos2]!=':') pos2++;
        if (code[pos2]==':') code[pos2]=' ';
        
        assop=1; mathop[cnt]=varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0; continue;
      }
      
      if  (code.substr(pos,7)=="default" && !is_letterd(code[pos+7]))
      {
        must_be_out_of_parenthesis("`default' statement")
        assignment_must_be_closed("`default' statement")
        error_if_previous_operator("`default' statement")
        declaration_must_be_closed("`default' statement")
        doesnt_belong_in_if("Statement `default'")
        error_if_fragment()
        close_all_open()
        
        wasaloop=wasaswitch=0;
        
        opsep()
        
        while is_letter(code[pos]) pos++;
        while (is_useless(code[pos])) pos++;
        if (code[pos]==':') code[pos]=' ';
        
        continue;
      }
      
      if (code.substr(pos,6)=="delete" && !is_letterd(code[pos+6]))
      {
        must_be_out_of_parenthesis("`delete' statement")
        assignment_must_be_closed("`delete' statement")
        error_if_previous_operator("`delete' statement")
        declaration_must_be_closed("`delete' statement")
        doesnt_belong_in_if("Statement `delete'")
        error_if_fragment()
        close_all_open()
        
        wasaloop=wasaswitch=0;
        
        opsep()
        
        while is_letter(code[pos]) pos++;
        
        //DELETE stuff
        while (is_useless(code[pos])) pos++;
        while (code[pos]=='[') { pos++; while (is_useless(code[pos])) pos++; if (code[pos]!=']') {error="Expected only empty brackets after `delete' statement"; return pos; } pos++; while (is_useless(code[pos])) pos++; } 
        if (!is_letter(code[pos])) { error="Expected identifier following `delete' statement"; return pos; } pos++;
        while (is_letter(code[pos])) pos++;
        
        //End DELETE stuff
        
        continue;
      }
      
      if (code.substr(pos,3)=="new" && !is_letterd(code[pos+3]))
      {
        must_be_out_of_parenthesis("`new' statement")
        error_if_previous_operator("`new' statement")
        error_if_fragment()
        close_all_open()
                
        while (is_letterd(code[pos])) pos++;
        while (is_useless(code[pos]) && (pos<len)) pos++;
        
        if (!(pos<len)) { error="Expression expected following `new' statement"; return pos; }
        if (!is_letter(code[pos])) { error="Type name expected following `new' statement"; return pos; }
        
        int tn=0;
        
        while (code[pos]!=';')
        {
          int spos=pos;
          if (!is_letter(code[pos]))
          {
            if (tn && code[pos]=='[') break;
            error="Unexpected symbol in allocation expression"; return pos;
          }
          
          while (is_letterd(code[pos])) pos++;
          
          if ((typenames.find(code.substr(spos,pos))!=std::string::npos)
          &&  (typeflags.find(code.substr(spos,pos))!=std::string::npos))
          { error="Invalid type name following `new' statement"; return pos; }
          
          while (is_useless(code[pos])) pos++;
          
          tn=1;
        }
        
        if (!tn) { error="Invalid dynamic allocation..."; return pos; }
        
        assop=varnamed[cnt]=1; mathop[cnt]=numnamed[cnt]=fnamed[cnt]=0; continue;
      }
      
      
      
      if (code.substr(pos,2)=="do" && !is_letterd(code[pos+2]))
      {
        must_be_out_of_parenthesis("statement")
        assignment_must_be_closed("statement")
        error_if_previous_operator("statement")
        declaration_must_be_closed("statement")
        doesnt_belong_in_if("Statement")
        error_if_fragment()
        close_all_open()
        
        dod[lvl][++iflvl[lvl]]=2; pos+=2;
        continue;
      }
      
      if (code.substr(pos,3)=="for" && !is_letterd(code[pos+3]))
      {
        must_be_out_of_parenthesis("statement")
        assignment_must_be_closed("statement")
        error_if_previous_operator("statement")
        declaration_must_be_closed("statement")
        error_if_fragment()
        close_all_open()
        
        if (infor) { error="...were you even trying?"; return pos; }
        doesnt_belong_in_if("Statement")
        
        infor=1; ford[lvl]=4; pos+=3;
        
        while (is_useless(code[pos])) pos++; 
        if (code[pos]!='(') { error="Symbol `(' expected following `for' statement"; return pos; }
        pos++;
        
        continue;
      }
      
      
      if ((code.substr(pos,4)=="exit"     && !is_letterd(code[pos+4]))
      ||  (code.substr(pos,5)=="break"    && !is_letterd(code[pos+5]))
      ||  (code.substr(pos,8)=="continue" && !is_letterd(code[pos+8])))
      {
        must_be_out_of_parenthesis("statement")
        assignment_must_be_closed("statement")
        error_if_previous_operator("statement")
        declaration_must_be_closed("statement")
        doesnt_belong_in_if("Statement")
        error_if_fragment()
        close_all_open()
        
        wasaloop=wasaswitch=0;
        
        opsep()
        
        while is_letter(code[pos]) pos++;
        continue;
      }
      
      if (code.substr(pos,7)=="default" && !is_letterd(code[pos+7]))
      {
        must_be_out_of_parenthesis("statement")
        assignment_must_be_closed("statement")
        error_if_previous_operator("statement")
        declaration_must_be_closed("statement")
        error_if_fragment()
        close_all_open()
        doesnt_belong_in_if("Statement `default'")
        
        wasaloop=wasaswitch=0;
        
        opsep()
        
        while is_letter(code[pos]) pos++;
        while (is_useless(code[pos])) pos++;
        if (code[pos]!=':') { error="Colon expected following default"; return pos; }
        
        pos++; continue;
      }
      
      if (code.substr(pos,4)=="then" && !is_letterd(code[pos+4]))
      {
        must_be_out_of_parenthesis("statement")
        assignment_must_be_closed("statement")
        error_if_previous_operator("statement")
        declaration_must_be_closed("statement")
        //Do not error_if_fragment()
        close_all_open()
        
        if (infor) { error="Unexpected statement in for parameters"; return pos; }
        
        if (ifd[lvl][iflvl[lvl]]!=2)
        { error="Statement `then' must immediately follow an if statement."; return pos; }
        
        pos+=4; continue;
      }
      if (code.substr(pos,4)=="else" && !is_letterd(code[pos+4]))
      {
        must_be_out_of_parenthesis("statement")
        assignment_must_be_closed("statement")
        error_if_previous_operator("statement")
        declaration_must_be_closed("statement")
        //Do not error_if_fragment()
        close_all_open()
        
        if (ifd[lvl][iflvl[lvl]]!=1)
        { error="Statement `else' must be preceeded by `if' or `then' statements"; return pos; }
        stated[lvl]=1; ifd[lvl][iflvl[lvl]]=0; iflvl[lvl]--;
        dod[lvl][iflvl[lvl]]++;
        
        pos+=4; continue;
      }
      
