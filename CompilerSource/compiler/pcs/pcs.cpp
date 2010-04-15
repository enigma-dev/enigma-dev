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

#include <map>
#include <string>
#include <stdlib.h> //system (Linux)
#include <cstdio>
using namespace std;
#include "../../general/estring.h"

namespace pcs
{
  string cargs="",largs="";
  map<int,string> pre,post;
  int prec,postc;
}

void load_filter_args()
{
   FILE* pcs=fopen("pcs.txt","rb");
   if (pcs==NULL) { printf("Failed to load Post-Compile Script.\r\n"); fflush(stdout); return; }

   printf("\n");
   int contpre=0,contpost=0;

   while (!feof(pcs))
   {
      string cmd; cmd="";
      char rb; rb=0;

      while (rb != EOF)
      {
         rb=fgetc(pcs);
         if (rb != EOF && rb != '\n' && rb != '\r')
         cmd+=rb;
         else if (cmd.length()>4)
         {
            if (cmd.substr(0,6)=="cargs:")
            {
               pcs::cargs=cmd.substr(6);
               printf("Accepted additional compile args \"%s\"\n",pcs::cargs.c_str());
               cmd=""; continue;
            }
            else if (cmd.substr(0,6)=="largs:")
            {
               pcs::largs=cmd.substr(6);
               printf("Accepted additional linker args \"%s\"\n",pcs::largs.c_str());
               cmd=""; continue;
            }
            else if (cmd.substr(0,4)=="pre:" || contpre)
            {
               pcs::pre[pcs::prec]=cmd.substr(4*(!contpre));
               printf("Accepted additional pre-resource command line \"%s\"\n",pcs::pre[pcs::prec].c_str());
               string* n=&(pcs::pre[pcs::prec]); contpre=0;
               if ((*n)[n->length()-1]=='\\') { n->erase(n->length()-1,1); if ((*n)[n->length()-2]!='\\') contpre=1; }
               pcs::prec++; cmd=""; continue;
            }
            else if (cmd.substr(0,5)=="post:" || contpost)
            {
               pcs::post[pcs::postc]=cmd.substr(5*(!contpost));
               printf("Accepted additional post-resource command line \"%s\"\n",pcs::post[pcs::postc].c_str());
               string* n=&(pcs::post[pcs::postc]); contpost=0;
               if ((*n)[n->length()-1]=='\\') {  n->erase(n->length()-1,1); if ((*n)[n->length()-2]!='\\') contpost=1; }
               pcs::postc++; cmd=""; continue;
            }
            cmd="";
         }
      }
   }
   printf("\n");
   fflush(stdout);
}

/*
void execute_pcs_pre_commands(string exename="")
{
  int sr;
  for (int i=0;i<pcs::prec;i++)
    if ((sr = system(string_replace_all(pcs::pre[i],"%exename%",exename).c_str())) != 0)
      printf("Pre-Compile command %d returned %d exit status\r\n",i,sr);
}

void execute_pcs_post_commands(string exename="")
{
  int sr;
  for (int i=0;i<pcs::postc;i++)
    if ((sr = system(string_replace_all(pcs::post[i],"%exename%",exename).c_str())) != 0)
      printf("Pre-Compile command %d returned %d exit status\r\n",i,sr);
}*/
