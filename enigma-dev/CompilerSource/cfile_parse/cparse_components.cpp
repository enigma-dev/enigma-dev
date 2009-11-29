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

#include <string>
using namespace std;

extern string cferr;
unsigned int handle_comments(string &cfile,unsigned int &pos,const unsigned int len)
{
  if (cfile[pos]=='/')
  {
    pos++;
    if (cfile[pos]=='/')
    {
      while (cfile[pos] != '\r' and cfile[pos] != '\n' and (pos++)<len);
      return unsigned(-2);
    }
    if (cfile[pos]=='*')
    {
      int spos=pos;
      pos+=2;
      
      while ((cfile[pos] != '/' or cfile[pos-1] != '*') and (pos++)<len);
      if (pos>=len)
      {
        cferr="Unterminating comment";
        return spos;
      }
      
      pos++;
      return unsigned(-2);
    }
    pos--;
  }
  return unsigned(-1);
}
