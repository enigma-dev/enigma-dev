/*
 *  OpenGLHeaders.h
 *  EnigmaXcode
 *
 *  Created by Alasdair Morrison on 12/08/2010.
 *  Copyright 2010 EnigmaDev. All rights reserved.
 *
 */
#include "../API_Switchboard.h"
#if ENIGMA_WS_COCOA !=0
#include <OpenGL/gl.h>	
#elif ENIGMA_WS_IPHONE != 0
#include <OpenGLES/ES1/gl.h>
#else
#include <GL/gl.h>
#endif
