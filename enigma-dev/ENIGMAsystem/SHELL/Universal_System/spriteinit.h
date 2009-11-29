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

namespace enigma
{
    void exe_loadsprs()
    {
       int nullhere;
       char exename[1025];

  	   windowsystem_write_exename(exename);

       FILE* exe=fopen(exename,"rb");

       if (exe==NULL)
       {
          show_error("Failed to load resources. The current exe could not be opened...",0);
          return;
       }

       fseek(exe,-8,SEEK_END);

       char str_quad[5]="    ";
       fread(str_quad,1,4,exe);

       if (((std::string)str_quad)!="sprn")
       { printf("No resorce data in exe.\n"); return; }

       int pos;
       fread(&pos,4,1,exe);

       int sprid,width,height,xorig,yorig;

       //Go to the sprites in the exe
       fseek(exe,pos,SEEK_SET);

       nullhere=-1;
       fread(&nullhere,4,1,exe);
       if (nullhere != 0) return;

       int sprcount; fread(&sprcount,4,1,exe);



       for (int i=0;i<sprcount;i++)
       {
          fread(&sprid,4,1,exe);
          fread(&width,4,1,exe);
          fread(&height,4,1,exe);
          fread(&xorig,4,1,exe);
          fread(&yorig,4,1,exe);

          enigma::new_sprexe(sprid, width, height, xorig, yorig, 1, 0);

          int subimages; fread(&subimages,4,1,exe);



          for (int ii=0;ii<subimages;ii++)
          {
             unsigned char transparent[5]; fread(transparent,1,4,exe);
             int unpacked; fread(&unpacked,1,4,exe);

             unsigned int size; fread(&size,4,1,exe);

             unsigned char* cpixels=new unsigned char[size+1];

             if (cpixels==NULL)
             {
               #if SHOWERRORS
               show_error("Failed to load sprite: Cannot allocate enough memory ["+string(unpacked)+"b]",0);
               #endif
               break;
             }

             int sz1=size;
             int sz2=fread(cpixels,1,size,exe);
             if (sz1 != sz2)
             {
               #if SHOWERRORS
               show_error("Failed to load sprite: Data is truncated before exe end. Read "+string(sz2)+" out of expected "+string(sz1),0);
               //show_error(std::string("Strerror reports ")+strerror(errno)+". eof: "+string(feof(exe)),0);
               #endif
               break;
             }

             unsigned char* pixels=new unsigned char[unpacked+1];
             int decsz=zlib_decompress(cpixels, size, unpacked, pixels);
             if (decsz!=width*height*3)
             {
               #if SHOWERRORS
               show_error("Sprite load error: Sprite does not match expected size",0);
               #endif
               continue;
             }

             delete []cpixels;

             sprexe(sprid, xorig, yorig, width, height, transparent[3] == 255, transparent[2],transparent[1],transparent[0],pixels);

             delete []pixels;

             nullhere=-1;
             fread(&nullhere,1,4,exe);
             if (nullhere!=0)
             {
               #if SHOWERRORS
               show_error("Sprite load error: Null terminator expected",0);
               #endif
               break;
             }
          }
       }
       nullhere=-1;
       fread(&nullhere,1,4,exe);
       if (nullhere!=0)
       {
         #if SHOWERRORS
         show_error("Sprite load error: Final null terminator expected, world cannot end",0);
         #endif
       }
       fclose(exe);
    }
}
