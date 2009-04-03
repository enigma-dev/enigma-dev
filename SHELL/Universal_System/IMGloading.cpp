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

namespace enigma
{
    char* load_bitmap(std::string filename,int* width,int* height, int* fullwidth, int* fullheight)
    {
        FILE *imgfile;
        int bmpstart, bmpwidth, bmpheight;
        int bmpsize, colordepth;

        imgfile=fopen((char*)filename.c_str(),"r");
        if (imgfile==NULL)
        return 0;

        fseek(imgfile,0,SEEK_END);
        bmpsize=ftell(imgfile);



        //Get the width, height, and start offset of bitmap pixels
        fseek(imgfile,10,SEEK_SET);
        fread(&bmpstart,1,4,imgfile);
        fseek(imgfile,18,SEEK_SET);
        fread(&bmpwidth,1,4,imgfile);
        fread(&bmpheight,1,4,imgfile);

        //Read the color depth, in bits
        fseek(imgfile,28,SEEK_SET);
    	colordepth=fgetc(imgfile);

        //Only take 24-bit bitmaps for now
        if (colordepth != 24)
        return 0;




        int widfull=1, hgtfull=1;

        do { widfull*=2; }
        while (widfull<bmpwidth);
        do { hgtfull*=2;}
        while (hgtfull<bmpwidth);



        int i, ih, iw, indx=0;
        char* bitmapbuffer=new char[(widfull*hgtfull*4+1+1)];
        int pad=bmpwidth%4; //This is that set of nulls that follows each line.




        fseek(imgfile,bmpstart,SEEK_SET);

        for (i=0; i<hgtfull-bmpheight; i++)
        {
             for (iw=0; iw<widfull; iw++)
             {
                 bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)]=0;
                 bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+1]=0;
                 bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+2]=0;
                 bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+3]=0;
                 indx+=4;
             }
        }
        for (ih=0; ih<bmpheight; ih++)
        {
    	    for (iw=0; iw<bmpwidth; iw++)
    	    {
                bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+2]=fgetc(imgfile);
                bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+1]=fgetc(imgfile);
                bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)]=fgetc(imgfile);

                bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+3]=255;

                indx+=4;
            }
            for (i=0; i<widfull-bmpwidth; i++)
            {
                bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)]=0;
                bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+1]=0;
                bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+2]=0;
                bitmapbuffer[4*(widfull*hgtfull-widfull*((int) ((indx/4)/widfull) +1)+(indx/4)%widfull)+3]=0;
                indx+=4;
            }

            fseek(imgfile,pad,SEEK_CUR);
         }

         fclose(imgfile);

         *width=bmpwidth;
         *height=bmpheight;
         *fullwidth=widfull;
         *fullheight=hgtfull;

         return bitmapbuffer;
    }
}
