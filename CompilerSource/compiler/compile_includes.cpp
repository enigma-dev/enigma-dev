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


#include <string>
#include <stdio.h>
using namespace std;

#include "../general/estring.h"
#include "../general/darray.h"


/*
 * These functions are designed to do all the file garbage for you, making the
 * code look a little less messy, and a lot simpler.
 */

char* writehere;
FILE* enigma_file;
int EXPECTNULL=0;

   inline void space(int size)
   {
     delete[] writehere;
     writehere=new char[size+1];
     
     for (int i=0;i<size+1;i++)
     writehere[i] = 0;
   }
   inline void cap(int at)
   {
     writehere[at] = 0;
   }
   
   string reads(int size)
   {
     space(size);
     cap(fread(writehere,size,1,enigma_file));
     return writehere;
   }
   string readSTR()
   {
     int size;
     if (fread(&size,4,1,enigma_file) != 1)
       return "";
     
     space(size+1);
     cap(fread(writehere,size,1,enigma_file));
     
     int readi;
     if (fread(&readi,4,1,enigma_file) != 1)
       return "";
     if (readi != 0) EXPECTNULL=1;
     return writehere;
   }
   /*string readNAME()
   {
     int size;
     fread(&size,4,1,enigma_file);
     
     space(size+1);
     fread(writehere,size,1,enigma_file);
     int readi;
     fread(&readi,4,1,enigma_file);
     if (readi != 0) EXPECTNULL=1;
     return writehere;
   }*/
   int readi()
   {
     int a;
     if (!fread(&a,4,1,enigma_file))
       return 0;
     return a;
   }
   unsigned char readb()
   {
     unsigned char a;
     if (!fread(&a,1,1,enigma_file))
       return 0;
     return a;
   }
   char* readv(int size)
   {
     char* a=new char[size+3];
     if (!fread(a,1,size,enigma_file));
     return a;
   }
   
   void writes(FILE* fn,string str)
   {
     fwrite(str.c_str(),1,str.length(),fn);
   }
   void writeDefine(FILE* fn,string varname,int value)
   {
     string write="const int "+varname+" = "+tostring(value)+";\r\n";
     fwrite(write.c_str(),1,write.length(),fn);
   }
   
   
   void transfer(int size,FILE* in,FILE* out)
   {
     unsigned char buf[BUFSIZ];
     int s;
     while (size > BUFSIZ)
     {
       s = fread(buf,BUFSIZ,1,in);
       fwrite(buf,s,1,out);
       size -= s;
     }
     size = fread(buf,size,1,in);
     fwrite(buf,size,1,out);
     fclose(out);
   }
   
   int fileout(char* name,char* data,int size)
   {
        FILE* out=fopen(name,"wb");
        if (out==NULL)
        {
          printf("Failed to open output file \"%s\"",name);
          fflush(stdout);
          return -8;
        }
        int n;

        for (n=0;n+BUFSIZ<size;n+=BUFSIZ)
        fwrite(&(((char*)data)[n]),BUFSIZ,1,out);
        fwrite(&(((char*)data)[n]),size-n,1,out);

        fclose(out);
        delete[] data;
        return 0;
   }
   
   
   
   
   
   int transi(FILE* out)
   {
       int a;
       if (fread(&a,4,1,enigma_file))
       fwrite(&a,4,1,out);
       return a;
   }
   void transSTR(FILE* out)
   {
       int a;
       if (!fread(&a,4,1,enigma_file))
         return;
       fwrite(&a,4,1,out);

       int aa;
       char outbuf[BUFSIZ+1];
       while (a>BUFSIZ)
       {
             aa=fread(outbuf,1,BUFSIZ,enigma_file);
             fwrite(outbuf,1,aa,out);
             a-=aa;
       }
       aa=fread(outbuf,1,a,enigma_file);
       fwrite(outbuf,1,aa,out);

       fseek(enigma_file,4,SEEK_CUR);
       fwrite("\0\0\0\0",4,1,out);
   }
