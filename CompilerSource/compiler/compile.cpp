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

#include "../OS_Switchboard.h"

#include <stdio.h>
#include <string>
#include <map>

using namespace std;

#include "../general/estring.h"
#include "../general/darray.h"

#include "../varnames/varnames.h"
#include "../syntax/syncheck.h"
#include "../parser/parser.h"
#include "pcs/pcs.h"

#include "license_string.h"
#include "compile_includes.h"


/* Call is CompileEMF(char* filename)
 * Result is zero on success or nonzero otherwise.
 *
 * Error codes are as follows:
 * ---------------------------
 *       1  | Syntax error
 *       0  | No error
 *      -1  | File not found; invalid format; invalid version
 *      -2  | Icon error
 *      -3  | Null terminator expected after STR.
 *      -4  | Indicator not found...
 *      -5  | Expected "inst" or "\0\0\0\0", found neither
 *      -6  | GCC failed to create an o file
 *      -7  | GCC failed to compile the exe
 *      -8  | Failed to open file
 *      -9  | Memory allocation failure
 *      -10 | WTFH MAN?
 *      -11 | Undefined script mischecked
 *      -12 | Variable type mismatch
 *
 *
 *
 *
 */




/*
 These are to manage data in a somewhat organized fashion.
 */

   struct scriptdata
   {
       string  name;
       string  code;
       varray<string> varnames;
       varray<string> varname_prefix;
       varray<string> varname_types;
       varray<string> varname_suffix;
       varray<string> fnames;
       int varcount,fcount,addind;
   };

   struct object
   {
       int id;
       int sprite,mask,parent,visible,solid,persistent;
       int depth;
       //int scrcount;
       string name;
       int eventcount;
       //varray<string> scriptnames;
       map<int,scriptdata> events;
   };

   struct instance //For in rooms.
   {
       int id;
       int obj;
       int x;
       int y;
       string code;
   };

   struct room
   {
       int id;
       string name;
       int width,height;
       int instance_count;
       string createcode;
       string caption; int speed;
       map<int,instance> instances;

       int backcolor;
       char start_vis[8],views_enabled;
       int area_x[8],area_y[8],area_w[8],area_h[8];
       int port_x[8],port_y[8],port_w[8],port_h[8];
       int object2follow[8];
       int hborder[8],vborder[8],hspd[8],vspd[8];
   };

   struct nameid
   {
       int id;
       string name;
   };






/* End ugly simplimfying code.
|* This function puts it all together. This is the one the interface calls.
\****************************************************************************/

