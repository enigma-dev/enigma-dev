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

#import "EnigmaView.h"
#include "CocoaFunctions.h"




@implementation EnigmaView


- (id)initWithFrame:(NSRect) frameRect
{
    NSOpenGLPixelFormat *nsglFormat;
    NSOpenGLPixelFormatAttribute attr[] = 
	{
        NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFAColorSize, BITS_PER_PIXEL,
		NSOpenGLPFADepthSize, DEPTH_SIZE,
        0 
	};
	
    [self setPostsFrameChangedNotifications: YES];
	
    
    nsglFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];
	
    
    if(!nsglFormat) { NSLog(@"Invalid NSOPENGLPixelFormat!"); return nil; }
	
    self = [super initWithFrame:frameRect pixelFormat:nsglFormat];
    [nsglFormat release];
    
    
    if(!self) { NSLog(@"Error self is nil!"); return nil; }
	
    [[self openGLContext] makeCurrentContext];
	
    first=YES;
    return self;
}


- (void)awakeFromNib
{
    NSLog(@"awakeFromNib");
    time = [ [NSTimer scheduledTimerWithTimeInterval: DEFAULT_TIME_INTERVAL
											  target:self selector:@selector(drawFrame) userInfo:nil repeats:YES]
			retain];
    
    [[NSRunLoop currentRunLoop] addTimer: time forMode: NSEventTrackingRunLoopMode];
    
    [[NSRunLoop currentRunLoop] addTimer: time forMode: NSModalPanelRunLoopMode];
	
	//init enigma
	init();
    
}

- (void)drawFrame
{
    if( first )
    {
        first = NO; 
        if ([[NSApp keyWindow] makeFirstResponder:self] ) 
            NSLog( @"self apparently made first responder" );
        else 
            NSLog( @"self is not first responder"); 
    }
	
	loopy();
    
    [[self openGLContext] flushBuffer];
	
}


- (BOOL)acceptsFirstResponder
{
    return YES;
}
- (BOOL)becomeFirstResponder
{
    return YES;
}

- (void)keyDown:(NSEvent *)theEvent
{
    NSLog( @"Key Down" );
}
// handle mouse up events (left mouse button)
- (void)mouseUp:(NSEvent *)theEvent
{
    NSPoint pt = [theEvent locationInWindow];
    NSLog(@"EVENT %f %f", pt.x, pt.y);
}
// handle mouse movement events (left mouse button)
- (void)mouseMoved:(NSEvent *)theEvent
{
    NSPoint pt = [theEvent locationInWindow];
    NSLog(@"EVENT %f %f", pt.x, pt.y);
}
// handle mouse up events (right mouse button)
- (void)rightMouseUp:(NSEvent *)theEvent
{
    NSLog( @"Mouse R up");
}
// handle mouse up events (other mouse button)
- (void)otherMouseUp:(NSEvent *)theEvent
{
    NSLog( @"Mouse O up");
}

@end