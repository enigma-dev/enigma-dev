/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Adam Domurad                                             **
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
struct collCustom {
       unsigned short xDim, yDim, byteDim;//X and Y dimensions in pixels, and then X dimension in bytes
       char * cMask;//Mask of the object, 1Dimensional Char array used as 2D, rounds xOff to next highest b;
       int xOff, yOff;//X and Y offsets
       float xOrigin, yOrigin;//Point of rotation
       float xFocus, yFocus;//Point of scaling
       collCustom (unsigned short axDim, unsigned short ayDim, int axOff=0, int ayOff=0, float axOrigin=0, float ayOrigin=0, float axFocus=0, float ayFocus=0, unsigned short aFill=1)
       {          
                  unsigned int byteRef;
                  unsigned char bitMod;
                  unsigned short i,ii,i2,ii2;
                  byteDim=((axDim-1)>>3)+1;
                  xDim = axDim;
                  yDim = ayDim;
                  cMask = new char[(int)(byteDim*yDim)];//
                  xOff = axOff;
                  yOff = ayOff;
                  xOrigin = axOrigin;
                  yOrigin = ayOrigin;                  
                  yFocus=ayFocus;
                  xFocus=axFocus;
                  if (aFill!=1)
                         {
                         for (i=0;i<byteDim*yDim;i++)
                         cMask[i]=0;
                         }
                  switch (aFill)
                  {
                         case 0:
                         break;
                         case 1:
                              for (i=0;i<byteDim*yDim;i++)
                              cMask[i]=255 >>((((i+1)%byteDim)==0)*((-xDim)&7));
                              break;
                         case 2://ellipse fill set, uses center of the bitmask as center of ellipse and a nonconstant radius
                              float h,xRad,cLen;
                              unsigned short byteI,byteII;
                              h=(float)(xDim)/(yDim);
                              xRad=(float)xDim/2;
                              for (i=0;i<yDim;i++)
                              {    
                                  cLen=sqrt((i+0.5)*h*(xDim-(i+0.5)*h));
                                  i2=(unsigned short)round(xRad-cLen);
                                  ii2=(unsigned short)round(xRad+cLen)-1;//Define the two X-Bounds
                                  byteI=i2>>3;
                                  byteII=ii2>>3;
                                  for (ii=byteI;ii<=byteII;ii++)//Set the array according to the X-Bounds
                                  {
                                      byteRef=ii+i*(byteDim);
                                      if (ii==byteI)
                                      {          
                                            bitMod=255>>(((-ii2-1)&7)*((i2+7)>ii2));
                                            bitMod&=255<<((i2)&7);
                                            cMask[byteRef]=(255&bitMod);
                                      }     
                                      else
                                      {
                                            if (ii==byteII)
                                            {
                                               bitMod=255>>((-ii2-1)&7);
                                               cMask[byteRef]=(255&bitMod);
                                            }
                                            else  
                                            cMask[byteRef]=(255);
                                      }
                                  }
                              }  
                              break;
                  }
       }
       ~collCustom()
       {
       delete[] cMask;
       }
};
/*
struct collRectangle {
       unsigned short xDim, yDim;//X and Y dimensions in pixels
       int xOff, yOff;//X and Y offsets
       float xOrigin, yOrigin;//Point of rotation
       collRectangle(unsigned short axDim, unsigned short ayDim, int axOff=0, int ayOff=0, float axOrigin=0, float ayOrigin=0)
       {
                  xDim = axDim;
                  yDim = ayDim;
                  xOff = axOff;
                  yOff = ayOff;
                  xOrigin = axOrigin;
                  yOrigin = ayOrigin;
       }
};
struct collEllipse {
       unsigned short xDim, yDim;//X and Y dimensions in pixels
       int xOff, yOff;//X and Y offsets 
       collEllipse(unsigned short axDim, unsigned short ayDim, int axOff=0, int ayOff=0, float acAngle=0, float axOrigin=0, float ayOrigin=0)
       {
                  xDim = axDim;
                  yDim = ayDim;
                  xOff = axOff;
                  yOff = ayOff;
       }
};
struct collTriangle {
       unsigned short x1,y1,x2,y2,x3,y3;   
       float xOrigin, yOrigin;//Point of rotation
       collTriangle(unsigned short aX1, unsigned short aY1, unsigned short aX2, unsigned short aY2,unsigned short aX3, unsigned short aY3, float acAngle=0, float axOrigin=0, float ayOrigin=0)
       {
                  x1=aX1;
                  x2=aX2;
                  x3=aX3;
                  y1=aY1;
                  y2=aY2;
                  y3=aY3;
                  xOrigin = axOrigin;
                  yOrigin = ayOrigin;
       }
};*/
struct collLinelist{
            int yOff;
            std::vector<float> x1List;
            std::vector<float> x2List;
            float xOrigin, yOrigin;//Point of rotation
            float xFocus, yFocus;//Point of scaling
            collLinelist()
            {
            yOff=0;
            xOrigin=0;
            yOrigin=0;
            xFocus=0;
            yFocus=0;
            }
            collLinelist(float xOrigin, float yOrigin, float xFocus, float yFocus)
            {
            yOff=0;
            xOrigin=0;
            yOrigin=0;
            xFocus=0;
            yFocus=0;
            }
            };
