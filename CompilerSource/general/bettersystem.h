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

int e_exec(const char* fcmd, const char* *Cenviron = NULL);
int e_execp(const char* cmd, string path);
int e_execs(string cmd);
int e_execs(string cmd, string cat1);
int e_execs(string cmd, string cat1, string cat2);
int e_execs(string cmd, string cat1, string cat2, string cat3);
int e_execsp(string cmd, string path);
int e_execsp(string cmd, string cat1, string path);
int e_execsp(string cmd, string cat1, string cat2, string path);
int e_execsp(string cmd, string cat1, string cat2, string cat3, string path);

//int better_system(string,string,string="",const char* = NULL);

