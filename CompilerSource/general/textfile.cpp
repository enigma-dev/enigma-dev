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

#include <stdio.h>
#include <string>
#include <map>

using namespace std;

struct textfile
{
   FILE* filestruct;
   string write;
   char rwa;
   int eof;
};
map<int,textfile*> textfiles;
int textfile_max=0;

int file_text_open_read(string filename)
{
    int ind=textfile_max++;
    textfile* ttx=new textfile;
    ttx->filestruct=fopen(filename.c_str(),"rb");
    ttx->rwa='r';
    ttx->eof=0;

    if (ttx->filestruct==NULL)
    return -1;

    textfiles[ind]=ttx;
    return ind;
}

int file_text_open_write(string filename)
{
    int ind=textfile_max++;
    textfile* ttx=new textfile;
    ttx->filestruct=fopen(filename.c_str(),"wb");
    ttx->rwa='w';
    ttx->eof=0;
    textfiles[ind]=ttx;
    return ind;
}

string file_text_read_string(int file)
{
    textfile* tf=textfiles[file];
    FILE* dw=tf->filestruct;

    if (tf->rwa != 'r')
    {
       #if SHOWERRORS
       show_error("File not opened for reading.",0);
       #endif
       return 0;
    }

    int s=ftell(dw);
    int a=fgetc(dw);
    string out;
    out="";

    while (a != '\r' && a != '\n')
    {
          if ((tf->eof=(a == EOF))) break;
          out+=(char)a;
          if ((tf->eof=(feof(dw)!=0))) break;
          a=fgetc(dw);
    }
    fseek(dw,s,SEEK_SET);

    return out;
}

int file_text_readln(int file)
{
    textfile* tf=textfiles[file];
    FILE* dw=tf->filestruct;

    if (tf->rwa != 'r')
    {
       #if SHOWERRORS
       show_error("File not opened for reading.",0);
       #endif
       return 0;
    }

    int b=fgetc(dw);

    while (b != '\r' && b != '\n')
    {
       if (tf->eof |= (b==EOF)) break;
       b=fgetc(dw);
       if (tf->eof |= (feof(dw)!=0)) break;
    }

    char smm=0;
    int isn=fgetc(dw);

    if (b=='\r')
    if (isn != '\n')
    { tf->eof |= (isn==EOF); smm=1; }

    if (smm)
    {
      tf->eof |= (fgetc(dw)==EOF);
      fseek(dw,-2,SEEK_CUR);
    }

    tf->eof |= (feof(dw)!=0);

    return 0;
}

int file_text_eof(int file)
{
    return textfiles[file]->eof;
}

int file_text_write_string(int file,string str)
{
    textfile* tf=textfiles[file];

    if (tf->rwa != 'w' && tf->rwa != 'a')
    {
       #if SHOWERRORS
       show_error("File not opened for writing.",0);
       #endif
       return 0;
    }

    tf->write=str;

    return 0;
}


int file_text_writeln(int file)
{
    textfile* tf=textfiles[file];
    FILE* dw=tf->filestruct;

    if (tf->rwa != 'w' && tf->rwa != 'a')
    {
       #if SHOWERRORS
       show_error("File not opened for writing.",0);
       #endif
       return 0;
    }

    if (tf->write != "")
    fwrite((void*)tf->write.c_str(),tf->write.size(),1,dw);
    fwrite("\n",1,1,dw);
    tf->write="";

    return 0;
}


int file_text_close(int file)
{
    textfile* tf=textfiles[file];
    FILE* dw=tf->filestruct;

    if (tf->rwa != 'r')
    if (tf->write != "")
    {
        fwrite((void*)tf->write.c_str(),tf->write.size(),1,dw);
        tf->write="";
    }

    fclose(dw);
    textfiles.erase(file);
    return 0;
}
