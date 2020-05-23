/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#import "subclass.h"
#import <Cocoa/Cocoa.h>

CGWindowID cocoa_wid = kCGNullWindowID;
CGWindowID cocoa_pwid = kCGNullWindowID;

@implementation NSWindow(subclass)

- (void)setChildWindowWithNumber:(CGWindowID)wid {
  [[NSNotificationCenter defaultCenter] addObserver:self
    selector:@selector(windowDidBecomeKey:)
    name:NSWindowDidUpdateNotification object:self];
  cocoa_pwid = [self windowNumber]; cocoa_wid = wid;
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
  if (cocoa_wid_exists(cocoa_pwid) && cocoa_wid_exists(cocoa_wid)) {
    [self orderWindow:NSWindowBelow relativeTo:cocoa_wid];
  } else {
    cocoa_wid = kCGNullWindowID;
    cocoa_pwid = kCGNullWindowID;
  }
}

@end
