/********************************************************************************\
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
\********************************************************************************/

/**Primitive functions*********************************************************\

pr_pointlist, pr_linelist, pr_linestrip, pr_trianglelist, pr_trianglestrip,
pr_trianglefan, pr_lineloop, pr_quadlist, pr_quadstrip, pr_polygon

int draw_primitive_begin(double kind)
int draw_vertex(double x, double y)
int draw_vertex_color(double x, double y, double color, double alpha)
int draw_primitive_end()

\******************************************************************************/

#include <GL/gl.h>
#if PRIMBUFFER
GLenum __primitivetype[PRIMDEPTH2];
int __primitivelength[PRIMDEPTH2];
float __primitivecolor[PRIMBUFFER][PRIMDEPTH2][4];
float __primitivexy[PRIMBUFFER][PRIMDEPTH2][2];
int __currentpcount[PRIMDEPTH2];
int __currentpdepth;
#endif

//namespace enigma{extern void untexture();}
namespace enigma{extern unsigned cur_bou_tha_noo_sho_eve_cha_eve;}
#define untexture() if(enigma::cur_bou_tha_noo_sho_eve_cha_eve) glBindTexture(GL_TEXTURE_2D,enigma::cur_bou_tha_noo_sho_eve_cha_eve=0);

int draw_primitive_begin(double dink){
	untexture();
	GLenum kind;
	switch((int)dink){
	case 2:kind=GL_LINES;
	break;case 3:kind=GL_LINE_STRIP;
	break;case 4:kind=GL_TRIANGLES;
	break;case 5:kind=GL_TRIANGLE_STRIP;
	break;case 6:kind=GL_TRIANGLE_FAN;
	break;case 7:kind=GL_LINE_LOOP;
	break;case 8:kind=GL_QUADS;
	break;case 9:kind=GL_QUAD_STRIP;
	break;case 10:kind=GL_POLYGON;
	break;default:kind=GL_POINTS;
	}
	#if !PRIMBUFFER
	glBegin(kind);
	#else
	if(++__currentpdepth>PRIMDEPTH2){
		#if SHOWERRORS
		show_error("Max open primitive count exceeded. Disable the limit via the buffer option, or increase buffer depth",0);
		#endif
		return -1;
     }
     __currentpcount[__currentpdepth]=0;
     __primitivetype[__currentpdepth]=kind;
     #endif
     return 0;
}

int draw_vertex(double x, double y){
	#if !PRIMBUFFER
	glVertex2f(x,y);
	#else
	int pco=__currentpcount[__currentpdepth]++;
	glGetFloatv(GL_CURRENT_COLOR,__primitivecolor[pco][__currentpdepth]);
	untexture();
	__primitivexy[pco][__currentpdepth][0]=x;
	__primitivexy[pco][__currentpdepth][1]=y;
	#if SHOWERRORS
	if(pco+1>PRIMBUFFER) show_error("Max point count exceeded",0);
	#endif
	#endif
	return 0;
}
int draw_vertex_color(double x, double y, int color, double alpha){
	unsigned int col=color;
	#if !PRIMBUFFER
	glPushAttrib(GL_CURRENT_BIT);
	untexture();
	glColor4f(
		(col&0xFF)/255.0,
		((col&0xFF00)>>8)/255.0,
		((col&0xFF0000)>>16)/255.0,
		alpha);
	glVertex2f(x,y);
	glPopAttrib();
	#else
	int pco=__currentpcount[__currentpdepth]++;
	__primitivecolor[pco][__currentpdepth][0]=(col&0xFF)/255.0;
	__primitivecolor[pco][__currentpdepth][1]=((col&0xFF00)>>8)/255.0;
	__primitivecolor[pco][__currentpdepth][2]=((col&0xFF0000)>>16)/255.0;
	__primitivecolor[pco][__currentpdepth][3]=alpha;
	__primitivexy[pco][__currentpdepth][0]=x;
	__primitivexy[pco][__currentpdepth][1]=y;
	#if SHOWERRORS
	if(pco+1>PRIMBUFFER) show_error("Max point count exceeded",0);
	#endif
	#endif
	return 0;
}
int draw_primitive_end(){
	untexture();
	#if PRIMBUFFER
	glPushAttrib(GL_CURRENT_BIT);
	glBegin(__primitivetype[__currentpdepth]);
	for (int DPEinx=0;DPEinx<__currentpcount[__currentpdepth];DPEinx++){
		glColor4fv(__primitivecolor[DPEinx][__currentpdepth]);
		glVertex2fv(__primitivexy[DPEinx][__currentpdepth]);
		__primitivecolor[DPEinx][__currentpdepth][0]=0;
		__primitivecolor[DPEinx][__currentpdepth][1]=0;
		__primitivecolor[DPEinx][__currentpdepth][2]=0;
		__primitivecolor[DPEinx][__currentpdepth][3]=0;
		__primitivexy[DPEinx][__currentpdepth][0]=0;
		__primitivexy[DPEinx][__currentpdepth][1]=0;
	}
	__currentpcount[__currentpdepth]=0;
	__primitivetype[__currentpdepth]=0;
	__primitivelength[__currentpdepth]=0;
	__currentpdepth--;
	glPopAttrib();
	#endif
	glEnd();
	return 0;
}
