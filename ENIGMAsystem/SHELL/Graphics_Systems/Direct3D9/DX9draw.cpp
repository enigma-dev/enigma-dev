/** Copyright (C) 2013 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <math.h>
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "DX9binding.h"
#include <stdio.h>
#include "Universal_System/roomsystem.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

#include <vector>
using std::vector;

namespace enigma {
  float circleprecision=24;
  extern unsigned char currentcolor[4];
}

namespace enigma_user
{

int draw_get_msaa_maxlevel()
{

}

bool draw_get_msaa_supported()
{

}

void draw_set_msaa_enabled(bool enable)
{

}

void draw_enable_alphablend(bool enable) {
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, enable);
}

bool draw_get_alpha_test() {
	DWORD* enabled;
	d3ddev->GetRenderState(D3DRS_ALPHATESTENABLE, enabled);
	return *enabled;
}

unsigned draw_get_alpha_test_ref_value()
{
	DWORD* val;
	d3ddev->GetRenderState(D3DRS_ALPHAREF, val);
	return *val;
}

void draw_set_alpha_test(bool enable)
{
	d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, enable); 
}

void draw_set_alpha_test_ref_value(unsigned val)
{
	d3ddev->SetRenderState(D3DRS_ALPHAREF, val);
}

void draw_set_line_pattern(unsigned short pattern, int scale)
{

}

void draw_point(gs_scalar x, gs_scalar y)
{

}

void draw_point_color(gs_scalar x, gs_scalar y, int col)
{

}

void draw_line(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2)
{

}

void draw_line_width(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float width)
{

}

void draw_line_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, int c1, int c2)
{

}

void draw_line_width_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float width, int c1, int c2)
{

}

void draw_rectangle(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, bool outline)
{

}

void draw_rectangle_angle(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float angle, bool outline)
{

}

void draw_rectangle_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, int c1, int c2, int c3, int c4, bool outline)
{

}

void draw_set_circle_precision(float pr) {
    enigma::circleprecision = pr<3 ? 3 : pr;
}
float draw_get_circle_precision() {
    return enigma::circleprecision;
}

struct D3DTLVERTEX
{
	float fX;
	float fY;
	float fZ;
};



void draw_circle(gs_scalar x, gs_scalar y, float rad, bool outline)
{
    double pr = 2 * M_PI / enigma::circleprecision;
	vector<float> Circle;
	    d3ddev->SetFVF(D3DFVF_XYZ);
    if(outline)
    {
        for (double i = 0; i <= 2*M_PI; i += pr)
        {
            double xc1=cos(i)*rad,yc1=sin(i)*rad;
			//D3DTLVERTEX v; 
			//v.fX = x+xc1; v.fY = y+yc1; v.fZ = 0;
			//v.fRHW = RHW;
			//v.Color = color;
			//v.fU = U; v.fV = V;
			//Circle.push_back(v);
			
			Circle.push_back(x+xc1); Circle.push_back(y+yc1); Circle.push_back(0);
        }
		d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, enigma::circleprecision, &Circle[0], sizeof(float) * 3);
    }
    else
    {
		//D3DTLVERTEX v; 
		//v.fX = x; v.fY = y; v.fZ = 0;
		//v.fRHW = RHW;
		//v.Color = color;
		//v.fU = U; v.fV = V;
		//Circle.push_back(v);
		Circle.push_back(x); Circle.push_back(y); Circle.push_back(0);
        for (double i = 0; i <= 2*M_PI; i += pr)
        {
            double xc1=cos(i)*rad,yc1=sin(i)*rad;
			//D3DTLVERTEX v; 
			//v.fX = x+xc1; v.fY = y+yc1; v.fZ = 0;
			//v.fRHW = RHW;
			//v.Color = color;
			//v.fU = U; v.fV = V;
			//Circle.push_back(v);
			Circle.push_back(x+xc1); Circle.push_back(y+yc1); Circle.push_back(0);
        }
		d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, enigma::circleprecision, &Circle[0], sizeof(float) * 3);
    }
}

void draw_circle_color(gs_scalar x, gs_scalar y, float rad, int c1, int c2, bool outline)
{

}

void draw_circle_perfect(gs_scalar x, gs_scalar y, float rad, bool outline)
{

}

void draw_circle_color_perfect(gs_scalar x, gs_scalar y, float rad, int c1, int c2, bool outline)
{

}

void draw_ellipse(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, bool outline)
{

}

void draw_ellipse_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, int c1, int c2, bool outline)
{

}

void draw_ellipse_perfect(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, bool outline)
{

}

void draw_triangle(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float x3, float y3, bool outline)
{

}

void draw_triangle_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float x3, float y3, int col1, int col2, int col3, bool outline)
{

}

void draw_roundrect(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,float rad, bool outline)
{

}

void draw_roundrect_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, int col1, int col2, bool outline)
{

}

void draw_arrow(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, gs_scalar arrow_size, gs_scalar line_size, bool outline)
{

}

void draw_button(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, gs_scalar border_width, bool up)
{

}

//Mind that health is 1-100
void draw_healthbar(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float amount, int backcol, int mincol, int maxcol, int dir, bool showback, bool showborder)
{

}

}

//#include <endian.h>
//TODO: Though serprex, the author of the function below, never included endian.h,
//   // Doing so is necessary for the function to work at its peak.
//   // When ENIGMA generates configuration files, one should be included here.

namespace enigma_user
{

int draw_getpixel(int x, int y)
{

}

int draw_mandelbrot(int x, int y, float w, double Zx, double Zy, double Zw, unsigned iter)
{

}

}

