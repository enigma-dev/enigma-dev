/** Copyright (C) 2008-2013 Josh Ventura
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

#include "OpenGLHeaders.h"
#if PRIMBUFFER
GLenum __primitivetype[PRIMDEPTH2];
int __primitivelength[PRIMDEPTH2];
float __primitivecolor[PRIMBUFFER][PRIMDEPTH2][4];
float __primitivexy[PRIMBUFFER][PRIMDEPTH2][2];
int __currentpcount[PRIMDEPTH2];
int __currentpdepth;
#endif

//namespace enigma{extern void untexture();}
namespace enigma{extern unsigned bound_texture;}
#define untexture() if(enigma::bound_texture) glBindTexture(GL_TEXTURE_2D,enigma::bound_texture=0);

GLenum ptypes_by_id[16] = { 
  GL_POINTS, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, 
  GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_LINE_LOOP, GL_TRIANGLE_STRIP,
  GL_TRIANGLE_STRIP, GL_TRIANGLE_STRIP, 
  
  //These are padding.
  GL_POINTS, GL_POINTS, GL_POINTS, GL_POINTS, GL_POINTS
}; //OPENGLES replaced GL_QUADS, GL_QUAD_STRIP, GL_POLYGON, with GL_TRIANGLE_STRIP's

void draw_set_primitive_aa(bool enable, int quality)
{
    if (enable==1){
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, quality);
    }else{
        glDisable(GL_LINE_SMOOTH);
    }
}

int draw_primitive_begin(int kind)
{
	untexture();
	GLenum dink = ptypes_by_id[ kind & 15 ];
	#if !PRIMBUFFER
	  //glBegin(kind); OPENGLES
	#else
    if(++__currentpdepth>PRIMDEPTH2)
    {
      show_error("Max open primitive count exceeded. Disable the limit via the buffer option, or increase buffer depth",0);
      return -1;
    }
    __currentpcount[__currentpdepth]= 0;
    __primitivetype[__currentpdepth]= dink;
  #endif
  return 0;
}


int draw_primitive_begin_texture(int kind, unsigned tex)
{
    if (enigma::bound_texture != tex)
        glBindTexture(GL_TEXTURE_2D, enigma::bound_texture = tex);
	GLenum dink = ptypes_by_id[ kind & 15 ];
#if !PRIMBUFFER
   // glBegin(kind);
#else
    if(++__currentpdepth>PRIMDEPTH2)
    {
        show_error("Max open primitive count exceeded. Disable the limit via the buffer option, or increase buffer depth",0);
        return -1;
    }
    __currentpcount[__currentpdepth]= 0;
    __primitivetype[__currentpdepth]= dink;
#endif
    return 0;
}

int draw_vertex(gs_scalar x, gs_scalar y)
{
	#if !PRIMBUFFER
	//glVertex2f(x,y); OPENGLES
	#else
    int pco=__currentpcount[__currentpdepth]++;
    glGetFloatv(GL_CURRENT_COLOR,__primitivecolor[pco][__currentpdepth]);
    untexture();
    __primitivexy[pco][__currentpdepth][0]=x;
    __primitivexy[pco][__currentpdepth][1]=y;
    
    if(pco+1>PRIMBUFFER) show_error("Max point count exceeded",0);
	#endif
	return 0;
}

int draw_vertex_color(gs_scalar x, gs_scalar y, int color, float alpha)
{
	unsigned int col=color;
	#if !PRIMBUFFER
	//glPushAttrib(GL_CURRENT_BIT); OPENGLES
	untexture();
	glColor4f(
		(col&0xFF)/255.0,
		((col&0xFF00)>>8)/255.0,
		((col&0xFF0000)>>16)/255.0,
		alpha);
	//glVertex2f(x,y); OPENGLES
	//glPopAttrib(); OPENGLES
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

int draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty)
{
#if !PRIMBUFFER
   /* glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    glTexCoord2f(tx,ty);
    glVertex2f(x,y);
    glPopAttrib();*/
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
int draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha)
{
#if !PRIMBUFFER
   /* glPushAttrib(GL_CURRENT_BIT);
    glColor4f(
              (col&0xFF)/255.0,
              ((col&0xFF00)>>8)/255.0,
              ((col&0xFF0000)>>16)/255.0,
              alpha);
    glTexCoord2f(tx,ty);
    glVertex2f(x,y);
    glPopAttrib();*/
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

int draw_primitive_end()
{
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
	//glEnd(); OPENGLES
	return 0;
}
