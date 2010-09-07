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

#import "WindowFunctions.h"
#import "EnigmaXcodeAppDelegate.h"


EnigmaXcodeAppDelegate* delegate;
NSPoint mouse; 

const char* cocoa_window_get_caption()
{
	return [[[delegate window] title] UTF8String];	
}

int getWindowDimension(int i)
{
	if(i == 0) return [[delegate window] frame].origin.x;
	if(i == 1) return [[delegate window] frame].origin.y;
	if(i == 2) return [[delegate window] frame].size.width;
	if(i == 3) return [[delegate window] frame].size.height-22;
	return 0;
}

void cocoa_window_set_size(unsigned int w,unsigned int h) {
	NSRect rect = NSMakeRect(0,0,w,h+22);
	[[delegate window] setFrame:rect display:YES ];
	
}
void cocoa_window_set_rectangle(int x,int y,int w,int h) {
	NSRect rect = NSMakeRect(x,y,w,h+22);
	[[delegate window] setFrame:rect display:YES ];
}

int getMouse(int i)
{
	switch(i)
	{
		case 0:  return mouse.x; //room
		case 1:  return -(mouse.y-480);
		case 2:  return mouse.x; //window
		case 3:  return  -(mouse.y-480)+22;
		default: return -1;
	}
	
}

void cocoa_window_set_caption(const char* caption)
{
	[[delegate window] setTitle:[NSString stringWithCString:caption length:strlen(caption)]];
}

void cocoa_screen_refresh()
{
	//[[self openGLContext] flushBuffer];
}

int cocoa_window_set_visible(int visible)
{
	if(visible==1)
	{
		/*XMapRaised(disp,win);
		 GLXContext glxc = glXGetCurrentContext();
		 glXMakeCurrent(disp,win,glxc);
		 if(visx != -1 && visy != -1)
		 window_set_position(visx,visy);*/
	}
	else {
		// Hide 
		[[delegate window] orderOut:[delegate window]]; 
	}
	return 0;
}

int cocoa_window_get_visible()
{
	
}

