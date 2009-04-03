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

#include <map>
#include <string>
#include <stdio.h>
#include "textfile.h"
using namespace std;

extern int __constcount;
extern map<int,string> __consts;

extern int __enigmaglobalcount;
extern map<int,string> __enigmaglobals;


void load_gnames()
{
   int b=0;
   __constcount=0;
   int a=file_text_open_read("consts.txt");
   if (a==-1) { printf("Failed to load constants. File does not exist...\r\n"); }
   else
   {
      while (!file_text_eof(a))
      {
         __consts[__constcount]=file_text_read_string(a);
         file_text_readln(a); __constcount++; b++; 
      }
      file_text_close(a);
   }
   #if DEBUGMODE
     printf("Read %d constants\n",b);
   #endif
   
   b=0;
   __enigmaglobalcount=0;
   a=file_text_open_read("globals.txt");
   if (a==-1) { printf("Failed to load globals. File does not exist...\r\n"); }
   else
   {
      while (!file_text_eof(a))
      {
         __enigmaglobals[__enigmaglobalcount]=file_text_read_string(a);
         file_text_readln(a); __enigmaglobalcount++; b++;
      }
      file_text_close(a);
   }
   #if DEBUGMODE
     printf("Read %d globals\n",b);
   #endif
}

