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
#import <sys/types.h>
#import <unistd.h>

CGWindowID cocoa_wid = kCGNullWindowID;
CGWindowID cocoa_pwid = kCGNullWindowID;

// pid of child wid
pid_t cocoa_pid = 0;

// pid of parent wid
pid_t cocoa_ppid = 0;

void subclass_helper(NSWindow *window) {
  if (cocoa_ppid == getpid() && cocoa_wid_exists(cocoa_wid)) {
    [window setCanHide:NO];
    [window orderWindow:NSWindowBelow relativeTo:cocoa_wid];
  } else if (cocoa_pid == getpid() && cocoa_wid_exists(cocoa_pwid)) {
    [window setCanHide:NO];
    [window orderWindow:NSWindowAbove relativeTo:cocoa_pwid];
    cocoa_wid_to_top(cocoa_wid);
  } else {
    [window setCanHide:YES];
    cocoa_wid = kCGNullWindowID;
    cocoa_pwid = kCGNullWindowID;
    cocoa_pid = 0;
    cocoa_ppid = 0;
  }
}

@implementation NSWindow(subclass)

- (void)setChildWindowWithNumber:(CGWindowID)wid {
  [[NSNotificationCenter defaultCenter] addObserver:self
    selector:@selector(windowDidBecomeKey:)
    name:NSWindowDidUpdateNotification object:self];
  [[NSNotificationCenter defaultCenter] addObserver:self
    selector:@selector(windowDidResignKey:)
    name:NSWindowDidUpdateNotification object:self];
  cocoa_pwid = [self windowNumber]; cocoa_wid = wid;
  cocoa_ppid = cocoa_pid_from_wid(cocoa_pwid);
  cocoa_pid = cocoa_pid_from_wid(cocoa_wid);
  [self orderWindow:NSWindowBelow relativeTo:wid];
}

- (void)setParentWindowWithNumber:(CGWindowID)pwid {
  [[NSNotificationCenter defaultCenter] addObserver:self
    selector:@selector(windowDidBecomeKey:)
    name:NSWindowDidUpdateNotification object:self];
  [[NSNotificationCenter defaultCenter] addObserver:self
    selector:@selector(windowDidResignKey:)
    name:NSWindowDidUpdateNotification object:self];
  cocoa_wid = [self windowNumber]; cocoa_pwid = pwid;
  cocoa_ppid = cocoa_pid_from_wid(cocoa_pwid);
  cocoa_pid = cocoa_pid_from_wid(cocoa_wid);
  [self orderWindow:NSWindowAbove relativeTo:pwid];
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
  subclass_helper(self);
}

- (void)windowDidResignKey:(NSNotification *)notification {
  subclass_helper(self);
}

- (void)windowDidUpdate:(NSNotification *)notification {
  subclass_helper(self);
}

@end
