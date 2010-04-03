/********************************************************************************\
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
\********************************************************************************/
bool collbitGet(collCustom& aColl,unsigned short aX,unsigned short aY){//Does not check for bounds
      return (aColl.cMask[(aX>>3)+aY*(aColl.byteDim)]&(1<<(aX&7)))!=0;
      }

bool collPoint(collLinelist& aColl, float aX, float aY)//checks for bounds
{
     aY-=aColl.yOff;
     aY=floor(aY);
     if (aY<0)
     {
     return 0;
     }
     if (aY>=aColl.x1List.size())
     {
     return 0;
     }
     return ((aX>=aColl.x1List[(int)aY])&&(aX<aColl.x2List[(int)aY]));
}

bool collCheck(collCustom& aColl, double aX, double aY, double bX, double bY, double aAngle, double axScale, double ayScale)
{     
     if (axScale==0)
     return 0;
     if (ayScale==0)
     return 0;    
     bX-=aX-aColl.xOff;
     bY-=aY-aColl.yOff;   
     if (aAngle==0)
     {     
     if ((axScale!=1)||(ayScale!=1))
     {
     bX=(bX-aColl.xFocus)/axScale+aColl.xFocus;
     bY=(bY-aColl.yFocus)/ayScale+aColl.yFocus; 
     }
     if ((bX<0)||(bY<0))
        return 0;
     if ((bX<aColl.xDim)&&(bY<aColl.yDim))
        return (aColl.cMask[((unsigned short)(bX)>>3)+(unsigned short)(bY)*(aColl.byteDim)]&(1<<((unsigned short)(bX)&7)))!=0;//collbitGet();
     else
         return 0;//out of bounds, therefore no collision here
     }
     else
     {
     double aSin, aCos, xSOrigin, ySOrigin;
     aAngle/=57.295779513082320876798154814105;
     aSin=sin(aAngle);
     aCos=cos(aAngle);           
     if ((axScale!=1)||(ayScale!=1))
     {
     bX=(bX-aColl.xFocus)/axScale+aColl.xFocus;
     bY=(bY-aColl.yFocus)/ayScale+aColl.yFocus; 
     }
     xSOrigin=(aColl.xOrigin-aColl.xFocus)*axScale+aColl.xFocus;
     ySOrigin=(aColl.yOrigin-aColl.yFocus)*ayScale+aColl.yFocus; 
     aX=(bX-xSOrigin)*aCos - (bY-ySOrigin)*aSin + xSOrigin;
     bY=(bY-ySOrigin)*aCos + (bX-xSOrigin)*aSin + ySOrigin;
     bX=aX; 
     if ((bX<0)||(bY<0))
        return 0;
     if ((bX<aColl.xDim)&&(bY<aColl.yDim))
        return (aColl.cMask[((unsigned short)(bX)>>3)+(unsigned short)(bY)*(aColl.byteDim)]&(1<<((unsigned short)(bX)&7)))!=0;//collbitGet();
     else
         return 0;//out of bounds, therefore no collision here
     }
} 

bool collCheck(collLinelist& aColl, double aX, double aY, double bX, double bY, double aAngle, double axScale, double ayScale)
{     
     if (axScale==0)
     return 0;
     if (ayScale==0)
     return 0;    
     bX-=aX;
     bY-=aY-aColl.yOff;   
     if (aAngle==0)
     {     
     if ((axScale!=1)||(ayScale!=1))
     {
     bX=(bX-aColl.xFocus)/axScale+aColl.xFocus;
     bY=(bY-aColl.yFocus)/ayScale+aColl.yFocus; 
     }
     return collPoint(aColl, (float)bX, (float)bY);
     }
     else
     {
     double aSin, aCos, xSOrigin, ySOrigin;
     aAngle/=57.295779513082320876798154814105;
     aSin=sin(aAngle);
     aCos=cos(aAngle);           
     if ((axScale!=1)||(ayScale!=1))
     {
     bX=(bX-aColl.xFocus)/axScale+aColl.xFocus;
     bY=(bY-aColl.yFocus)/ayScale+aColl.yFocus; 
     }
     xSOrigin=(aColl.xOrigin-aColl.xFocus)*axScale+aColl.xFocus;
     ySOrigin=(aColl.yOrigin-aColl.yFocus)*ayScale+aColl.yFocus; 
     aX=(bX-xSOrigin)*aCos - (bY-ySOrigin)*aSin + xSOrigin;
     bY=(bY-ySOrigin)*aCos + (bX-xSOrigin)*aSin + ySOrigin;
     bX=aX; 
     return collPoint(aColl, (float)bX, (float)bY);
     }
} 
