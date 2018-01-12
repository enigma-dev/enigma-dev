/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
 **  Copyright (C) 2014 Seth N. Hetu                                             **
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
#import "EnigmaView.h"

EnigmaXcodeAppDelegate* delegate;
NSPoint mouse; 

void cocoa_window_set_fullscreen(bool full) 
{
	if (full) {
		[[delegate enigmaview] enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
	} else {
		[[delegate enigmaview] exitFullScreenModeWithOptions: nil];
	}
}

int cocoa_window_get_fullscreen()
{
	return [[delegate enigmaview] isInFullScreenMode];
}

const char* cocoa_window_get_caption()
{
	return [[[delegate window] title] UTF8String];	
}

int cocoa_get_screen_size(int getWidth)
{
	if (getWidth) {
		return [[NSScreen mainScreen] frame].size.width;
	} else {
		return [[NSScreen mainScreen] frame].size.height;
	}
}

void cocoa_clipboard_set_text(const char* text)
{
  //Get the general pasteboard, clear its contents.
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];

  //Pass an array of objects to write to the pasteboard.
  NSString* objToCopy = [NSString stringWithUTF8String:text];
  NSArray* objectsToCopy = [NSArray arrayWithObject:objToCopy];
  [pasteboard writeObjects:objectsToCopy];
}

char* cocoa_clipboard_get_text()
{
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
  NSArray* classes = [[NSArray alloc] initWithObjects:[NSString class], nil];
  NSDictionary *options = [NSDictionary dictionary];
  NSArray *copiedItems = [pasteboard readObjectsForClasses:classes options:options];
  if (copiedItems!=nil && [copiedItems count]>0) {
    NSString* res1 = [copiedItems firstObject];
    const char* res2 = [res1 cStringUsingEncoding:NSUTF8StringEncoding]; //This will be auto-collected.
    char* res3 = malloc(strlen(res2)+1); //This will be freed by the caller (clipboard_get_text()).
    strcpy(res3, res2);
    return res3;
  }

  return 0;
}

bool cocoa_clipboard_has_text()
{
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
  NSArray* classes = [[NSArray alloc] initWithObjects:[NSString class], nil];
  NSDictionary *options = [NSDictionary dictionary];
  NSArray *copiedItems = [pasteboard readObjectsForClasses:classes options:options];
  return copiedItems!=nil && [copiedItems count]>0;
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
	NSRect rect = NSMakeRect([[delegate window] frame].origin.x,[[delegate window] frame].origin.y,w,h+22);
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
		case 1:  return -(mouse.y-[[delegate window] frame].size.height);
		case 2:  return mouse.x; //window
        case 3: return -(mouse.y-[[delegate window] frame].size.height)-22;
		default: return -1;
	}
	
}

void cocoa_window_set_color(int bgrColor) 
{
	[[delegate window] setBackgroundColor: [NSColor colorWithRed:(bgrColor&0xFF)  green:((bgrColor&0xFF00)>>8)  blue:((bgrColor&0xFF0000)>>16)  alpha:1.0]];
}

void cocoa_window_set_caption(const char* caption)
{
	[[delegate window] setTitle:[NSString stringWithCString:caption length:strlen(caption)]];
}

void cocoa_screen_refresh()
{
    [[delegate enigmaview] flushOpenGL];
    
    //[[[[delegate window] contentView] openGLContext] flushBuffer];
	//[[self openGLContext] flushBuffer];
}

int cocoa_window_set_visible(int visible)
{
	if(visible==1)
	{
		[[delegate window] makeKeyAndOrderFront:nil]; 
		delegate.visible= YES;
	}
	else {
		if ([[delegate window] isVisible]) {
		// Hide 
		[[delegate window] orderOut:nil]; 
			delegate.visible= NO;
		}
	}
	return 0;
}

int cocoa_window_get_visible()
{
	return (delegate.visible) ? 1 : 0;
}

void cocoa_flush_opengl() {
    //[delegate applicationDidUpdate:NULL];
    //[delegate flushOpenGL];
    //[[delegate window] update];
    //[[delegate window] flushWindow];
    //EnigmaView* ev = (EnigmaView*)[[delegate window] contentView];
    //[ev flushOpenGL];
    //contentView
}

int cocoa_window_get_region_width() {
    return getWindowDimension(2);
}
int cocoa_window_get_region_height() {
    return getWindowDimension(3);
}

//This does not appear to work right.
/*const char* cocoa_get_working_directory() {
    NSBundle* bundle = [NSBundle mainBundle];
    return [[bundle bundlePath] UTF8String];
}*/

