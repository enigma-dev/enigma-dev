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

int external_define(string dll,string func,int calltype,bool returntype,int argcount,
                    bool t01=0,bool t02=0,bool t03=0,bool t04=0,bool t05=0,bool t06=0,bool t07=0,bool t08=0,
                    bool t09=0,bool t10=0,bool t11=0,bool t12=0,bool t13=0,bool t14=0,bool t15=0,bool t16=0);

variant external_call(int id,variant a1=0,variant a2=0,variant a3=0,variant a4=0,variant a5=0,variant a6=0,variant a7=0,variant a8=0,
                             variant a9=0,variant a10=0,variant a11=0,variant a12=0,variant a13=0,variant a14=0,variant a15=0,variant a16=0);


