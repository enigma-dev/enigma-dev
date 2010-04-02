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

/**Some color functions********************************************************\

c_aqua, c_black, c_blue, c_dkgray, c_fuchsia, c_gray, c_green, c_lime, c_ltgray,
c_maroon, c_navy, c_olive, c_purple, c_red, c_silver ,c_teal, c_white, c_yellow

int draw_clear(double color)
int draw_clear_alpha(double color, double alpha)
int draw_set_color(double col)
int draw_set_color_rgb(double red, double green, double blue)
int draw_set_alpha(double alpha)
int draw_set_color_rgba(double red, double green, double blue, double alpha)
int draw_get_color()
int make_color_rgb(double red, double green, double blue)
int color_get_red(double color)
int color_get_green(double color)
int color_get_blue(double color)
double color_get_hue(double color)
double color_get_value(double color)
double color_get_saturation(double color)
int make_color_hsv(double hue,double saturation,double value)

\******************************************************************************/


#define c_aqua      16776960
#define c_black     0
#define c_blue      16711680
#define c_dkgray    4210752
#define c_fuchsia   16711935
#define c_gray      8421504
#define c_green     32768
#define c_lime      65280
#define c_ltgray    12632256
#define c_maroon    128
#define c_navy      8388608
#define c_olive     32896
#define c_purple    8388736
#define c_red       255
#define c_silver    12632256
#define c_teal      8421376
#define c_white     16777215
#define c_yellow    65535



int draw_clear(double color)
{
    color=(int)enigma::negmod(color,16777215);
    
    glClearColor(__GETR(color)/255.0,__GETG(color)/255.0,__GETB(color)/255.0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
}
int draw_clear_alpha(double color, double alpha)
{
    color=(int)enigma::negmod(color,16777215);
    glClearColor(__GETR(color)/255.0,__GETG(color)/255.0,__GETB(color)/255.0,enigma::negmod(alpha,255)/255.0);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
}



int draw_set_color(double col)
{
     int color=(int)enigma::negmod((int)col,16777215);
     enigma::currentcolor[0]=__GETR(color)/255.0;
     enigma::currentcolor[1]=__GETG(color)/255.0;
     enigma::currentcolor[2]=__GETB(color)/255.0;
     glColor4f(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
     return 0;
}
int draw_set_color_rgb(double red, double green, double blue)
{
     enigma::currentcolor[0]=enigma::negmod(red,255)/255.0;
     enigma::currentcolor[1]=enigma::negmod(green,255)/255.0;
     enigma::currentcolor[2]=enigma::negmod(blue,255)/255.0;
     
     glColor4f(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
     return 0;
}
int draw_set_alpha(double alpha)
{
    enigma::currentcolor[3]=enigma::negmod(alpha,1); //yeah, don't divide by 255
    glColor4f(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
    return 0;
}
int draw_set_color_rgba(double red, double green, double blue, double alpha)
{ 
     enigma::currentcolor[0]=enigma::negmod(red,255)/255.0;
     enigma::currentcolor[1]=enigma::negmod(green,255)/255.0;
     enigma::currentcolor[2]=enigma::negmod(blue,255)/255.0;
     enigma::currentcolor[3]=enigma::negmod(alpha,1); //yeah, don't divide by 255
     glColor4f(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
     return 0;
}




int draw_get_color()
{
    return (int)((enigma::currentcolor[0]*255)+(enigma::currentcolor[1]*255)*255+(enigma::currentcolor[2]*255)*255*255);
}





int make_color_rgb(double red, double green, double blue)
{
     int r=(int)enigma::negmod(red,255);
     int g=(int)enigma::negmod(green,255);
     int b=(int)enigma::negmod(blue,255);
     
     return (int) (r + g*256 + b*65536);
}







int color_get_red(double color)
{
    int c=(int)enigma::negmod(color,16777215);
    return __GETR(c);
}
int color_get_green(double color)
{
     int c=(int)enigma::negmod(color,16777215);
     return __GETG(c);
}
int color_get_blue(double color)
{
     int c=(int)enigma::negmod(color,16777215);
     return __GETB(c);
}






double color_get_hue(double color)
{
    int c=(int)enigma::negmod(color,16777215);
    
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
    int c=(int)enigma::negmod(color,16777215);
    
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
    int c=(int)enigma::negmod(color,16777215);
    
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
double h=enigma::negmod(hue,255),s=enigma::negmod(saturation,255),v=enigma::negmod(value,255);
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
