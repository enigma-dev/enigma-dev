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

#import <string.h>

#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>

#import "videoplayer.h"

bool hidden = false;

const char *cocoa_window_get_contentview(const char *window) {
  NSWindow *win = (NSWindow *)strtoull(window, NULL, 10);
  unsigned long long ull = (unsigned long long)[win contentView];
  return [[NSString stringWithFormat:@"%llu", ull] UTF8String];
}

void cocoa_show_cursor() {
  if (hidden) {
    [NSCursor unhide];
    hidden = false;
  }
}

const char *cocoa_prefer_global_windowid(const char *window) {
  unsigned long long ull = strtoull(window, NULL, 10);
  if ([NSApp windowWithWindowNumber:(unsigned long)ull] != nil) {
    return [[NSString stringWithFormat:@"%llu",
      [NSApp windowWithWindowNumber:(unsigned long)ull]] UTF8String];
  }
  return window;
}

void cocoa_process_run_loop(const char *video,
  const char *window, bool close_mouse, bool close_key) {
  [[NSRunLoop currentRunLoop] runUntilDate:
  [NSDate dateWithTimeIntervalSinceNow:0.005]];
  NSPoint mouseLoc; mouseLoc = [NSEvent mouseLocation];
  CGWindowID windowNumber = [NSWindow
    windowNumberAtPoint:mouseLoc belowWindowWithWindowNumber:0];
  NSView *view = (NSView *)strtoull(window, NULL, 10);
  CGWindowID wid = [[view window] windowNumber];
  if (windowNumber == wid) {
    if (!hidden) { [NSCursor hide]; hidden = true; }
    if (close_mouse && ([NSEvent pressedMouseButtons] == 1 << 0 ||
      [NSEvent pressedMouseButtons] == 1 << 1)) {
      video_stop(video);
    }
  } else {
    cocoa_show_cursor();
  }
  NSEvent *keyEvent = [[view window]
    nextEventMatchingMask:(NSEventMaskKeyDown | NSEventMaskLeftMouseDown |
    NSEventMaskRightMouseDown | NSEventMaskMouseMoved)];
  if ([keyEvent type] == 10 && [keyEvent keyCode] == kVK_Escape) {
    video_stop(video);
  }
}

