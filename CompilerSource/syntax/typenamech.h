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

      while ((code.substr(pos,8)=="unsigned" && !is_letterd(code[pos+8]))
      ||     (code.substr(pos,6)=="signed"   && !is_letterd(code[pos+6]))
      ||     (code.substr(pos,6)=="static"   && !is_letterd(code[pos+6]))
      ||     (code.substr(pos,5)=="const"    && !is_letterd(code[pos+5]))
      ||     (code.substr(pos,5)=="short"    && !is_letterd(code[pos+5]))
      ||     (code.substr(pos,4)=="long"     && !is_letterd(code[pos+4]))
      //     ////////////////////////////////////////////////////////////
      ||     (code.substr(pos,7)=="cstring"  && !is_letterd(code[pos+7]))
      ||     (code.substr(pos,6)=="double"   && !is_letterd(code[pos+6]))
      ||     (code.substr(pos,5)=="float"    && !is_letterd(code[pos+5]))
      ||     (code.substr(pos,3)=="int"      && !is_letterd(code[pos+3]))
      ||     (code.substr(pos,3)=="var"      && !is_letterd(code[pos+3]))
      ||     (code.substr(pos,4)=="char"     && !is_letterd(code[pos+4]))
      ||     (code.substr(pos,4)=="bool"     && !is_letterd(code[pos+4]))
      //     ////////////////////////////////////////////////////////////
      ||     (code.substr(pos,6)=="localv"   && !is_letterd(code[pos+6])))
      {
        if       (code.substr(pos,6)=="localv")  { dectype[chkdec]=0; pos+=6; }
        
        else if (code.substr(pos,8)=="unsigned") { dectype[chkdec]=1; pos+=8; }
        else if (code.substr(pos,6)=="signed")   { dectype[chkdec]=2; pos+=6; }
        else if (code.substr(pos,6)=="static")   { dectype[chkdec]=3; pos+=6; }
        else if (code.substr(pos,5)=="const")    { dectype[chkdec]=4; pos+=5; }
        
        else if (code.substr(pos,5)=="short")    { dectype[chkdec]=11; pos+=5; }
        else if (code.substr(pos,4)=="long")     { dectype[chkdec]=12; pos+=4; }
        
        else if (code.substr(pos,7)=="cstring")  { dectype[chkdec]=101; pos+=7; }
        else if (code.substr(pos,6)=="double")   { dectype[chkdec]=102; pos+=6; }
        else if (code.substr(pos,5)=="float")    { dectype[chkdec]=103; pos+=5; }
        else if (code.substr(pos,3)=="int")      { dectype[chkdec]=104; pos+=3; }
        else if (code.substr(pos,3)=="var")      { dectype[chkdec]=105; pos+=3; }
        else if (code.substr(pos,4)=="char")     { dectype[chkdec]=106; pos+=4; }
        else if (code.substr(pos,4)=="bool")     { dectype[chkdec]=107; pos+=4; }
        
        while (is_useless(code[pos])) pos++;
        
        chkdec++;
      }
      if (chkdec>0)
      {
        must_be_out_of_parenthesis("declaration")
        assignment_must_be_closed("declaration")
        error_if_previous_operator("declaration")
        declaration_must_be_closed("new declaration")
        separate_statement_outside_lists()
        
        close_all_open()
        
        
        
        
        
        int actualnamed=0;
        for (int i=0;i<chkdec;i++)
        {
          if (dectype[i]>0  && dectype[i]<10)
          {
            if (i+1==chkdec) { error="Invalid declaration: No type named"; return pos; }
            if (actualnamed != 0) { error="Invalid declaration: Flag given after type name"; return pos; }
          }
          if (dectype[i]>10 && dectype[i]<100)
            if (actualnamed != 0) { error="Invalid declaration: Flag given after type name"; return pos; }
          if (dectype[i]>100)
          {
            if (actualnamed != 0) { error="Invalid declaration: Multiple types named"; return pos; }
            actualnamed=1;
          }
        }
        
        int find1=0,find2=0;
        for (int i=0;i<chkdec;i++) {
          if (dectype[i]==1) find1++;
          if (dectype[i]==2) find2++; }
        if (find1 && find2) { error="Invalid declaration: Conflicting flag types"; return pos; }
        if (find1>1 || find2>1) { error="Invalid declaration: Redundant flag types"; return pos; }
        
        find1=0; find2=0;
        for (int i=0;i<chkdec;i++) {
          if (dectype[i]==3) find1++;
          if (dectype[i]==4) find2++; }
        if (find1>1 || find2>1) { error="Invalid declaration: Redundant flag types"; return pos; }
        
        find1=0; find2=0;
        for (int i=0;i<chkdec;i++) {
          if (dectype[i]==11) find1++;
          if (dectype[i]==12) find2++; }
        if (find1 && find2) { error="Invalid declaration: Conflicting flag types"; return pos; }
        if (find1>2 || find2>2) { error="Invalid declaration: Excessive flag types"; return pos; }
        
        find1=0; find2=0;
        for (int i=0;i<chkdec;i++) {
          if (dectype[i]/10==1) find1++;
          if (dectype[i]/100==1) find2++; }
        if (find1>1 && find2>0) { error="Invalid declaration: Excessive flag types with type name"; return pos; }
        
        find1=0; find2=0;
        for (int i=0;i<chkdec;i++) {
          if (dectype[i]==11) find1++;
          if (dectype[i]==12) find2++; }
        if (find1 && find2) { error="Invalid declaration: Conflicting flag types"; return pos; }
        if (find1>2 || find2>2) { error="Invalid declaration: Excessive flag types"; return pos; }
        
        find1=0; find2=0;
        for (int i=0;i<chkdec;i++) {
          if (dectype[i]==1) find1++;
          if (dectype[i]==102 || dectype[i]==102) find2++; }
        if (find1 && find2) { error="Invalid declaration: Flag type conflicts with type name"; return pos; }
        
        chkdec=0;
        inDecList=1;
        continue;
      }
      
