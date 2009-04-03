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

if (code[pos]=='(')
      {
        if (!varnamed[cnt]) //Isn't a function
        {
          if (par_empty(code,pos)) { error="Empty parentheses"; return pos; }
          
          if (!mathop[cnt] && !assop)
          {
            int a=check_cast(code,pos);
            if (a==-2)
            {
              error="Unterminating parenthesis";
              return pos;
            }
            if (a==-1)
            {
              error="Invalid cast type";
              return pos;
            }
            if (a==1) 
            {
              pos=eop(code,pos)+1;
              if (pos==-1) { error="Unterminating parenthesis"; return pos; }
              
              if ((assop || (cnt>0)) && (varnamed[cnt] || numnamed[cnt] || fnamed[cnt]))
              {
                if (cnt>0) if (!infunction[cnt]) 
                { error="Operator expected at this point: Cannot have multiple statements inside parentheses"; return pos; }
                varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0;
                opsep();
                if (!cnt) assop=0;
              }
              if (varnamed[cnt] || numnamed[cnt]) { error="Operator expected at this point"; return pos; }
              
              mathop[cnt]=1;
              continue;
            } //move to the end of the cast
          }
          else 
          {
            int a=check_cast(code,pos);
            if (a==-3) { return pos; }
            if (a==-2) { error="Nonterminating parenthesis"; return pos; }
            if (a==-1) { error="Invalid cast type"; return pos; }
            if (a==1) 
            {
              pos=eop(code,pos)+1;
              if (pos==-1) { error="Nonterminating parenthesis"; return pos; }
              
              //already a complete statement
              if ((assop || (cnt>0)) && (varnamed[cnt] || numnamed[cnt] || fnamed[cnt]))
              {
                if (cnt>0) if (!infunction[cnt]) 
                { error="Operator expected at this point: Cannot have multiple statements inside parentheses"; return pos; }
                varnamed[0]=numnamed[0]=fnamed[0]=0;
                opsep();
                assop=0;
              }
              if (varnamed[cnt] || numnamed[cnt]) { error="Operator expected at this point"; return pos; }
              
              mathop[cnt]=1;
              continue;
            } //move to the end of the cast
          }
          
          if (numnamed[cnt]) { error="Numbers are not functions"; return pos; }
          
          cnt++; bracket[cnt]=0; pos++; 
          infunction[cnt]=mathop[cnt]=varnamed[cnt]=fnamed[cnt]=numnamed[cnt]=infunction[cnt]=0;
          continue;
        }
        else //Is a function
        {
          if (inDecList) if (!assop) { error="Cannot declare a function in event code"; return pos; }
          
          int a=checkfunction(pos,code,len);
          if (a>=0) return a; 
          
          varnamed[cnt]=0; fnamed[cnt]=1; assop=1;
          cnt++; bracket[cnt]=0; mathop[cnt]=varnamed[cnt]=fnamed[cnt]=numnamed[cnt]=0;
          infunction[cnt]=1;
          
          pos++; continue;
        }
      }
      
      if (code[pos]==',')
      {
        if (!infunction[cnt] && !inDecList && !bracket[cnt])
        { error="Symbol `,' used outside of list"; return pos; }
        if (inDecList && !(varnamed[cnt] || assop)) { error="Declaration doesn't declare anything"; return pos; }
        if (inDecList) assop=0;
        if (mathop[cnt]) { error="Secondary expression expected before `,' symbol"; return pos; }
        varnamed[cnt]=0; mathop[cnt]=varnamed[cnt]=numnamed[cnt]=fnamed[cnt]=0; pos++; continue;
      }
      
      if (code[pos]==')')
      {
        error_if_previous_operator("`)' symbol")
        close_all_open()
        
        cnt--;
        if (cnt==-1 && infor)
        {
          if (ford[lvl]!=1) { error="Expected three expressions in parentheses following `for' statement, provided "+tostring(4-ford[lvl]); return pos; }
          infor=0; stated[lvl]=1; cnt=0;
        }
        else
        {
          if (cnt<0 && !infor) { error="Unnecessary closing parenthesis at this point"; return pos; }
          
          fnamed[cnt]=infunction[cnt+1];
          numnamed[cnt]=!fnamed[cnt];
          varnamed[cnt]=0; mathop[cnt]=0;
        }
        
        pos++; continue;
      }
      




if (code[pos]=='[')
{
  int posb=pos-1; while (is_useless(code[posb])) posb--;
  if (!(varnamed[cnt] || code[posb]==')' || code[posb]==']')) { error="Variable name expected before `[' symbol"; return pos; }
  
  cnt++;
  bracket[cnt]=1;
  
  pos++; 
  infunction[cnt]=mathop[cnt]=varnamed[cnt]=fnamed[cnt]=numnamed[cnt]=infunction[cnt]=0;
  continue;
}

if (code[pos]==']')
{
  if (!bracket[cnt]) { error="Unexpected closing bracket"; return pos; }
  bracket[cnt]=1;
  cnt--;
  
  varnamed[cnt]=1;
  pos++; continue;
}
