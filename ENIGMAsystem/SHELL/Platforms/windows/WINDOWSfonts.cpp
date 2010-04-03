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

/***********************************************

int draw_text(double x,double y,std::string text)
double string_width(char* text)
double string_height(char* text)

************************************************/

#include <string>
#include <gl/gl.h>
#include <windows.h>

int show_error(std::string errortext,double fatal);

namespace enigma
{
    HDC fontDC;
    struct font
    {
    	GLYPHMETRICSFLOAT* metrics;
    	GLuint listBase;
    	int size;
    };
    font* currentFont = NULL;
    
    void draw_set_font_struct(font* f)
    {
    	currentFont = f;
    }
    
    extern HDC window_hDC;
    
    //FIXME: Serp thought this should be a macro since only two files use it on Linux. Now it looks filthy.
    extern unsigned cur_bou_tha_noo_sho_eve_cha_eve;
    #define untexture() if(enigma::cur_bou_tha_noo_sho_eve_cha_eve) glBindTexture(GL_TEXTURE_2D,enigma::cur_bou_tha_noo_sho_eve_cha_eve=0);
    
    font* font_add_struct(const char* name,int size,int bold,int italic,int underline,int strike,int first,int last) 
    {
        font* newFont = new font;
        int count=last-first+1;
        
        newFont->metrics = new GLYPHMETRICSFLOAT[count];
        newFont->listBase = glGenLists(count);
        
        /*
        int nHeight,	            // logical height of font 
        int nWidth,	                // logical average character width 
        int nEscapement,	        // angle of escapement 
        int nOrientation,	        // base-line orientation angle 
        int fnWeight,	            // font weight 
        DWORD fdwItalic,	        // italic attribute flag 
        DWORD fdwUnderline,	        // underline attribute flag 
        DWORD fdwStrikeOut,	        // strikeout attribute flag 
        DWORD fdwCharSet,	        // character set identifier 
        DWORD fdwOutputPrecision,   // output precision 
        DWORD fdwClipPrecision,	    // clipping precision 
        DWORD fdwQuality,	        // output quality 
        DWORD fdwPitchAndFamily,    // pitch and family ;;lolwut?
        LPCTSTR lpszFace            // pointer to typeface name string 
        
        */
        HFONT hfont = CreateFont(-MulDiv(size, GetDeviceCaps(fontDC, LOGPIXELSY), 72),0,0,0,FW_REGULAR*(!bold)+FW_BOLD*bold,italic,underline,strike,ANSI_CHARSET,OUT_TT_PRECIS,
         CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_DONTCARE|DEFAULT_PITCH,name);
        
        SelectObject(fontDC, hfont);
        /*
        HDC  hdc,
        DWORD  first,
        DWORD  count,
        DWORD  listBase,
        FLOAT  deviation,
        FLOAT  extrusion,
        int  format,
        LPGLYPHMETRICSFLOAT lpgmf 
        */
    	wglUseFontOutlines(fontDC, first, count, newFont->listBase, 0.0, 0.0, WGL_FONT_POLYGONS, newFont->metrics);
    	newFont->size=size;
    	return newFont;
    }
    
    void init_fonts()
    {
         fontDC = CreateCompatibleDC(window_hDC);	// Use this for putting fonts into
         enigma::draw_set_font_struct(font_add_struct("Arial",14,0,0,0,0,0,255));
         if (currentFont==NULL) { show_error("Failed to load font Arial; I can't see how it would not be supported by the system.",0); }
    }
}

int draw_text(double x,double y,std::string text)
{
    untexture();
    
    if (enigma::currentFont==NULL) return -1;
    
    glPushMatrix();
    glPushAttrib(GL_POLYGON_BIT|GL_TRANSFORM_BIT);
    
    glTranslated(x,y,0);
    glScaled(enigma::currentFont->size,-enigma::currentFont->size,enigma::currentFont->size);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glListBase(enigma::currentFont->listBase);
    glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
    
    glTranslated(x,y,0);
    glListBase(enigma::currentFont->listBase);
    glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
    glLoadIdentity();
    
    glPopAttrib();
    glPopMatrix();
    
    return 0;
}

double string_width(char* text)
{
    char* p = text;
    double length = 0;
    
    for( ; *p; ++p)
       length += enigma::currentFont->metrics[*p].gmfCellIncX;
    
    return length;
}

double string_height(char* text)
{
    char* p = text;
    double height = 0;
    
    for( ; *p; ++p) {
       double newHeight = enigma::currentFont->metrics[*p].gmfCellIncY;

       if(newHeight > height)
	       height = newHeight;
    }
    
    return height;
}
