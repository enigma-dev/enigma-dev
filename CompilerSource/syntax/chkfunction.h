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

//Uses the same variables as the other code. Like copypasting, only easier to understand
  int checkfunction(string &code,int pos,int len,int canargs=1)
  { 
    int posfs,posfe=pos-1;
    while (!is_letterd(code[posfe])) posfe--; posfs=posfe;
    while (posfs>=0 && is_letterd(code[posfs])) posfs--;
    posfs++;
    string fname=code.substr(posfs,posfe+1-posfs);
    
    while (is_digit(fname[0])) fname.erase(0,1);
    
    externs *func=NULL;
    varray<externs> *f=&(extarray[fname]);
    for (unsigned int i=0;i<f->size;i++) 
      if ((*f)[i].flags & EXTFLAG_FUNCTION)
        { func=&((*f)[i]); break; }
        
    if (func==NULL) //not a function
      error=(char*)(("`"+fname+"' is not a recognized function or script").c_str()); return posfs;
    
    int cnt=0,args=0;
    int args_exceeded_at=-1;
    
    int argc=func->fargs.size;
    while (cnt>-1)
    {
      pos++;
      
      if (code[pos]=='"') { pos++;  while (code[pos]!='"')  pos++; if (args==0) args=1; }
      if (code[pos]=='\'') { pos++; while (code[pos]!='\'') pos++; if (args==0) args=1; }
      
      if (code[pos]=='[') { pos++; int bcnt=1; while(bcnt>0 && pos<len) { pos++; if (code[pos]=='[') bcnt++; if (code[pos]==']') bcnt--; } }
      
      if (code[pos]=='(') { cnt++; if (args==0) args=1; }
      if (code[pos]==',' && cnt==0)
      {
        if (args==0) args=1; args++;
        if (args>argc) if (args_exceeded_at==-1) args_exceeded_at=pos;
      }
      if (code[pos]==')') cnt--;
      
      if (!(pos<len)) { error="List of function parameters unterminated"; return pos; }
    }
    
    if (argc==-100) return -1;
    if (argc==-116)
    { if (args<=16) return-1; else { error="Too many arguments to script, provided "+tostring(args)+", limit is 16"; return pos; } }
    
    if (argc>=0)
    {
      if (args>argc) { string err="Too many arguments to function `"+fname+"': Provided "+tostring(args)+", requested "+tostring(argc); error=(char*)err.c_str(); return (args_exceeded_at!=-1)?args_exceeded_at:pos; }
      if (args<argc) { string err="Too few arguments to function `"+fname+"': Provided "+tostring(args)+", requested "+tostring(argc); error=(char*)err.c_str(); return pos; }
    }
    
    return -1;
  }
