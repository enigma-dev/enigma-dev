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

/**Sprite adding functions*****************************************************\

int sprite_add(ARG filename,ARG2 imgnumb,ARG3 precise,ARG4 transparent,ARG5 smooth,ARG6 preload,ARG7 x_offset,ARG8 y_offset)

\******************************************************************************/


//          sprite_add(    fname,        imgnumb,     precise,     transparent,     smooth,     preload,     xorig,        yorig)
int sprite_add(std::string filename,int imgnumb,int precise,int transparent,int smooth,int preload,int x_offset,int y_offset)
{
    char* pxdata;
    GLuint texture;
    int width,height,fullwidth,fullheight;
    pxdata=enigma::load_bitmap(filename,&width,&height,&fullwidth,&fullheight);
    
    
     glGenTextures(1, &texture);
     glBindTexture(GL_TEXTURE_2D, texture);
     
     glTexImage2D(GL_TEXTURE_2D,
 			     smooth,
 			     4,
 			     fullwidth,
 			     fullheight,
 			     0,
 			     GL_RGBA,
 			     GL_UNSIGNED_BYTE,
 			     pxdata
 			     );
     
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    
     //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, bmpwidth, bmpheight, GL_RGB, GL_UNSIGNED_BYTE, readbuffer); 
     
     
     
     //Free buffer
     free(pxdata);
     //enigma::spritestructarray[enigma::currentspriteind].pixeldata=(void**) malloc(sizeof(void*));
     //enigma::spritestructarray[enigma::currentspriteind].pixeldata[0]=bitmapbuffer;
     
     enigma::spritestructarray[enigma::currentspriteind]=new enigma::sprite;
     
     enigma::spritestructarray[enigma::currentspriteind]->id=enigma::currentspriteind;
     
     enigma::spritestructarray[enigma::currentspriteind]->subcount=1;
     enigma::spritestructarray[enigma::currentspriteind]->width=width;
     enigma::spritestructarray[enigma::currentspriteind]->height=height;
     
     enigma::spritestructarray[enigma::currentspriteind]->xoffset=x_offset;
     enigma::spritestructarray[enigma::currentspriteind]->yoffset=y_offset;
     
     enigma::spritestructarray[enigma::currentspriteind]->texbordx=(double) width/fullwidth;
     enigma::spritestructarray[enigma::currentspriteind]->texbordy=(double) height/fullheight;
     
     enigma::spritestructarray[enigma::currentspriteind]->texturearray[0]=texture;
     
     enigma::currentspriteind++;
     
     return enigma::currentspriteind-1;
}



/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */

namespace enigma
{
    //Adds an empty sprite to the list
    int new_sprexe(int w, int h, int x, int y, int pl, int sm) 
    {
         int fullwidth=1,fullheight=1;
         while (fullwidth<w) fullwidth*=2;
         while (fullheight<h) fullheight*=2;
         
         enigma::spritestructarray[enigma::currentspriteind]=new enigma::sprite;
         
         enigma::spritestructarray[enigma::currentspriteind]->id=enigma::currentspriteind;
         
         enigma::spritestructarray[enigma::currentspriteind]->subcount=0;
         enigma::spritestructarray[enigma::currentspriteind]->width=w;
         enigma::spritestructarray[enigma::currentspriteind]->height=h;
         
         enigma::spritestructarray[enigma::currentspriteind]->xoffset=x;
         enigma::spritestructarray[enigma::currentspriteind]->yoffset=y;
         
         enigma::spritestructarray[enigma::currentspriteind]->texbordx=(double) w/fullwidth;
         enigma::spritestructarray[enigma::currentspriteind]->texbordy=(double) h/fullheight;
         
         enigma::currentspriteind++;
         
         return enigma::currentspriteind-1;
    }
    
    
    #if COLLIGMA
    collCustom* generate_bitmask(unsigned char* pixdata,int x,int y,int w,int h)
    {
      collCustom* thenewmask = new collCustom(w,h,x,y,x,y);
      
        for (int xp=0;xp<w*h;xp++)
        {
          collbitSet(*thenewmask,xp % w,xp/w,pixdata[xp*4+3]>0); //the width times the number of rows, the current column*4, then 3
        }
      return thenewmask;
    }
    #endif
    
    //Adds a subimage to an existing sprite from the exe
    void sprexe(int sprid,int x, int y, int w, int h, 
                int te, int tr,int tg,int tb,
                unsigned char* chunk)
    {
        GLuint texture;
        int fullwidth=1,fullheight=1;
        while (fullwidth<w) fullwidth*=2;
        while (fullheight<h) fullheight*=2;
        
        unsigned char* imgpxdata=new unsigned char[4*fullwidth*fullheight+1];
        
        
        //Organize this into RGBA format.
        //It was stored as RGB to save space.
        int i1=0,i2=0;
        unsigned char R,G,B;
        int rowindex,colindex;
        for (rowindex=0; rowindex<h; rowindex++)
        {
          if (te)
          for (colindex=0; colindex<w; colindex++)
          {
              imgpxdata[i2++]=R=chunk[i1++]; //Read R
              imgpxdata[i2++]=G=chunk[i1++]; //Read G
              imgpxdata[i2++]=B=chunk[i1++]; //Read B
              
              if (R!=tr || G!=tg || B!=tb)
              imgpxdata[i2++]=255; else imgpxdata[i2++]=0; //Transparency
          } else for (colindex=0; colindex<w; colindex++) {
              imgpxdata[i2++]=R=chunk[i1++]; //Read R
              imgpxdata[i2++]=G=chunk[i1++]; //Read G
              imgpxdata[i2++]=B=chunk[i1++]; //Read B
              imgpxdata[i2++]=255; //Transparency
          }
          while (colindex<fullwidth)
          { imgpxdata[i2++]=0; imgpxdata[i2++]=0; imgpxdata[i2++]=0; imgpxdata[i2++]=0; colindex++; }
        }
        while (rowindex<fullheight) { for (colindex=0; colindex<fullwidth; colindex++)
        { imgpxdata[i2++]=0; imgpxdata[i2++]=0; imgpxdata[i2++]=0; imgpxdata[i2++]=0; } rowindex++; }
        
        #if COLLIGMA
        collCustom* themask=generate_bitmask(imgpxdata,x,y,w,h);
        #endif
        
         glGenTextures(1, &texture);
         glBindTexture(GL_TEXTURE_2D, texture);
         
         glTexImage2D(GL_TEXTURE_2D,0,4,fullwidth,fullheight,
     			      0,GL_RGBA,GL_UNSIGNED_BYTE,imgpxdata);
         
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        
         //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, bmpwidth, bmpheight, GL_RGB, GL_UNSIGNED_BYTE, readbuffer); 
         
         glBindTexture(GL_TEXTURE_2D,0);
         
         //Free buffer
         delete []imgpxdata;
         
         
         enigma::sprite* sprstr=enigma::spritestructarray[sprid];
         sprstr->texturearray[sprstr->subcount]=texture;
         #if COLLIGMA
         sprstr->bitmask[enigma::spritestructarray[sprid]->subcount]   =  themask;
         #endif
         sprstr->subcount+=1;
    }
}        
