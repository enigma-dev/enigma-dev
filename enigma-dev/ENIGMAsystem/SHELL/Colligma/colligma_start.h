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
//#include <cstdlib>
//#include <cmath>
//#include <vector>
//#include <limits>
//constants
const unsigned char fill_rectangle=0, fill_triangle=1, fill_ellipse=2, bm_negate=4;
float maxFloat=std::numeric_limits<float>::max(), minFloat=std::numeric_limits<float>::min();
//fill_rectangle is a 2D rectangular of 1s
//fill_triangle is a 2D polygon fill of the area of a triangle, wherever a point lies inside the triangle it is 1
//fill_ellipse is a stretched circle to fit a 2D rectangle, wherever a point lies inside the stretched circle it is 1

//bm_normal sets the bitMask to the shape and sets everything else to be 0, completely overrides previous linelists
//bm_add sets the bitMask on wherever parts of the shape are on (that is to say, it will never turn set a bit to 0), overlaps previous linelists
//bm_subtract sets the bitMask off wherever parts of the shape are on (will never set a bit to 1), not applicable to line lists
//bm_negate negates the bitmask wherever parts of the shape are on (changes 0 to 1, 1 to 0 wherever the shape is 1), not applicable to linelists
#include "structures.h"
bool collbitNegate(collCustom& aColl,unsigned short,unsigned short); 
bool collbitSet(collCustom& aColl,unsigned short X,unsigned short Y,bool Value=1);                                          
bool collbitGet(collCustom& aColl,unsigned short X,unsigned short Y);
bool collCheck(collCustom& aColl, double aX, double aY, double bX, double bY, double aAngle=0, double axScale=1, double ayScale=1);
bool collCheck(collLinelist& aColl, double aX, double aY, double bX, double bY, double aAngle=0, double axScale=1, double ayScale=1);
bool collPoint(collLinelist& aColl,float X,float Y);
bool collFill(collCustom& aColl, bool Fill);

bool collFill(collCustom& aColl,unsigned char Blend,float aX,float aY,collCustom& bColl,float bX,float bY,double aAngle=0,double bAngle=0);
bool collFill(collCustom& aColl,unsigned char Type, unsigned char Blend,float aX1,float aY1,float aX2,float aY2,float aX3,float aY3, double Angle=0, float xOrigin=0, float yOrigin=0);
bool collFill(collCustom& aColl,unsigned char Blend,float aX,float aY,collLinelist& bColl,float bX,float bY,double Angle);

bool collFill(collLinelist& aColl,unsigned char Type, unsigned char Blend,float aX1,float aY1,float aX2,float aY2,float aX3,float aY3, double Angle=0, float xOrigin=0, float yOrigin=0);
bool collFill(collLinelist& aColl,unsigned char Blend,float aX,float aY,collLinelist& bColl,float bX=0,float bY=0);

bool collCheck(collCustom& aColl,float aX,float aY,collCustom&,float bX,float bY,double aAngle=0,double bAngle=0);
bool collCheck(collCustom& aColl,float aX,float aY,collLinelist& bColl,float bX=0,float bY=0,double Angle=0);

bool collCheck(collLinelist& aColl,float aX,float aY,collLinelist& bColl,float bX=0,float bY=0);
bool collCheck(collLinelist& bColl,float bX,float bY,collCustom& aColl,float aX=0,float aY=0,double Angle=0);

void collResize(collLinelist& aColl,unsigned int newsize)
{    
      aColl.x1List.resize(newsize);
      aColl.x2List.resize(newsize);
}
