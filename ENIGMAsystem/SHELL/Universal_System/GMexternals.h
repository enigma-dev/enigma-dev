/********************************************************************************\
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
\********************************************************************************/

/*

This was almost an attempt at it, but ehm, no.

*/

/*
namespace enigma
{
   struct extcall
   {
     FARTPROC fart;
     unsigned char argnum;
     char restp;
     char atype[12];
     
     extcall() { fart=NULL; argnum=-1; }
   }
     
   
   std::map<int,extcall> externs;
   int extcount=0;
}

int external_define(string dll,string name,double calltype,double restype,
    double argnum,double arg0=0,double arg1=0,double arg2=0,double arg3=0,double arg4=0,double arg5=0,double arg6=0,
    double arg7=0,double arg8=0,double arg9=0,double arg10=0,double arg11=0)
{
    HINSTANCE hGetProcIDDLL = LoadLibrary(dll.c_str());
    externs[extcount].fart=GetProcAddress(GetModuleHandle(dll.c_str()),name.c_str());
    externs[extcount].argnum=(unsigned char)argnum;
    externs[extcount].restp=(char)restype;
    externs[extcount].atype[0]=arg0; externs[extcount].atype[1]=arg1;
    externs[extcount].atype[2]=arg2; externs[extcount].atype[3]=arg3;
    externs[extcount].atype[4]=arg4; externs[extcount].atype[5]=arg5;
    externs[extcount].atype[6]=arg6; externs[extcount].atype[7]=arg7;
    externs[extcount].atype[8]=arg8; externs[extcount].atype[9]=arg9;
    externs[extcount].atype[10]=arg10; externs[extcount].atype[11]=arg11;
    return extcount++;
}

enigma::variant external_call(double funcint, 
enigma::variant arg1=0, enigma::variant arg1=1, enigma::variant arg1=2, enigma::variant arg1=3, enigma::variant arg1=4, enigma::variant arg1=5, 
enigma::variant arg1=6, enigma::variant arg1=7, enigma::variant arg1=8, enigma::variant arg1=9, enigma::variant arg1=10, enigma::variant arg1=11)
{
    void *a0,*a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9,*a10,*a11;
    
    enigma::extcall blah=enigma::externs[]
    if 
    
    
    
}*/
