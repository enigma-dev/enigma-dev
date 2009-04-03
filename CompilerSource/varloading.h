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


int localc=0;
map <int,std::string> localn,localt;


void load_locals()
{
    string localvars;
    
    FILE* readinherits=fopen("locals.txt","rb");
    if (readinherits==NULL)
    {
    localvars=
"var alarm;\r\nvar bbox_bottom;\r\nvar bbox_left;\r\nvar bbox_right;\r\nvar bbox_top;\r\n\
var depth;\r\nenigma::directionv direction;\r\nvar friction;\r\nvar gravity;\r\n\
var gravity_direction;\r\nenigma::hspeedv hspeed;\r\nvar image_alpha;\r\nvar image_angle;\r\n\
var image_blend;\r\nvar image_index;\r\nvar image_number;\r\nvar image_single;\r\n\
var image_speed;\r\nvar image_xscale;\r\nvar image_yscale;\r\nvar mask_index;\r\n\
var path_endaction;\r\nvar path_index;\r\nvar path_orientation;\r\nvar path_position;\r\n\
var path_positionprevious;\r\nvar path_scale;\r\nvar path_speed;\r\nvar persistent;\r\n\
var solid;\r\nenigma::speedv speed;\r\nvar sprite_height;\r\nvar sprite_index;\r\n\
var sprite_width;\r\nvar sprite_xoffset;\r\nvar sprite_yoffset;\r\nvar timeline_index;\r\n\
var timeline_position;\r\nvar timeline_speed;\r\nvar visible;\r\nenigma::vspeedv vspeed;\r\n\
var x;\r\nvar xprevious;\r\nvar xstart;\r\nvar y;\r\nvar yprevious;\r\nvar ystart;\r\n\
double id, object_index;";
    
    #if DEBUGMODE
      printf("Locals file not located. Improvised.\n"); fflush(stdout);
    #endif
    }
    else
    {
      int sz; fseek(readinherits,0,SEEK_END);
      sz=ftell(readinherits); fseek(readinherits,0,SEEK_SET);
      
      #if DEBUGMODE
        printf("Reading %d bytes of locals...\n",sz);fflush(stdout);
      #endif
      
      char* rdths=new char[sz+1];
      #if DEBUGMODE
      int szr=fread(rdths,1,sz,readinherits);
      #else
      fread(rdths,1,sz,readinherits);
      #endif
      rdths[sz]=0;
      
      localvars=rdths;
      
      free(rdths);
      fclose(readinherits);
      
      #if DEBUGMODE
        printf("Read %d bytes of locals\n",szr); fflush(stdout);
      #endif
    }
    
    string typeinq; int lastmark=-1,lastemark=-1;
    for (unsigned int i=0; i<localvars.length();)
    {
      while (localvars[i]==' ' || localvars[i]=='\r' || localvars[i]=='\n' || localvars[i]=='\t') { i++; }
      
      if ((localvars[i]>='A' && localvars[i]<='Z') 
      || (localvars[i]>='a' && localvars[i]<='z') 
      || (localvars[i]=='_') || (localvars[i]=='*') 
      || (localvars[i]=='[') || (localvars[i]==']') 
      || (localvars[i]>='0' && localvars[i]<='9'))
      {
         if (lastmark!=-1)
          {
            typeinq+=" ";
            typeinq+=localvars.substr(lastmark,lastemark-lastmark);
            lastmark=-1; lastemark=-1;
          }
         
         lastmark=i;
         while ((localvars[i]>='A' && localvars[i]<='Z') 
         || (localvars[i]>='a' && localvars[i]<='z') 
         || (localvars[i]=='_') || (localvars[i]=='*') 
         || (localvars[i]=='[') || (localvars[i]==']') 
         || (localvars[i]>='0' && localvars[i]<='9'))
         i++; while (localvars[i]==' ' || localvars[i]=='\r' || localvars[i]=='\n' || localvars[i]=='\t') i++;
         while (localvars[i]==':')
         {
           i++; while ((localvars[i]>='A' && localvars[i]<='Z') 
           || (localvars[i]>='a' && localvars[i]<='z') 
           || (localvars[i]=='_') || (localvars[i]=='*') 
           || (localvars[i]=='[') || (localvars[i]==']') 
           || (localvars[i]>='0' && localvars[i]<='9'))
           i++; while (localvars[i]==' ' || localvars[i]=='\r' || localvars[i]=='\n' || localvars[i]=='\t') i++;
         }
         lastemark=i;
         continue;
      }
         
      
      
      if (localvars[i]=='/')
      {
        if (localvars[i+1]=='/')
        {
           while (i<localvars.length() && localvars[i]!='\r' && localvars[i]!='\n') i++;
        }
        else if (localvars[i+1]=='*')
        {
           i++; while (i<localvars.length() && !(localvars[i]=='*' && localvars[i]=='/')) i++;
           i+=2;
        }
        continue;
      }
      
      
      
      if ((localvars[i]==',')||(localvars[i]==';'))
      {
          if (lastmark!=-1)
          {
            localt[localc]=typeinq;
            localn[localc]=localvars.substr(lastmark,lastemark-lastmark);
            localc++; lastmark=-1; lastemark=-1;
            if (localvars[i]==';') typeinq="";
          }
        i++; continue;
      }
      else
      {
          if (lastmark!=-1)
          {
            typeinq+=" ";
            typeinq+=localvars.substr(lastmark,lastemark-lastmark);
            lastmark=-1; lastemark=-1;
          }
        i++; continue;
      }
      printf("ERROR in local variable data\n\n"); break;
    }
    #if DEBUGMODE
      printf("Read locals successfully.\n");
    #endif
}
