/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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

#import  <Cocoa/Cocoa.h>
#import  <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

#define BITS_PER_PIXEL          32.0
#define DEPTH_SIZE              32.0
#define DEFAULT_TIME_INTERVAL 	0.001

@interface EnigmaView : NSOpenGLView
{
    
    bool first;
    
    NSWindow *StartingWindow;
    NSTimer  *time;
   
	BOOL opt, ctrl, command, shift;
    NSThread* timerThread;
    NSRunLoop* runLoop;
}

- (id)initWithFrame:(NSRect) frameRect;
- (void) flushOpenGL;
-(void) startTimer;
-(void) startTimerThread;
-(void) terminateEnigma;

@end
