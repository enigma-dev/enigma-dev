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

#include <GL/gl.h>

#define __GETR(x) (((unsigned int)x & 0x0000FF))
#define __GETG(x) (((unsigned int)x & 0x00FF00) >> 8)
#define __GETB(x) (((unsigned int)x & 0xFF0000) >> 16)

extern double min(double value1, double value2), max(double value1,double value2);

namespace enigma
{  extern double currentcolor[4];  }




int draw_clear(double color)
{
    color=(int)color;
    
    glClearColor(__GETR(color)/255.0,__GETG(color)/255.0,__GETB(color)/255.0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
}
int draw_clear_alpha(double color, double alpha)
{
    color=(int)color;
    glClearColor(__GETR(color)/255.0,__GETG(color)/255.0,__GETB(color)/255.0,(((int)(alpha*255))&255)/255.0);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
}



int draw_set_color(double col)
{
     int color=((int)col)&16777215;
     enigma::currentcolor[0]=__GETR(color)/255.0;
     enigma::currentcolor[1]=__GETG(color)/255.0;
     enigma::currentcolor[2]=__GETB(color)/255.0;
     glColor4f(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
     return 0;
}
int draw_set_color_rgb(double red, double green, double blue)
{
     enigma::currentcolor[0]=(((int)red)&255)/255.0;
     enigma::currentcolor[1]=(((int)green)&255)/255.0;
     enigma::currentcolor[2]=(((int)blue)&255)/255.0;
     
     glColor4f(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
     return 0;
}
int draw_set_alpha(double alpha)
{
    enigma::currentcolor[3]=(((int)(alpha*255))&255)/255.0; 
    glColor4f(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
    return 0;
}
int draw_set_color_rgba(double red, double green, double blue, double alpha)
{ 
     enigma::currentcolor[0]=(((int)red)&255)/255.0;
     enigma::currentcolor[1]=(((int)green)&255)/255.0;
     enigma::currentcolor[2]=(((int)blue)&255)/255.0;
     enigma::currentcolor[3]=(((int)(alpha*255))&255)/255.0;
     glColor4f(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
     return 0;
}




int draw_get_color()
{
    return (int)((enigma::currentcolor[0]*255)+(enigma::currentcolor[1]*255)*0xFF+(enigma::currentcolor[2]*255)*0xFFFF);
}




int make_color(double red, double green, double blue)
{
     int r=((int)red)&255;
     int g=((int)green)&255;
     int b=((int)blue)&255;
     
     return (int) (r + g*0xFF + b*0xFFFF);
}


int make_color_rgb(double red, double green, double blue)
{
     int r=((int)red)&0xFF;
     int g=((int)green)&0xFF;
     int b=((int)blue)&0xFF;
     
     return (int) (r + g*0xFF + b*0xFFFF);
}







int color_get_red(double color)
{
    int c=(int)color;
    return __GETR(c);
}
int color_get_green(double color)
{
     int c=(int)color;
     return __GETG(c);
}
int color_get_blue(double color)
{
     int c=(int)color;
     return __GETB(c);
}






double color_get_hue(double color)
{
    int c=(int)color;
    
    double r=__GETR(c),
           g=__GETG(c),
           b=__GETB(c);
    
    double cmpmin, cmpmax, delta;

	cmpmin = r; if (g<cmpmin) cmpmin=g; if (b<cmpmin) cmpmin=b;
	cmpmax = r; if (g>cmpmax) cmpmax=g; if (b>cmpmax) cmpmax=b;

	delta = cmpmax - cmpmin;

	if( cmpmax == 0 )
	{
		return 0;
	}
	
	double h;
	
	if (r==cmpmax)
	  h= (g-b)/delta;
	else if(g==cmpmax)
	  h= 2+(b-r)/delta;
	else
	  h= 4+(r-g)/delta;

	h*=60; if (h<0) h+=360;
	
	return h;
}
double color_get_value(double color)
{
    int c=(int)color;
    
    double r=__GETR(c),
           g=__GETG(c),
           b=__GETB(c);
    
    if (r>=g and r>=b)
	   return r;
	if (g>=r and g>=b)
	   return g;
	if (b>=r and b>=g)
	   return b;
    
    return -1;
}
double color_get_saturation(double color)
{
    int c=(int)color;
    
    double r=__GETR(c),
           g=__GETG(c),
           b=__GETB(c);
    
    double cmpmin, cmpmax, delta;

	cmpmin=r; if (g<cmpmin) cmpmin=g; if (b<cmpmin) cmpmin=b;
	cmpmax=r; if (g>cmpmax) cmpmax=g; if (b>cmpmax) cmpmax=b;

	delta=cmpmax - cmpmin;

	if(cmpmax != 0)
	return delta/cmpmax;
	else 
    {
		return 0;
	}
}



int make_color_hsv(double hue,double saturation,double value)
{
double h=((int)hue)&255,s=((int)saturation)&255,v=((int)value)&255;
double red,green,blue;

red = min(255.0,max(510-min(h,255-h)/42.5*255.0,0));
green = min(255.0,max(510-max(85-h,h-85)/42.5*255.0,0));
blue = min(255.0,max(510-max(170-h,h-170)/42.5*255.0,0));

red = red/255.0*v;
green = green/255.0*v;
blue = blue/255.0*v;

red += (v-red)*(1-s/255.0);
green += (v-green)*(1-s/255.0);
blue += (v-blue)*(1-s/255.0);

red=(int)  red;
green=(int)green;
blue=(int) blue;
if (red<0) red=0;
if (green<0) green=0;
if (blue<0) blue=0;

return (int)(red+ 256*green+ 65536*blue +.5);
}


#undef __GETR
#undef __GETG
#undef __GETB
