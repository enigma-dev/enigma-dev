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

#include <stdlib.h>
#include <map>
#include <string>

using namespace std;

#include "syncheck.h"
#include "parser.h"

#define is_letter(x)  ((x>='a' && x<='z') || (x>='A' && x<='Z') || (x=='_'))
  #define is_digit(x)   (x>='0' && x<='9')
  #define is_letterd(x) (is_letter(x) || is_digit(x))
  #define is_unary(x)   (x=='!' || x=='~' || x=='+' || x=='-' || x=='&' || x=='*')
  #define is_linker(x)  (x=='+' || x=='-' || x=='*' || x=='/' || x=='=' || x=='!' || x=='~' || x=='&' || x=='|' || x=='^' || x=='.')
#define is_useless(x) (x==' ' || x=='\r' || x=='\n' || x=='\t')

int file_check(string filename)
{
    FILE* in=fopen(filename.c_str(),"rb");
    if (in==NULL) return -10;

    fseek(in,0,SEEK_END);
    int size=ftell(in);
    fseek(in,0,SEEK_SET);

    char* in2=new char[size+1];
    fread(in2,1,size,in);
    in2[size]='\0';
    fclose(in);

    if (*in2!='1') { puts("An error occurred during syntax check. Version error."); return 0; }
    in2++; if (*in2=='\r') { in2++; if (*in2=='\n') in2++; }
    else if (*in2=='\n') in2++;
    else { puts("An error occurred during syntax check. No newline following version."); return 0; }

    /* Read scripts from beginning of file */

    /*Clear scripts: Deprecated as of R4
    if (syncheck::scrcount>0)
    { syncheck::scrcount=0; syncheck::scrnames.clear(); }*/

    //Segfaulting piece o' shit
    for (int i=0;in2[i]!='\r' && in2[i]!='\n';i++)
    {
        if (!is_letter(in2[i]))
        { printf("An error occurred during syntax check. Invalid script name \"%s\".",in2); return 0; }
        i++; while is_letterd(in2[i]) i++;

        if (in2[i]!=',') { puts("An error occurred during syntax check. Format error."); return 0; }
        in2[i]=0; i++;

        syncheck::addscr(in2);
        in2+=i; i=0;
    }


    if (*in2=='\r') { in2++; if (*in2=='\n') in2++; }
    else if (*in2=='\n') in2++;
    else { puts("An error occurred during syntax check. No newline following list of script names."); return 0; }

    int retval=syncheck::syntacheck(in2);
    if (retval==-1) { string code=parser_main(in2); }

    int l=0,p=0; int i;
    for (i=0;i<retval;i++) { if (in2[i]=='\r') { l++; if (in2[i+1]=='\n') i++; p=i+1; } else if (in2[i]=='\n') { l++; p=i+1; } }
    printf("Line %d, position %d (absolute index %d): %s\r\n",(l)+1,(retval-p)+1,retval,syncheck::error.c_str());

    delete []in2;
    return retval;
}
