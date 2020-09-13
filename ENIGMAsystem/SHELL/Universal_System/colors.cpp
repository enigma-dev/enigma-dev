#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include <algorithm>
#include <cmath>

using std::max;
using std::min;
using std::fabs;

static inline int bclamp(int x){return x > 255 ? 255 : x < 0 ? 0 : x;}

namespace enigma_user {

int color_get_red  (int c){return COL_GET_R(c);}
int color_get_green(int c){return COL_GET_G(c);}
int color_get_blue (int c){return COL_GET_B(c);}

int color_get_hue(int c)
{
  int r = COL_GET_R(c),g = COL_GET_G(c),b = COL_GET_B(c);
  int cmpmax = r>g ? (r>b?r:b) : (g>b?g:b);
  if(!cmpmax) return 0;

  double cmpdel = cmpmax - (r<g ? (r<b?r:b) : (g<b?g:b)); //Maximum difference
  double h = (r == cmpmax ? (g-b)/cmpdel : (g==cmpmax ? 2-(r-g)/cmpdel : 4+(r-g)/cmpdel));
  return int((h<0 ? h+6 : h) * 42.5); //42.5 = 60/360*255
}
int color_get_value(int c)
{
  int r = COL_GET_R(c), g = COL_GET_G(c), b = COL_GET_B(c);
  return r>g ? (r>b?r:b) : (g>b?g:b);
}
int color_get_saturation(int color)
{
  int r = COL_GET_R(color), g = COL_GET_G(color), b = COL_GET_B(color);
  int cmpmax = r>g  ?  (r>b ? r : b)  :  (g>b ? g : b);
  return cmpmax  ?  255 - int(255 * (r<g ? (r<b?r:b) : (g<b?g:b)) / double(cmpmax))  :  0;
}

int make_color_rgb(unsigned char r, unsigned char g, unsigned char b){
  return r | (g << 8) | (b << 16);
}

int make_color_rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
  return r | (g << 8) | (b << 16) | (a << 24);
}

int make_color_hsv(int hue,int saturation,int value)
{
  int h = hue&255, s = saturation&255,v = value&255;
  double vf = v; vf /= 255.0;
  double
    red   = bclamp(510 - min(h,     255-h) * 6) * vf,
    green = bclamp(510 - max(85-h,   h-85) * 6) * vf,
    blue  = bclamp(510 - max(170-h, h-170) * 6) * vf;

  red   += (v-red)   * (1 - s/255.0);
  green += (v-green) * (1 - s/255.0);
  blue  += (v-blue)  * (1 - s/255.0);

  int redr   = int(red);
  int greenr = int(green);
  int bluer  = int(blue);

  return (redr>0 ? redr : 0) | (greenr>0 ? (greenr<<8) : 0) | (bluer>0 ? (bluer<<16) : 0);
}

int merge_color(int c1,int c2,double amount)
{
  amount = amount > 1 ? 1 : (amount < 0 ? 0 : amount);
  return (unsigned char)(fabs(COL_GET_R(c1)+(COL_GET_R(c2)-COL_GET_R(c1))*amount))
  |      (unsigned char)(fabs(COL_GET_G(c1)+(COL_GET_G(c2)-COL_GET_G(c1))*amount))<<8
  |      (unsigned char)(fabs(COL_GET_B(c1)+(COL_GET_B(c2)-COL_GET_B(c1))*amount))<<16;
}

}
