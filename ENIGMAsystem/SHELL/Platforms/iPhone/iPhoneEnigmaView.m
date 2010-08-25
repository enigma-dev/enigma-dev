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
#import "iPhoneEnigmaView.h"
#include "CocoaFunctions.h"

@implementation iPhoneEnigmaView


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
	[self initOpenGL];
    return self;
}

- (id)initWithCoder:(NSCoder*)coder {
    if (self = [super initWithCoder:coder]) {
        // Initialization code
    }
	[self initOpenGL];
    return self;
}


- (void)drawRect:(CGRect)rect {
    // Drawing code
}


-(BOOL) initOpenGL
{
	
	context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	if (!context || ![EAGLContext setCurrentContext:context])
	{
		[self release];
		return NO;
	}
	glGenFramebuffersOES(1, &defaultFramebuffer);
	glGenRenderbuffersOES(1, &colorRenderbuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	
	if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		NSLog(@"Failed to make complete framebuffer object %x",glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}
	animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0 /60.0) target:self selector:@selector(render) userInfo:nil repeats:TRUE];
	
	init();
	
	return YES;
}


-(void) render
{
	loopy();

	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
	GLenum err = glGetError();
	if(err)
		NSLog(@"%x OPENGLES error", err);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{ 
} 

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{ 
	//get the first touch from the set 
	UITouch *firstTouch=[[touches allObjects] objectAtIndex:0]; 
	//get the the point inside the view 
	currentTouchPoint=[firstTouch locationInView:self]; 
	NSLog(@"TOUCHED!");
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{ 
	//set the last point to the current touch 
	lastTouchPoint=currentTouchPoint; 
	//get the first touch from the set 
	UITouch *firstTouch=[[touches allObjects] objectAtIndex:0]; 
	//get the touch point inside the view 
	currentTouchPoint=[firstTouch locationInView:self];	 
	
	rotation+=currentTouchPoint.x-lastTouchPoint.x;
	
} 







+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

- (void)dealloc {
	//tear down GL
	if (defaultFramebuffer)
	{
		glDeleteFramebuffersOES(1, &defaultFramebuffer);
		defaultFramebuffer=0;
	}
	if (colorRenderbuffer)
	{
		glDeleteRenderbuffersOES(1, &colorRenderbuffer);
		colorRenderbuffer=0;
	}
	
	//tear down context
	if ([EAGLContext currentContext] == context)
		[EAGLContext setCurrentContext:nil];
	[context release];
	context=nil;
    [super dealloc];
}


@end