double CompileEGMf(string filename,string outname,int debug=0,int build=0,int fullcomp=0)
{
  string     str_read;
  int             int_read;
  char*           ptr_read;

  string changefile="";
  if (build)
  {
    changefile=outname;
    printf("Changed the log file to \"%s\"\n",changefile.c_str());
    outname=filename;
    for (int i=outname.length(); i>0; i--)
    { if (outname[i]==DIRECTORY_CHAR || outname[i]=='/') { outname.erase(i); break; } }
    outname+="buildgame.exe";

    for (unsigned int i=0; i<changefile.length(); i++)
    if (changefile[i]==DIRECTORY_CHAR || changefile[i]=='/') { changefile.insert(i,DIRECTORY_STR); i++; }
  }

  const string GLOBALscriptargs="(enigma::variant argument0, enigma::variant argument1, enigma::variant argument2, enigma::variant argument3, enigma::variant argument4, enigma::variant argument5, enigma::variant argument6, enigma::variant argument7, enigma::variant argument8, enigma::variant argument9, enigma::variant argument10, enigma::variant argument11, enigma::variant argument12, enigma::variant argument13, enigma::variant argument14, enigma::variant argument15)";
  const string GLOBALscriptargsinit="(enigma::variant argument0=0, enigma::variant argument1=0, enigma::variant argument2=0, enigma::variant argument3=0, enigma::variant argument4=0, enigma::variant argument5=0, enigma::variant argument6=0, enigma::variant argument7=0, enigma::variant argument8=0, enigma::variant argument9=0, enigma::variant argument10=0, enigma::variant argument11=0, enigma::variant argument12=0, enigma::variant argument13=0, enigma::variant argument14=0, enigma::variant argument15=0);";


  writehere = NULL;
  enigma_file=fopen(filename.c_str(),"rb");
  if (enigma_file==NULL) { printf("Failed to open file \"%s\"\n",filename.c_str()); fflush(stdout); return -8; }

  /* Open and check the file.
  ****************************/

      /* Did it open right? */
      if (enigma_file == NULL)
        { printf("File \"%s\" could not be opened\n",filename.c_str()); fflush(stdout); return -1; }

     printf("Opened file.\n");
     fflush(stdout);

      /* Check the magic number at the beginning. */
      if (reads(4) != "EGMf")
        { printf("File \"%s\" invalid\n",filename.c_str()); fflush(stdout); return -1; }

      /* Verify the version number. */
      if (readi() != 4)
        { printf("File \"%s\" is of the wrong version.\n",filename.c_str()); fflush(stdout); return -1; }

      printf("File is valid.\n"); fflush(stdout);

  /* Export the icon from the file.
  **********************************/

      /* Read the size */
      int_read=readi();

      /* Read the icon. It should be exactly the size we just read. */
      ptr_read=readv(int_read);

      /* Checkpoint: Null integer */
      if (readi() != 0)
        return -2;

      printf("Read icon data.\n");
      fflush(stdout);

      /* Dump the icon into its own file */
      if (fileout((char*)"game_icon.ico",(char*)ptr_read,int_read)==-8) return -8;

  printf("Exported icon.\n");
  fflush(stdout);

  /* Read the list of script names.
  ***********************************/

      int script_count=readi();
      scriptdata* scripts=new scriptdata[script_count];

      for (int i=0;i<script_count;i++)
      {
        scripts[i].name=readSTR();
        syncheck::addscr(scripts[i].name);
      }

      if (EXPECTNULL) return -3;

  printf("Read script names.\n");
  fflush(stdout);

  /* Read the list of object names and ids.
  *******************************************/

      int object_count=readi();
      object* objects=new object[object_count];

      for (int i=0;i<object_count;i++)
      {
          objects[i].id=readi();
          objects[i].name=readSTR();
          //objects[i].scrcount=0;
      }

      if (EXPECTNULL) return -3;

  printf("Read object names.\n");
  fflush(stdout);


  /* Read the list of room names and ids.
  *****************************************/

      int room_count=readi();
      room* rooms=new room[room_count];

      for (int i=0;i<room_count;i++)
      {
          rooms[i].id=readi();
          rooms[i].name=readSTR();
      }

      if (EXPECTNULL) return -3;


  printf("Read room names.\n");
  fflush(stdout);


  /* Read the list of sprite names and ids.
  *******************************************/

      int sprite_count = readi();
      nameid* sprites = new nameid[sprite_count];

      for (int i=0;i<sprite_count;i++)
      {
          sprites[i].id=readi();
          sprites[i].name=readSTR();
      }

      if (EXPECTNULL) return -3;

  printf("Read sprite names.\n");
  fflush(stdout);





  /* Export the resource names to a file of their definitions.
  **************************************************************/

      FILE* resources;
      resources=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h","wb");
      if (resources==NULL) { printf("Failed to open critical source file\n"); fflush(stdout);  return -8; }

      writes(resources,"//This file was generated by the ENIGMA Development Environment.\r\n\r\n");

      writes(resources,"//Object names\r\n");
      for (int i=0;i<object_count;i++)
        writeDefine(resources,objects[i].name,objects[i].id);

      writes(resources,"\r\n//Sprite names\r\n");
      for (int i=0;i<sprite_count;i++)
        writeDefine(resources,sprites[i].name,sprites[i].id);

      writes(resources,"\r\n//Room names\r\n");
      for (int i=0;i<room_count;i++)
        writeDefine(resources,rooms[i].name,rooms[i].id);

      fclose(resources);

  printf("Wrote resource names.\n");
  fflush(stdout);


  /* Export the objects in switch format
  ****************************************/

      FILE* objectswitch;
      objectswitch=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h","wb");
      if (objectswitch==NULL) { printf("Failed to open critical source file.\nOn a side note, YOU SHOULD NOT BE SEEING THIS ERROR.\n"); fflush(stdout); return -8; }

      for (int i=0;i<object_count;i++)
      {
        writes(objectswitch,"case "+tostring(objects[i].id)+": \r\n new enigma::OBJ_"+objects[i].name+"; \r\n break; \r\n");
      }

      fclose(objectswitch);

  printf("Exported object switch.\n");
  fflush(stdout);





  /* Read and parse all scripts.
  **************************************************************/

      if (reads(4) != "scr\0")
      return -4;

      for(int i=0;i<script_count;i++)
      {
         string code=readSTR();
         if (EXPECTNULL) return -3;

         int synt=syncheck::syntacheck(code);
         if (!(synt<0))
         {
           printf("\nERROR: Compile error. Invalid syntax.\n\n");
           printf("In script `%s',\n",scripts[i].name.c_str());

           int l=0,p=0; int i;
           for (i=0;i<synt;i++) { if (code[i]=='\r') { l++; if (code[i+1]=='\n') i++; p=i+1; } else if (code[i]=='\n') { l++; p=i+1; } }
           printf("Syntax check returned error on line %d, position %d; or absolute index %d:\n",(l)+1,(synt-p)+1,synt);

           printf("%s\n\n",syncheck::error.c_str());
           printf("Compile can not continue, aborting.\n");
           fflush(stdout);
           return 1;
         }

         scripts[i].code="{\r\n"+parser_main(code)+" \n  return 0;\n}";

         for(int ii=0;ii<varcount;ii++)
         {
           scripts[i].varnames[ii]=varnames[ii];
           scripts[i].varname_prefix[ii]=varname_prefix[ii];
           scripts[i].varname_types[ii]=varname_types[ii];
           scripts[i].varname_suffix[ii]=varname_suffix[ii];
         }
         for(int ii=0;ii<fcount;ii++)
         scripts[i].fnames[ii]=fnames[ii];

         scripts[i].varcount=varcount;
         scripts[i].fcount=fcount;
      }

  printf("Loaded and parsed all scripts.\n");
  fflush(stdout);


  /* Thrash through the scripts to retrieve ALL varnames.
  **************************************************************/

      /*for (int cmxvii=0;cmxvii<(int)(script_count/2.0+.5);cmxvii++)
      {
          for (int i=0;i<script_count;i++)
          {
              printf("%s- ",scripts[i].name.c_str());
              for (int ii=0;ii<scripts[i].fcount;ii++)
              printf("%s,",scripts[i].fnames[ii].c_str());
              printf("\r\n");
          }*/
          //For each script
          for (int superscript=0;superscript<script_count;superscript++)
          {
             //For each script on each script's list
             for (int i=0;i<scripts[superscript].fcount;i++) for (int subscript=0;subscript<script_count;subscript++)  if (scripts[subscript].name==scripts[superscript].fnames[i])
             {
                //Go through the list of scripts script I's subscripts call, and add them
                for (int subsubscr=0;subsubscr<scripts[subscript].fcount;subsubscr++)
                {
                   int onl=0;

                   //check if the subscript's subscript is on the original script's list already.
                   for (int subsubscr2=0;subsubscr2<scripts[superscript].fcount;subsubscr2++)
                   if (scripts[superscript].fnames[subsubscr2]==scripts[subscript].fnames[subsubscr])
                   { onl=1; break; }

                   if (!onl)
                   {
                     //printf("Link'd %s to %s\n",scripts[subscript].fnames[subsubscr].c_str(),scripts[superscript].name.c_str());
                     scripts[superscript].fnames[scripts[superscript].fcount] = scripts[subscript].fnames[subsubscr];
                     scripts[superscript].fcount++;
                   }
                }
                break;
             }
          }
      //}
      printf("\"Link'd\" all scripts.\n"); fflush(stdout);
      for (int i=0;i<script_count;i++)
      {
         for (int ii=0;ii<scripts[i].fcount;ii++)
         {
            for (int iii=0;iii<script_count;iii++)
            {
               if (scripts[iii].name==scripts[i].fnames[ii])
               {
                  for (int iv=0;iv<scripts[iii].varcount; iv++)
                  {
                     int onl=0;
                     for (int v=0;v<scripts[i].varcount;v++)
                     if (scripts[i].varnames[v]==scripts[iii].varnames[iv])
                     { onl=1; break; }

                     if (!onl)
                     {
                       scripts[i].varnames[scripts[i].varcount]=scripts[iii].varnames[iv];
                       scripts[i].varcount++;
                     }
                  }
               }
            }
         }
      }


  printf("Retrieved all script variables.\n");
  fflush(stdout);



  /* Read and parse all objects.
  **************************************************************/

      if (reads(4) != "obj\0")
      return -4;


      //for each object
      for(int i=0;i<object_count;i++)
      {
         objects[i].sprite=readi();
         objects[i].solid=readi();
         objects[i].visible=readi();
         objects[i].depth=readi();
         objects[i].persistent=readi();

         objects[i].parent=readi(); //-1 = no parent
         objects[i].mask=readi();   //-1 = same as sprite

         int namelen=readi();

         objects[i].eventcount=0;
         //for each event
         for (int ii=0;namelen!=0;ii++)
         {
            //Read the name
            string n = reads(namelen);
            //Store the name
            objects[i].events[ii].name = n;

            //Read the null
            int_read=readi();
            if (int_read != 0) return -5;

            //Read the additional index
            objects[i].events[ii].addind=readi();

            //Read the code
            string code=readSTR();
            //Was there a null int?
            if (EXPECTNULL) return -3;

            //Check it
            int synt=syncheck::syntacheck(code);
            if (!(synt<0))
            {
              printf("\nERROR: Compile error. Invalid syntax.\n\n");
              printf("In icon %d of event `%s' for object `%s',\n",0,objects[i].events[ii].name.c_str(),objects[i].name.c_str());

              int l=0,p=0; int i;
              for (i=0;i<synt;i++) { if (code[i]=='\r') { l++; if (code[i+1]=='\n') i++; p=i+1; } else if (code[i]=='\n') { l++; p=i+1; } }
              printf("Syntax check returned error on line %d, position %d; or absolute index %d:\n",(l)+1,(synt-p)+1,synt);

              printf("%s\n\n",syncheck::error.c_str());
              printf("Compile can not continue, aborting.\n");
              fflush(stdout);
              return 1;
            }
            //Parse it
            code=parser_main(code)+"  \r\n  return 0;\r\n";

            //Put this all in the object
            if (objects[i].events[ii].name=="step")
            {
              objects[i].eventcount++;
              objects[i].events[ii].code="enigma::step_basic(this);\r\n  \r\n  "+code;
            }
            else
            {
              objects[i].eventcount++;
              objects[i].events[ii].code=code;
            }

            for(int iii=0;iii<varcount;iii++)
            {
              objects[i].events[ii].varnames[iii]=varnames[iii];
              objects[i].events[ii].varname_prefix[iii]=varname_prefix[iii];
              objects[i].events[ii].varname_types[iii]=varname_types[iii];
              objects[i].events[ii].varname_suffix[iii]=varname_suffix[iii];
            } objects[i].events[ii].varcount=varcount;

            //For each script
            for(int iii=0;iii<fcount;iii++)
            {
              int scrfind=-1;
              for (int iv=0;iv<script_count;iv++) if (scripts[iv].name==fnames[iii]) { scrfind=iv; break; }
              if (scrfind==-1)
              {
                printf(
"***********************************************************************************************\n\
Double error! Syntax check mischecked undefined script `%s', now compiler is confused. Aborting.\n\
************************************************************************************************\n",
                fnames[iii].c_str()); fflush(stdout);
                return -11;
              }
              else //if the script exists
              {
                //printf("%d locals in script\n",scripts[scrfind].varcount);
                //Add the script to the object if ithe object doesn't already have it
                //Deprecated as of WILDCLASS rethink, release 4
                /*bool scrinobj1; scrinobj1=0;
                for (int iv=0;iv<objects[i].scrcount;iv++)
                  if (objects[i].scriptnames[iv]==fnames[iii]) { scrinobj1=1; break; }
                if (scrinobj1 != -1)
                {
                  objects[i].scriptnames[objects[i].scrcount]=fnames[iii];
                  objects[i].scrcount++;
                  //add the script's variables, too
                  for (int iv=0;iv<scripts[scrfind].varcount;iv++)
                  {
                    int tvc=objects[i].events[ii].varcount;
                    int alrin=0; for (int v=0;v<tvc;v++) if (objects[i].events[ii].varnames[v]==scripts[scrfind].varnames[iv]) { alrin=1; break; }
                    if (!alrin)
                    {
                      objects[i].events[ii].varnames[tvc]=scripts[scrfind].varnames[iv];
                      objects[i].events[ii].varname_prefix[tvc]=scripts[scrfind].varname_prefix[iv];
                      objects[i].events[ii].varname_types[tvc]=scripts[scrfind].varname_types[iv];
                      objects[i].events[ii].varname_suffix[tvc]=scripts[scrfind].varname_suffix[iv];
                      objects[i].events[ii].varcount++;
                    }
                  }
                  //Add the script's subscripts to the object if they don't exist
                  //printf("Adding %d underlying script calls to object...\n",scripts[scrfind].fcount);
                  for (int iv=0;iv<scripts[scrfind].fcount;iv++)
                  {
                      int scrinobj2; scrinobj2=0;
                      for (int v=0;v<objects[i].scrcount;v++)
                        if (objects[i].scriptnames[v]==scripts[scrfind].fnames[iv])
                        { scrinobj2=1; break; }
                      if (!scrinobj2)
                      {
                        //printf("    Added script `%s' to object %d\n",scripts[scrfind].fnames[iv].c_str(),i);
                        objects[i].scriptnames[objects[i].scrcount]=scripts[scrfind].fnames[iv];
                        objects[i].scrcount++;
                      }
                    }
                  }
                  */
               }
             }


             //Read new name size
             namelen=readi();
         }
      }


  printf("Loaded and parsed all events.\n");
  fflush(stdout);


  /* Read rooms
  *************************************************************/

      if (reads(4) != "rmm\0")
        return -4;

      for (int i=0;i<room_count;i++)
      {
          rooms[i].caption=readSTR();
          rooms[i].width=readi();
          rooms[i].height=readi();
          rooms[i].speed=readi();
          int col=readi();
          if (((col&0xFF000000)>>6)>0) { rooms[i].backcolor=((col&0xFF)<<16) + (col&0xFF00) + ((col&0xFF0000)>>16); }
          else rooms[i].backcolor=-1;


          //read the create code
          str_read=readSTR();
          if (EXPECTNULL) return -3;

          // Syntax check and parse the code

              int synt=syncheck::syntacheck(str_read);
              if (!(synt<0))
              {
                 printf("\nERROR: Compile error. Invalid syntax.\n\n");
                 printf("In room creation code for room `%s',\n",rooms[i].name.c_str());

                 int l=0,p=0; int i;
                 for (i=0;i<synt;i++) { if (str_read[i]=='\r') { l++; if (str_read[i+1]=='\n') i++; p=i+1; } else if (str_read[i]=='\n') { l++; p=i+1; } }
                 printf("Syntax check returned error on line %d, position %d; or absolute index %d:\n",(l)+1,(synt-p)+1,synt);

                 printf("%s\n",syncheck::error.c_str());
                 printf("Compile can not continue, aborting.\n");
                 fflush(stdout);
                 return 1;
              }
              if (str_read!="")
              rooms[i].createcode=parser_main(str_read)+"  \r\n  return 0;\r\n";

              string vardecs;
              for (int ii=0;ii<varcount;ii++)
              {
                if (!varname_onlists(varnames[ii]))
                {
                  if (varname_types[ii]=="") vardecs+="var "+varnames[ii]+";\n";
                  else                     vardecs+=varname_types[ii]+" "+varnames[ii]+";\n";
                }
              }
              varcount=0;

              rooms[i].createcode=vardecs+rooms[i].createcode;

          //End syntax check and parse

          rooms[i].views_enabled=readi();

          for (int ii=0;ii<8;ii++)
          {
              rooms[i].start_vis[ii]=readi();
              rooms[i].area_x[ii]=readi(); rooms[i].area_y[ii]=readi(); rooms[i].area_w[ii]=readi(); rooms[i].area_h[ii]=readi();
              rooms[i].port_x[ii]=readi(); rooms[i].port_y[ii]=readi(); rooms[i].port_w[ii]=readi(); rooms[i].port_h[ii]=readi();
              rooms[i].hborder[ii]=readi(); rooms[i].vborder[ii]=readi(); rooms[i].hspd[ii]=readi(); rooms[i].vspd[ii]=readi();
              rooms[i].object2follow[ii]=readi();
          }

          rooms[i].instance_count=0;
          for (int ii=0;reads(4)=="inst";ii++)
          {
                rooms[i].instance_count++;
                rooms[i].instances[ii].id=readi();
                rooms[i].instances[ii].obj=readi();
                rooms[i].instances[ii].x=readi();
                rooms[i].instances[ii].y=readi();

                str_read=readSTR();
                int synt=syncheck::syntacheck(str_read);
                if (!(synt<0))
                {
                   printf("\nERROR: Compile error. Invalid syntax.\n\n");
                   printf("In room creation code for instance %d of object `%s' in room `%s'\n",rooms[i].instances[ii].id,objects[rooms[i].instances[ii].obj].name.c_str(),rooms[i].name.c_str());

                   int l=0,p=0; int i;
                   for (i=0;i<synt;i++) { if (str_read[i]=='\r') { l++; if (str_read[i+1]=='\n') i++; p=i+1; } else if (str_read[i]=='\n') { l++; p=i+1; } }
                   printf("Syntax check returned error on line %d, position %d; or absolute index %d:\n",(l)+1,(synt-p)+1,synt);

                   printf("%s\n\n",syncheck::error.c_str());
                   printf("Compile can not continue, aborting.\n");
                   fflush(stdout);

                   return 1;
                }
                if (str_read!="")
                rooms[i].instances[ii].code=parser_main("with ("+tostring(rooms[i].instances[ii].id)+") { "+str_read+" }");

                //TODO: PLACE LOOP FOR LIST OF LOCALS HERE AND VERIFY THAT OBJECTS[rooms[i].instances[ii].obj] HAS ALL THE LOCALS.
                //IF NOT, DECLARE THEM AT THE TOP OF THE SCRIPT. </TODO>
          }

          fseek(enigma_file,-4,SEEK_CUR);
          if (readi() != 0)
          return -5;
      }


      printf("Read room data.\n");
      fflush(stdout);


  /* Read the parent section, codenamed myn, though I forget how or why.
  ************************************************************************/

      if (reads(4) != "myn\0")
        return -4;

      string parent_bstep=readSTR();
      string parent_estep=readSTR();

    //Ism never actually writes anything here though,
    //so don't bother doing anything with it. </honest comment + laziness>


  /* Export the objects and scripts, and everything else.
  **************************************************************/

      /********************************************************************
      * Start by writing down the declarations of all the objects' events.
      *********************************************************************/
      FILE *IDE_EDIT_objectdeclarations,*IDE_EDIT_objectfunctionality;
      IDE_EDIT_objectdeclarations=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h","wb");
      IDE_EDIT_objectfunctionality=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h","wb");
      if (IDE_EDIT_objectdeclarations==NULL) return -8;
      if (IDE_EDIT_objectfunctionality==NULL) return -8;

      writes(IDE_EDIT_objectdeclarations,"//This file was generated by the ENIGMA Development Environment.\r\n\r\n");
      writes(IDE_EDIT_objectfunctionality,"//This file was generated by the ENIGMA Development Environment.\r\n\r\n");

      int didcreate,diddestroy,didstep;
      string evname,vname,vstars,vtags,varrays;
      for (int i=0;i<object_count;i++)
      {
         //start by writing the class name and then include the two sets of shared vaariables.
         writes(IDE_EDIT_objectdeclarations,
         "struct OBJ_"+objects[i].name+":public object \r\n{\r\n");

         //No create or destroy event yet
         didcreate=0; diddestroy=0; didstep=0;

         int write_count=0;
         varray<string> to_write,to_write_stars,to_write_tags,to_write_arrays;
         for (int ii=0;ii<objects[i].eventcount;ii++)
         {
             for (int iii=0;iii<objects[i].events[ii].varcount;iii++)
             {
                 vname=objects[i].events[ii].varnames[iii];
                 vstars=objects[i].events[ii].varname_prefix[iii];
                 vtags=objects[i].events[ii].varname_types[iii];
                 varrays=objects[i].events[ii].varname_suffix[iii];
                 if (!varname_onlists(vname))
                 {
                   int onit=0;

                   for (int iv=0;iv<write_count;iv++)
                   if (to_write[iv]==vname)
                   {
                     //make sure the tags match, or one is ""
                     if (to_write_tags[iv]!=vtags && to_write_tags[iv] != "" && vtags != "")
                     {
                       printf("Mechanics Error: Redeclaration of local variable `%s' in event `%s' for object `%s' as a different type. Compile can not continue.\n",vname.c_str(),objects[i].events[ii].name.c_str(),objects[i].name.c_str());
                       fflush(stdout); return -12;
                     }
                     if (to_write_tags[iv]!="" && vtags!="")
                     {
                       if (to_write_tags[iv] != vstars || string_count('[',(char*)to_write_arrays[iv].c_str()) != string_count('[',(char*)varrays.c_str()))
                       {
                         printf("Mechanics Warning: Redeclaration of local variable `%s' in event `%s' for object `%s' as different pointer-to types.\n",vname.c_str(),objects[i].events[ii].name.c_str(),objects[i].name.c_str());
                         fflush(stdout);
                       }

                     }

                     if (to_write_tags[iv] == "" && vtags != "")
                     {
                       to_write_tags[iv] = vtags;
                       to_write_stars[iv] = vstars;
                       to_write_arrays[iv] = varrays;
                     }


                     onit=1;
                   }

                   for (int iv=0;iv<sprite_count;iv++) if (sprites[iv].name==vname) onit=1;
                   for (int iv=0;iv<object_count;iv++) if (objects[iv].name==vname) onit=1;
                   for (int iv=0;iv<  room_count;iv++) if (  rooms[iv].name==vname) onit=1;

                   if (!onit)
                   {
                     to_write[write_count]=vname;
                     to_write_stars[write_count]=vstars;
                     to_write_tags[write_count]=vtags;
                     to_write_arrays[write_count]=varrays;
                     write_count++;
                   }
                 }
             }
         }

         //We now have a condensed list of varnames and their matching tags.
         //Now we must export them to the file for great justice.
         for (int ii=0;ii<write_count;ii++)
         {
             if (to_write_tags[ii]=="")
             writes(IDE_EDIT_objectdeclarations," var "+to_write[ii]+"; \r\n");
             else
             fprintf(IDE_EDIT_objectdeclarations,"  %s%s%s%s;\r\n",to_write_tags[ii].c_str(),to_write_stars[ii].c_str(),to_write[ii].c_str(),to_write_arrays[ii].c_str());
         }


         //Write the scripts the object will use
         //Deprecated as of WILDCLASS rethink, release 4
         /*
         fprintf(IDE_EDIT_objectdeclarations," //Scripts (%d)\r\n",objects[i].scrcount);
         for (int ii=0;ii<objects[i].scrcount;ii++)
          writes(IDE_EDIT_objectdeclarations," enigma::variant "+objects[i].scriptnames[ii]+GLOBALscriptargsinit+"; \r\n");
         fprintf(IDE_EDIT_objectdeclarations," \r\n");
         */

         string evname=""; string mouselist="", kbdlist="", krlist="",kplist="", almlist="";
         for (int ii=0;ii<objects[i].eventcount;ii++)
         {
            evname=objects[i].events[ii].name;
            int addind=objects[i].events[ii].addind;

            if (evname == "create") didcreate=1;
            if (evname == "destroy") diddestroy=1;
            if (evname == "step") didstep=1;

            if (objects[i].events[ii].name=="keyboard")
            {
               if (addind>1 && addind<256)
               kbdlist+="  if (enigma::keybdstatus["+tostring(objects[i].events[ii].addind)+"] == 1) ";
               else kbdlist+="  if (keyboard_check_pressed("+tostring(addind)+") == 1) ";

               kbdlist+="  myevent_"+evname+tostring(addind)+"();\r\n\r\n";
               writes(IDE_EDIT_objectdeclarations,"enigma::variant myevent_"+evname+tostring(addind)+"();\r\n\r\n");
               continue;
            }
            if (objects[i].events[ii].name=="keypress")
            {
               if (addind>1 && addind<256)
               kplist+="  if (enigma::keybdstatus["+tostring(objects[i].events[ii].addind)+"]==1 && enigma::last_keybdstatus["+tostring(objects[i].events[ii].addind)+"]==0) ";
               else kplist+="  if (keyboard_check_pressed("+tostring(addind)+") == 1) ";

               kplist+="  myevent_"+evname+tostring(addind)+"();\r\n\r\n";
               writes(IDE_EDIT_objectdeclarations,"enigma::variant myevent_"+evname+tostring(addind)+"();\r\n\r\n");
               continue;
            }
            if (objects[i].events[ii].name=="keyrelease")
            {
               if (addind>1 && addind<256)
               krlist+="  if (enigma::keybdstatus["+tostring(objects[i].events[ii].addind)+"]==0 && enigma::last_keybdstatus["+tostring(objects[i].events[ii].addind)+"]==1) ";
               else krlist+="  if (keyboard_check_pressed("+tostring(addind)+") == 1) ";

               krlist+="  myevent_"+evname+tostring(addind)+"();\r\n\r\n";
               writes(IDE_EDIT_objectdeclarations,"enigma::variant myevent_"+evname+tostring(addind)+"();\r\n\r\n");
               continue;
            }

            if (objects[i].events[ii].name=="mouse")
            {
               int didit=1;
               switch (addind)
               {
                 case 1: case 2: case 3:
                   mouselist+="  if (enigma::mousestatus["+tostring(addind-1)+"] == 1) "; break;
                 case 4: case 5: case 6:
                   mouselist+="  if (enigma::mousestatus["+tostring(addind-4)+"] == 1 && enigma::last_mousestatus["+tostring(addind-4)+"]== 0) "; break;
                 case 7: case 8: case 9:
                   mouselist+="  if (enigma::mousestatus["+tostring(addind-7)+"] == 0 && enigma::last_mousestatus["+tostring(addind-7)+"]== 1) "; break;

                 case 50: case 51: case 52:
                   mouselist+="  if (enigma::mousestatus["+tostring(addind-50)+"] == 1) "; break;
                 case 53: case 54: case 55:
                   mouselist+="  if (enigma::mousestatus["+tostring(addind-53)+"] == 1 && enigma::last_mousestatus["+tostring(addind-53)+"] == 0) "; break;
                 case 56: case 57: case 58:
                   mouselist+="  if (enigma::mousestatus["+tostring(addind-56)+"] == 0 && enigma::last_mousestatus["+tostring(addind-56)+"] == 1) "; break;

                 case 60:
                   mouselist+="  for (int enigmaACALLBACKITERATOR=enigma::mousewheel;enigmaACALLBACKITERATOR>0;enigmaACALLBACKITERATOR-=120) "; break;
                 case 61:
                   mouselist+="  for (int enigmaACALLBACKITERATOR=(-enigma::mousewheel);enigmaACALLBACKITERATOR>0;enigmaACALLBACKITERATOR-=120) "; break;

                 default:
                   didit=0;
               }
               if (didit) mouselist+=" myevent_"+evname+tostring(addind)+"();\r\n\r\n";
               writes(IDE_EDIT_objectdeclarations,"enigma::variant myevent_"+evname+tostring(addind)+"();\r\n\r\n");
               continue;
            }

/*  LEFT BUTTON = 0;  RIGHT_BUTTON = 1;  MIDDLE BUTTON = 2;  NO BUTTON = 3;  LEFT PRESS = 4;
RIGHT PRESS = 5;  MIDDLE PRESS = 6;  LEFT RELEASE = 7;  RIGHT RELEASE = 8;  MIDDLE RELEASE = 9;
MOUSE ENTER = 10;  MOUSE LEAVE = 11;  MOUSE WHEEL UP = 60;  MOUSE WHEEL DOWN = 61;
GLOBAL LEFT BUTTON = 50;  GLOBAL RIGHT BUTTON = 51;  GLOBAL MIDDLE BUTTON = 52;
GLOBAL LEFT PRESS = 53;  GLOBAL RIGHT PRESS = 54;  GLOBAL MIDDLE PRESS = 55;
GLOBAL LEFT RELEASE = 56;  GLOBAL RIGHT RELEASE = 57;  GLOBAL MIDDLE RELEASE = 58;
JOYSTICK1 LEFT = 16;  JOYSTICK1 RIGHT = 17;  JOYSTICK1 UP = 18;  JOYSTICK1 DOWN = 19;
JOYSTICK1 BUTTON1 = 21;  JOYSTICK1 BUTTON2 = 22;  JOYSTICK1 BUTTON3 = 23;  JOYSTICK1 BUTTON4 = 24;
JOYSTICK1 BUTTON5 = 25;  JOYSTICK1 BUTTON6 = 26;  JOYSTICK1 BUTTON7 = 27;  JOYSTICK1 BUTTON8 = 28;
JOYSTICK2 LEFT = 31;  JOYSTICK2 RIGHT = 32;  JOYSTICK2 UP = 33;  JOYSTICK2 DOWN = 34;
JOYSTICK2 BUTTON1 = 36;  JOYSTICK2 BUTTON2 = 37;  JOYSTICK2 BUTTON3 = 38;  JOYSTICK2 BUTTON4 = 39;
JOYSTICK2 BUTTON5 = 40;  JOYSTICK2 BUTTON6 = 41;  JOYSTICK2 BUTTON7 = 42;  JOYSTICK2 BUTTON8 = 43;*/




            if (objects[i].events[ii].name=="alarm")
            {
               string a=tostring(objects[i].events[ii].addind);
               almlist+="  if(alarm["+a+"]>=0) alarm["+a+"]--; \r\n  if ((alarm["+a+"]) == 0) ";
               almlist+="  myevent_"+evname+tostring(addind)+"();\r\n\r\n";
               writes(IDE_EDIT_objectdeclarations,"enigma::variant myevent_"+evname+tostring(addind)+"();\r\n\r\n");
               continue;
            }



            writes(IDE_EDIT_objectdeclarations," #define ENIGMAEVENT_"+evname+" 1\r\n enigma::variant myevent_"+evname+"(); \r\n\r\n");
         }

         if (kbdlist!="") writes(IDE_EDIT_objectdeclarations," #define ENIGMAEVENT_keyboard 1\r\n enigma::variant myevent_keyboard()\r\n {\r\n"+kbdlist+"\r\n  return 0;\r\n }\r\n");
         if (kplist!="") writes(IDE_EDIT_objectdeclarations," #define ENIGMAEVENT_keypress 1\r\n enigma::variant myevent_keypress()\r\n {\r\n"+kplist+"\r\n  return 0;\r\n }\r\n");
         if (krlist!="") writes(IDE_EDIT_objectdeclarations," #define ENIGMAEVENT_keyrelease 1\r\n enigma::variant myevent_keyrelease()\r\n {\r\n"+krlist+"\r\n  return 0;\r\n }\r\n");

         if (mouselist!="") writes(IDE_EDIT_objectdeclarations," #define ENIGMAEVENT_mouse 1\r\n enigma::variant myevent_mouse()\r\n {\r\n"+mouselist+"  return 0;\r\n }\r\n");

         if (almlist!="") writes(IDE_EDIT_objectdeclarations," #define ENIGMAEVENT_alarm 1\r\n enigma::variant myevent_alarm()\r\n {\r\n"+almlist+"  return 0;\r\n }\r\n");


         //Step event should be present
         if (!didstep)
         { fprintf(IDE_EDIT_objectdeclarations," #define ENIGMAEVENT_step 1\r\n enigma::variant myevent_step() { enigma::step_basic(this); return 0; }\r\n"); }

         //Write the constructor. That is, the create event + general instance initiation. Or just the latter.
         if (!didcreate) writes(IDE_EDIT_objectdeclarations," OBJ_"+objects[i].name+"(void) \r\n { \r\n  enigma::constructor(this);\r\n  sprite_index="+tostring(objects[i].sprite)+";\r\n }\r\n");
         else writes(IDE_EDIT_objectdeclarations," OBJ_"+objects[i].name+"(void) \r\n { \r\n  sprite_index="+tostring(objects[i].sprite)+";\r\n  enigma::constructor(this);\r\n  myevent_create();\r\n }\r\n");


          //don't have to worry about undefined references to create thanks to this line.
         if (diddestroy) writes(IDE_EDIT_objectdeclarations," ~OBJ_"+objects[i].name+"(void) \r\n { \r\n  if (!enigma::nodestroy) myevent_destroy(); \r\n }\r\n");


         //End the class
         writes(IDE_EDIT_objectdeclarations,"};\r\n\r\n");
      }



  printf("Wrote event declarations.\n");
  fflush(stdout);

      /*********************************************************************
      * Next write down the actual code in those events.
      *********************************************************************/

      int evaddind;


      for (int i=0;i<object_count;i++)
      {
         string objname=objects[i].name;

         for (int ii=0;ii<objects[i].eventcount;ii++)
         {
            evname=objects[i].events[ii].name;
            evaddind=objects[i].events[ii].addind;


            //Write the name of the event, scoped into its class
            if (evname!="keyboard" && evname!="keypress" && evname!="keyrelease" && evname!="mouse" && evname!="alarm")
            writes(IDE_EDIT_objectfunctionality,"enigma::variant enigma::OBJ_"+objname+"::myevent_"+evname+"() \r\n{\r\n");
            else writes(IDE_EDIT_objectfunctionality,"enigma::variant enigma::OBJ_"+objname+"::myevent_"+evname+tostring(evaddind)+"() \r\n{\r\n");

            //write the code
            writes(IDE_EDIT_objectfunctionality,objects[i].events[ii].code+"\r\n");
            //close it
            writes(IDE_EDIT_objectfunctionality,"}\r\n\r\n");
         }
      }


      /*********************************************************************
      * Export a table of object info
      *********************************************************************/

      FILE* IDE_EDIT_objectdata=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectdata.h","wb");
      fputs("//This file was generated by the ENIGMA Development Environment.\r\n\r\n",IDE_EDIT_objectdata);

      for (int i=0;i<object_count;i++)
      {
         fprintf(IDE_EDIT_objectdata,"enigma::objectdata[%d].name=\"%s\";\r\n",objects[i].id,objects[i].name.c_str());
         fprintf(IDE_EDIT_objectdata,"  enigma::objectdata[%d].sprite_index=%d;\r\n",objects[i].id,objects[i].sprite);
         fprintf(IDE_EDIT_objectdata,"  enigma::objectdata[%d].mask_index=%d;\r\n",objects[i].id,objects[i].mask);
         fprintf(IDE_EDIT_objectdata,"  enigma::objectdata[%d].parent=%d;\r\n",objects[i].id,objects[i].parent);
         fprintf(IDE_EDIT_objectdata,"  enigma::objectdata[%d].visible=%d;\r\n",objects[i].id,objects[i].visible);
         fprintf(IDE_EDIT_objectdata,"  enigma::objectdata[%d].solid=%d;\r\n",objects[i].id,objects[i].solid);
         fprintf(IDE_EDIT_objectdata,"  enigma::objectdata[%d].depth=%d;\r\n",objects[i].id,objects[i].depth);
         fprintf(IDE_EDIT_objectdata,"  enigma::objectdata[%d].persistent=%d;\r\n\r\n",objects[i].id,objects[i].persistent);
      }
      fclose(IDE_EDIT_objectdata);


      /* Then include all the scripts that are called in each object.
      *******************************************************************
      //Deprecated as of WILDCLASS rethink, release 4
      for (int i=0; i<object_count;i++)
      {
         for (int ii=0;ii<objects[i].scrcount;ii++)
         {
           writes(IDE_EDIT_objectfunctionality,"enigma::variant enigma::OBJ_"+objects[i].name+"::"+objects[i].scriptnames[ii]+GLOBALscriptargs+"\r\n");
           for (int iii=0; iii<objects[i].scrcount;iii++) if (scripts[iii].name==objects[i].scriptnames[ii])
           { writes(IDE_EDIT_objectfunctionality,scripts[iii].code+"\r\n\r\n"); break; }
         }
      }
      */


      fclose(IDE_EDIT_objectdeclarations);
      fclose(IDE_EDIT_objectfunctionality);

  printf("Wrote event code.\n");
  fflush(stdout);




  /* Give it inherited_locals.h
  ************************************************************/

    FILE* IDE_EDIT_inherited_locals=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h","wb");
    if (IDE_EDIT_inherited_locals==NULL) return -8;

    for (int i=0; i<localcount; i++)
    fprintf(IDE_EDIT_inherited_locals,"%s %s;\r\n",localt[i].c_str(),localn[i].c_str());

    fclose(IDE_EDIT_inherited_locals);

  printf("Wrote inherited locals.\n");
  fflush(stdout);






  /* Implicit access of potentially nonexisting members
  **************************************************************/

      varray<string> alltypes,allnames,allprefixes,allarrays; int allvarcount=0;
      for (int i=0;i<object_count;i++)
      {
         for (int ii=0; ii<objects[i].eventcount; ii++)
         {
            for (int iii=0;iii<objects[i].events[ii].varcount; iii++)
            {
              int on=0;
              for(int iv=0;iv<allvarcount;iv++)
              if (allnames[iv]==objects[i].events[ii].varnames[iii])
              {
                 on=1;
                 if (alltypes[iv]!=objects[i].events[ii].varname_types[iii] && objects[i].events[ii].varname_types[iii]!="" && alltypes[iv]!="")
                 {
                    printf("\n\n*********************************%s*********************************\n",string(allnames[iv].length(),'*').c_str());
                    printf(    "ERROR: Objects contain variable `%s' under two different type names.\n",allnames[iv].c_str());
                    printf(    "Cannot continue with varname of different types. Bailing.\n");
                    printf(    "*********************************%s*********************************\n\n",string(allnames[iv].length(),'*').c_str());
                    return 1;
                 }
                 break;
              }
              if (!on)
                for(int iv=0;iv<localcount;iv++)
                  if (localn[iv]==objects[i].events[ii].varnames[iii])
                  {
                    on=1;
                    break;
                  }
              if (!on)
              {
                for(int iv=0;iv<object_count;iv++)
                  if (objects[iv].name==objects[i].events[ii].varnames[iii])
                  {
                    on=1;
                    break;
                  }
                if (!on)
                {
                  for(int iv=0;iv<sprite_count;iv++)
                    if (sprites[iv].name==objects[i].events[ii].varnames[iii])
                    {
                      on=1;
                      break;
                    }
                  if (!on)
                  {
                    on=varname_onlists(objects[i].events[ii].varnames[iii]);
                    if (!on)
                    {
                       allnames[allvarcount]=objects[i].events[ii].varnames[iii];
                       allprefixes[allvarcount]=objects[i].events[ii].varname_prefix[iii];
                       if (objects[i].events[ii].varname_types[iii]!="")
                       alltypes[allvarcount]=objects[i].events[ii].varname_types[iii];
                       else alltypes[allvarcount]="var";
                       allarrays[allvarcount]=objects[i].events[ii].varname_suffix[iii];
                       allvarcount++;
                    }
                  }
                }
              }
            }
         }
      }

      for (int i=0;i<globalcount;i++)
      {
          int on=0;
          for(int ii=0;ii<allvarcount;ii++)
          if (allnames[ii]==globals[i])
          {
            on=1;
            if (alltypes[ii]!=globaltags[i] && globaltags[i]!="" && alltypes[ii]!="")
            {
               printf("\n\n*********************************%s*****************************************\n",string(allnames[ii].length(),'*').c_str());
               printf(    "ERROR: Global variable `%s' under different type name than local equivalant.\n",allnames[ii].c_str());
               printf(    "Cannot continue with varname of different types. Bailing.\n");
               printf(    "*********************************%s*****************************************\n\n",string(allnames[ii].length(),'*').c_str());
               return 1;
            }
          }
          if (!on) on=varname_onlists(globals[i]);
          if (!on)
          {
            allnames[allvarcount]=globals[i];
            alltypes[allvarcount]=globaltags[i];
            allprefixes[allvarcount]=globalstars[i];
            allarrays[allvarcount]=globalarrays[i];
            if (alltypes[allvarcount]=="") alltypes[allvarcount]="var";
            allvarcount++;
          }
          break;
      }


      varray<string> alltypenames,alltypestars,alltypearrays; int alltypecount=0;
      for (int i=0; i<allvarcount; i++)
      {
        int isatype=0;
        for (int ii=0; ii<alltypecount; ii++) if (alltypenames[ii]==alltypes[i]) isatype=1;
        if (!isatype) { alltypenames[alltypecount]=alltypes[i];
                        alltypearrays[alltypecount]=allarrays[i];
                        alltypestars[alltypecount]=allprefixes[i];
                        alltypecount++; }
      }


      for (int ii=0; ii<allvarcount; ii++)
      {
            string justname;
            string allname=allnames[ii];

            justname="";
            unsigned int j=0;

            while (j<allnames[ii].length() && !((allnames[ii][j]>='A' && allnames[ii][j]<='Z')
            || (allnames[ii][j]>='a' && allnames[ii][j]<='z')
            || (allnames[ii][j]=='_')))
            j++;

            while (j<allnames[ii].length() && ((allnames[ii][j]>='A' && allnames[ii][j]<='Z')
            || (allnames[ii][j]>='a' && allnames[ii][j]<='z')
            || (allnames[ii][j]>='0' && allnames[ii][j]<='9')
            || (allnames[ii][j]=='_')))
            { justname+=allnames[ii][j]; j++; }

            allnames[ii]=justname;
      }

      for (int ii=0; ii<localcount; ii++)
      {
            string justname;
            string allname=localn[ii];

            justname="";
            unsigned int j=0;

            while (j<localn[ii].length() && !((localn[ii][j]>='A' && localn[ii][j]<='Z')
            || (localn[ii][j]>='a' && localn[ii][j]<='z')
            || (localn[ii][j]=='_')))
            j++;

            while (j<localn[ii].length() && ((localn[ii][j]>='A' && localn[ii][j]<='Z')
            || (localn[ii][j]>='a' && localn[ii][j]<='z')
            || (localn[ii][j]>='0' && localn[ii][j]<='9')
            || (localn[ii][j]=='_')))
            { justname+=localn[ii][j]; j++; }

            localn[ii]=justname;
      }


      for (int ii=0; ii<allvarcount; ii++)
      {
         for (int iii=0; iii<globalcount;iii++)
         {
            if (!varname_oninheriteds(globals[iii]))
            if (globals[iii]==allnames[ii])
            {
              globaltags[iii]=alltypes[ii];
              globalstars[iii]=allprefixes[ii];
              globalarrays[iii]=allarrays[ii];
            }
         }
      }


      FILE* IDE_EDIT_wildclass=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_wildclass.h","wb");
      fputs(enigma_license,IDE_EDIT_wildclass);

      fprintf(IDE_EDIT_wildclass,"namespace enigma\r\n{\r\n");
      fprintf(IDE_EDIT_wildclass,"  struct wildclass\r\n  {\r\n");

      int checkedoff[allvarcount];
      for (int ix=0;ix<allvarcount; ix++)
      checkedoff[ix]=0;

      fprintf(IDE_EDIT_wildclass,"    //Contains %d variables\r\n",allvarcount);
      for (int i=0; i<allvarcount; i++)
      {
        fprintf(IDE_EDIT_wildclass,"    %s %s(*%s)%s;\r\n",
        alltypes[i].c_str(),
        allprefixes[i].c_str(),
        allnames[i].c_str(),
        allarrays[i].c_str());
      }
      fprintf(IDE_EDIT_wildclass,"    \r\n");

      fprintf(IDE_EDIT_wildclass,"    //Contains %d locals\r\n",localcount);
      for (int i=0; i<localcount; i++)
      {
        /*if (localn[i][0]=='*' || localn[i][1]=='*' || (allnames[i].find("[",0) != string::npos))
        fprintf(IDE_EDIT_wildclass,"    %s %s;\r\n",localt[i].c_str(),localn[i].c_str());
        else*/
        fprintf(IDE_EDIT_wildclass,"    %s *%s;\r\n",localt[i].c_str(),localn[i].c_str());
      }


      fprintf(IDE_EDIT_wildclass,"    //Contains %d different types\r\n",alltypecount);
      for (int i=0; i<alltypecount; i++)
      {
        fprintf(IDE_EDIT_wildclass,"    %s %s(*garbage%s%s%s)%s;\r\n",
        alltypenames[i].c_str(),
        alltypestars[i].c_str(),
        string_replace_all(alltypenames[i]," ","_").c_str(),
        string_replace_all(string_replace_all(alltypestars[i],"*","star_"),"(","parenth_").c_str(),
        arraybounds_as_str(alltypearrays[i]).c_str(),
        alltypearrays[i].c_str());
      }
      fprintf(IDE_EDIT_wildclass,"    \r\n    \r\n");



      /*********************************************************************************************
      *                                                                                            *
      *********************************************************************************************/

      fputs("    void setto(object* from)\r\n    {\r\n",IDE_EDIT_wildclass);

      for (int ii=0; ii<allvarcount; ii++)
      {
         fprintf(IDE_EDIT_wildclass,"        %s = garbage%s%s%s;\r\n",allnames[ii].c_str(),
         string_replace_all(alltypes[ii]," ","_").c_str(),
         string_replace_all(string_replace_all(allprefixes[ii],"*","star_"),"(","parenth_").c_str(),
         arraybounds_as_str(allarrays[ii]).c_str());
      }

      for (int ii=0; ii<localcount; ii++)
      {
         fprintf(IDE_EDIT_wildclass,"      %s = &from->%s;\r\n",localn[ii].c_str(),localn[ii].c_str());
      }


      /*                                                                                         */
      /*                                                                                         */
      /*                                                                                         */

      for (int i=0;i<object_count;i++)
      {
        int putted;
        putted=0;

        for (int ii=0; ii<allvarcount; ii++)
        {
           if (!putted)
           { fprintf(IDE_EDIT_wildclass,"      \r\n      if (from->object_index==%d)\r\n      {\r\n",objects[i].id); putted=1; }

           int has=0;
           for (int iii=0; iii<objects[i].eventcount; iii++)
            for (int iv=0;iv<objects[i].events[iii].varcount; iv++)
             if (objects[i].events[iii].varnames[iv]==allnames[ii]) { has=1; break; }
           if (has)
             fprintf(IDE_EDIT_wildclass,"        %s = &((OBJ_%s*)from)->%s;\r\n",allnames[ii].c_str(),objects[i].name.c_str(),allnames[ii].c_str());
        }

        if (putted) fputs("      }\r\n",IDE_EDIT_wildclass);
      }

      fprintf(IDE_EDIT_wildclass,"      \r\n      if (from->object_index==-5) //Global\r\n      {\r\n");

      for (int ii=0; ii<globalcount; ii++)
        fprintf(IDE_EDIT_wildclass,"        %s = &((globals*)from)->%s;\r\n",globals[ii].c_str(),globals[ii].c_str());

      fputs("      }\r\n",IDE_EDIT_wildclass);

      fputs("    }\r\n",IDE_EDIT_wildclass);

      /*********************************************************************************************
      *********************************************************************************************/

      fputs("    \r\n    wildclass() {}\r\n",IDE_EDIT_wildclass);
      fputs(        "    wildclass(object* from)            { setto(from); }\r\n",IDE_EDIT_wildclass);
      fputs(        "    wildclass &operator=(object *from) { setto(from); return *this; }",IDE_EDIT_wildclass);

      /********************************************************************************************/

      fprintf(IDE_EDIT_wildclass,"  };\r\n}\r\n");
      fclose(IDE_EDIT_wildclass);






  /* Finally, write the with declarations and then the with code and scripts
  *************************************************************************/

      FILE *IDE_EDIT_withdeclarations;
      IDE_EDIT_withdeclarations=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_withdeclarations.h","wb");
      if (IDE_EDIT_withdeclarations==NULL) return -8;

      writes(IDE_EDIT_withdeclarations,"//This file was generated by the ENIGMA Development Environment.\r\n\r\n");

      for (int i=0; i<globalcount;i++)
      {
         if (!varname_oninheriteds(globals[i]))
         {
           if (globaltags[i]=="")
             writes(IDE_EDIT_withdeclarations,"var "+globals[i]+"; \r\n");
           else
             fprintf(IDE_EDIT_withdeclarations,"%s %s%s%s; \r\n",globaltags[i].c_str(),globalstars[i].c_str(),globals[i].c_str(),globalarrays[i].c_str());
         }
      }



      fclose(IDE_EDIT_withdeclarations);


  printf("Wrote with and script code.\n");
  fflush(stdout);



  /* Export the mode data
  **********************************************************/

      FILE *IDE_EDIT_modesenabled;
      IDE_EDIT_modesenabled=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h","wb");

      fputs("//This file was generated by the ENIGMA Development Environment.\r\n\r\n",IDE_EDIT_modesenabled);
      fprintf(IDE_EDIT_modesenabled,"#define BUILDMODE %d\r\n",build);
      fprintf(IDE_EDIT_modesenabled,"#define DEBUGMODE %d\r\n\r\n",debug);

      if (build) fprintf(IDE_EDIT_modesenabled,"#define BUILDFILE \"%s\"\r\n\r\n",changefile.c_str());

      fclose(IDE_EDIT_modesenabled);

  printf("Wrote mode data.\n");
  fflush(stdout);


  /* Export the room data
  **********************************************************/

      FILE* IDE_EDIT_roomarrays,* IDE_EDIT_roomcreates;
      IDE_EDIT_roomarrays=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h","wb");
      IDE_EDIT_roomcreates=fopen("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h","wb");
      if (IDE_EDIT_roomarrays==NULL) return -8;
      if (IDE_EDIT_roomcreates==NULL) return -8;

      fprintf(IDE_EDIT_roomarrays,"//File generated by the ENIGMA Development Environment\r\n\r\n");
      fprintf(IDE_EDIT_roomcreates,"//File generated by the ENIGMA Development Environment\r\n\r\n");

      int room_max=0;

      for (int i=0;i<room_count;i++)
      {
         for (int ii=0; ii<rooms[i].instance_count; ii++)
         {
             if (rooms[i].instances[ii].code!="")
             {
               fprintf(IDE_EDIT_roomcreates,"  //Instance creation code \r\nenigma::variant instcreate%d()\r\n{\r\n  %s\r\n  return 0;\r\n}\r\n\r\n",rooms[i].instances[ii].id,rooms[i].instances[ii].code.c_str());
             }
         }
         if (rooms[i].createcode!="")
         fprintf(IDE_EDIT_roomcreates,"//Room creation code \r\nenigma::variant roomcreatecode%d()\r\n{\r\n  %s\r\n  return 0;\r\n}\r\n\r\n",rooms[i].id,rooms[i].createcode.c_str());



         fputs("//Room code (Room and Instance Create code)\r\n",IDE_EDIT_roomcreates);
         fprintf(IDE_EDIT_roomcreates,"void roomcreate%d()\r\n{\r\n",rooms[i].id);
           if (rooms[i].createcode!="")
           fprintf(IDE_EDIT_roomcreates,"  roomcreatecode%d();\r\n",rooms[i].id);
           for (int ii=0; ii<rooms[i].instance_count; ii++)
           {
               if (rooms[i].instances[ii].code!="")
               {
                 fprintf(IDE_EDIT_roomcreates,"  instcreate%d();\r\n",rooms[i].instances[ii].id);
               }
           }
         fputs("}\r\n",IDE_EDIT_roomcreates);
      }

      //clear to here
      for (int i=0;i<room_count;i++)
      {
          if (i>room_max) room_max=i;

          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].name=\"%s\";\r\n",rooms[i].id,rooms[i].name.c_str());
          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].cap=\"%s\";\r\n",rooms[i].id,rooms[i].caption.c_str());
          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].backcolor=%d;\r\n",rooms[i].id,rooms[i].backcolor);
          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].spd=%d;\r\n",rooms[i].id,rooms[i].speed);
          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].width=%d;\r\n",rooms[i].id,rooms[i].width);
          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].height=%d;\r\n",rooms[i].id,rooms[i].height);
          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].views_enabled=%d;\r\n",rooms[i].id,rooms[i].views_enabled);
          for (int ii=0;ii<7;ii++)
          {
              fprintf(IDE_EDIT_roomarrays,"    enigma::roomdata[%d].start_vis[%d]=%d;\r\n",rooms[i].id,ii,rooms[i].start_vis[ii]);
              fprintf(IDE_EDIT_roomarrays,"    enigma::roomdata[%d].area_x[%d]=%d; enigma::roomdata[%d].area_y[%d]=%d; enigma::roomdata[%d].area_w[%d]=%d; enigma::roomdata[%d].area_h[%d]=%d;\r\n",rooms[i].id,ii,rooms[i].area_x[ii],rooms[i].id,ii,rooms[i].area_y[ii],rooms[i].id,ii,rooms[i].area_w[ii],rooms[i].id,ii,rooms[i].area_h[ii]);
              fprintf(IDE_EDIT_roomarrays,"    enigma::roomdata[%d].port_x[%d]=%d; enigma::roomdata[%d].port_y[%d]=%d; enigma::roomdata[%d].port_w[%d]=%d; enigma::roomdata[%d].port_h[%d]=%d;\r\n",rooms[i].id,ii,rooms[i].port_x[ii],rooms[i].id,ii,rooms[i].port_y[ii],rooms[i].id,ii,rooms[i].port_w[ii],rooms[i].id,ii,rooms[i].port_h[ii]);
              fprintf(IDE_EDIT_roomarrays,"    enigma::roomdata[%d].object2follow[%d]=%d;\r\n",rooms[i].id,ii,rooms[i].object2follow[ii]);
              fprintf(IDE_EDIT_roomarrays,"    enigma::roomdata[%d].hborder[%d]=%d; enigma::roomdata[%d].vborder[%d]=%d; enigma::roomdata[%d].hspd[%d]=%d; enigma::roomdata[%d].vspd[%d]=%d;\r\n",  rooms[i].id,ii,rooms[i].hborder[ii],rooms[i].id,ii,rooms[i].vborder[ii],rooms[i].id,ii,rooms[i].hspd[ii],rooms[i].id,ii,rooms[i].vspd[ii]);
          }
          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].instancecount=%d;\r\n",rooms[i].id,rooms[i].instance_count);
          fprintf(IDE_EDIT_roomarrays,"  enigma::roomdata[%d].createcode=(void(*)())roomcreate%d;\r\n",rooms[i].id,rooms[i].id);
      }

      int idmax=0;

      fprintf(IDE_EDIT_roomarrays,"int instdata[]={\r\n"); int wontl;
      for (int i=wontl=0;i<room_count;i++) for (int aind=wontl=0;aind<rooms[i].instance_count;aind++)
      {
        fprintf(IDE_EDIT_roomarrays,"%d,%d,%d,%d,",rooms[i].instances[aind].id,rooms[i].instances[aind].obj,rooms[i].instances[aind].x,rooms[i].instances[aind].y);
        wontl++; if (wontl>9) { wontl=0; fprintf(IDE_EDIT_roomarrays,"\r\n"); }
        if (idmax<rooms[i].instances[aind].id) idmax=rooms[i].instances[aind].id;
      } fprintf(IDE_EDIT_roomarrays,"0};\r\n\r\n\r\n\r\n");

      fprintf(IDE_EDIT_roomarrays,"enigma::room_max=%d+1;\r\n",room_max);
      fprintf(IDE_EDIT_roomarrays,"enigma::maxid=%d+1;\r\n",idmax);

      fclose(IDE_EDIT_roomarrays);
      fclose(IDE_EDIT_roomcreates);

  printf("Wrote room data.\n");
  fflush(stdout);








  /* Compile the exe
  **********************************************************/

      string gcc,o_compile_command,exe_compile_command,linker,libs;

      #if WINDOWS
      FILE* adhoc=fopen("gcc_adhoc","rb");

      if (adhoc == NULL)
      {
         FILE* gccname=fopen("gcc_path","rb");
         if (gccname == NULL)
         {
            gcc="ENIGMAsystem\\bin\\g++.exe";
         }
         else
         {
            char readhere[1025];
            readhere[fread(readhere,1,1024,gccname)]='\0';
            gcc=readhere;
            fclose(gccname);
         }
      }
      else
      {
         gcc="g++ ";
         fclose(adhoc);
      }
      #else
      gcc="g++ ";
      #endif

      #if WINDOWS
        linker="-lopengl32 ENIGMAsystem\\include\\zlib\\libzlib.a -mwindows";
      #elif LINUX
        linker="-lGL -lz";
      #endif

      //The basics
      //{
        libs += " ENIGMAsystem/SHELL/Universal_System/CallbackArrays.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/collisions.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/compression.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/EGMstd.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/estring.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/IMGloading.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/instance.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/mathnc.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/OBJaccess.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/object.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/reflexive_types.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/roomsystem.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/simplecollisions.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/spritestruct.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/var_cr3.o";
        libs += " ENIGMAsystem/SHELL/Universal_System/WITHconstruct.o";
      //}

      #if WINDOWS
        libs += " ENIGMAsystem/SHELL/Platforms/WINDOWS/WINDOWScallback.o";
        libs += " ENIGMAsystem/SHELL/Platforms/WINDOWS/WINDOWSfonts.o";
        libs += " ENIGMAsystem/SHELL/Platforms/WINDOWS/WINDOWSstd.o";
        libs += " ENIGMAsystem/SHELL/Platforms/WINDOWS/WINDOWSwindow.o";
      #elif LINUX
        libs += " ENIGMAsystem/SHELL/Platforms/xlib/XLIBwindow.o";
        #endif

      //if (graphics_system==gs_opengl)
      //{
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GSblend.o";
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GScolors.o";
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GSenable.o";
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GSprmtvs.o";
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GSsprite.o";
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GSspriteadd.o";
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GSstdraw.o";
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GSsurface.o";
        libs += " ENIGMAsystem/SHELL/Graphics_Systems/OpenGL/GSmiscextra.o";
      //}

      #if WINDOWS
      o_compile_command=         gcc+" -s -c ENIGMAsystem\\SHELL\\SHELLmain.cpp";
      #else
      o_compile_command=         gcc+" -s -c ENIGMAsystem/SHELL/SHELLmain.cpp";
      #endif
      exe_compile_command=       gcc+" -o \""+outname+"\" SHELLmain.o"+libs+" "+linker;

      //Read the Post-Compile Script
      load_filter_args();
      o_compile_command+=pcs::cargs;
      exe_compile_command+=pcs::largs;



      int result=0;

      printf("Starting compile. This may take a minute.\n");
      fflush(stdout);


      printf("Execute: %s\n",o_compile_command.c_str()); fflush(stdout);

      try {
      result=system(o_compile_command.c_str());    //compile o
      } catch (int a)   { printf("Caught error code %d\n",a); fflush(stdout); }
        catch (char* a) { printf("GCC error: %s\n",a); fflush(stdout); }

      if (result!=0)
      {
          printf("Compile failed. [%d]\n",result);
          return -6;
      }


      printf("Execute: %s\n",exe_compile_command.c_str());  fflush(stdout);

      result=system(exe_compile_command.c_str());  //compile exe
      if (result != 0)
      {
          printf("Compile failed, though outputted code is syntactically correct. This likely resulted from linker errors. Please report this occurence.\n(%s) [%d]",exe_compile_command.c_str(),result);
          return -7;
      }





  //Execute the post-compile commands
  printf("Compiler finished. Executing PCS...\n"); fflush(stdout);
  execute_pcs_pre_commands(outname);
  fflush(stdout);



  printf("Adding sprites...\n");
  fflush(stdout);

  /* Read the sprites now that we've compiled, and put them in the exe.
  ***********************************************************************/

      //Get ready to write to the exe
      FILE* exe=fopen(outname.c_str(),"rb");

      if (exe==NULL)
      {
         printf("Could not open executable... File does not seem to exist\n(%s)\n",outname.c_str());
         fflush(stdout);
         return -8;
      }

      fseek(exe,0,SEEK_END);
      int spritelocation=ftell(exe);

      fclose(exe);

      exe=fopen(outname.c_str(),"ab");

      if (exe==NULL)
      {
         printf("Could not open executable... It is there, but appears to be write protected.\n");
         fflush(stdout);
         return -8;
      }

      //Where do we write the sprites?
      //(Also used later to indicate location for reading.)


      //read that we are at the sprite section
      if (reads(4) != "spr\0")
      return -4;

      //Add some nulls to the exe
      fwrite("\0\0\0\0",1,4,exe);

      //Add some nulls to the exe
      fwrite(&sprite_count,4,1,exe);

      for (int i=0;i<sprite_count;i++)
      {
         int idttrans=sprites[i].id;
         fwrite(&idttrans,4,1,exe); //id
         transi(exe); //width
         transi(exe); //height
         transi(exe); //xorig
         transi(exe); //yorig

         int subimages=transi(exe); //duh (subimages)

         for (int ii=0;ii<subimages;ii++)
         {
            transi(exe);  //transparent color

            transi(exe);  //size when unpacked

            transSTR(exe); //sprite data
         }
      }

      printf("Added sprites.\n");
      fflush(stdout);

      //Tell where the sprites are
      fwrite("\0\0\0\0sprn",8,1,exe);
      fwrite(&spritelocation,4,1,exe);

      printf("Finalized sprites.\n");
      fflush(stdout);

      //close that
      fclose(exe);
      fclose(enigma_file);



      //Execute the post-resource commands
      printf("Resources added. Executing post PCS...\n"); fflush(stdout);
      execute_pcs_post_commands(outname);
      fflush(stdout);



      //run it for build mode
      if (build)
      {
        int sr = system(outname.c_str());
        printf("'Build mode' game exited with status %d\r\n",sr);
        remove(outname.c_str());
      }

      //clean up
      remove(filename.c_str());
      remove("SHELLmain.o");
      remove("game_icon.ico");

      printf("Cleaned up.\n");
      fflush(stdout);


  return 0;
}
