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

#import <Cocoa/Cocoa.h>
#import <sys/types.h>

NSWindow *cocoa_window_from_wid(CGWindowID wid) {
  return [NSApp windowWithWindowNumber:wid];
}

CGWindowID cocoa_wid_from_window(NSWindow *window) {
  return [window windowNumber];
}

const unsigned
RES_BOOL = 0,
RES_WID = 1,
RES_PID = 2;

unsigned long cocoa_helper_function(unsigned long arg, unsigned restype) {
  bool result = 0;
  const CGWindowLevel kScreensaverWindowLevel = CGWindowLevelForKey(kCGScreenSaverWindowLevelKey);
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly |
  kCGWindowListExcludeDesktopElements, kCGNullWindowID);
  CFIndex windowCount = 0;
  if ((windowCount = CFArrayGetCount(windowArray))) {
    for (CFIndex i = 0; i < windowCount; i++) {
      NSDictionary *windowInfoDictionary =
        (__bridge NSDictionary *)((CFDictionaryRef)CFArrayGetValueAtIndex(windowArray, i));
      NSNumber *ownerPID = (NSNumber *)(windowInfoDictionary[(id)kCGWindowOwnerPID]);
      NSNumber *level = (NSNumber *)(windowInfoDictionary[(id)kCGWindowLayer]);
      if (level.integerValue < kScreensaverWindowLevel) {
        NSNumber *windowID = windowInfoDictionary[(id)kCGWindowNumber];
        if (arg == windowID.integerValue) {
          pid_t pid = ownerPID.integerValue;
          if (restype == RES_BOOL) result = 1;
          if (restype == RES_PID) result = pid;
          break;
        }
      }
      if (level.integerValue == 0) {
        if (restype == RES_WID + RES_PID) {
          NSNumber *windowID = windowInfoDictionary[(id)kCGWindowNumber];
          result = arg ? windowID.integerValue : ownerPID.integerValue;
          break;
        }
      }
    }
  }
  CFRelease(windowArray);
  return result;
}

bool cocoa_wid_exists(CGWindowID wid) {
  return cocoa_helper_function(wid, RES_BOOL);
}

pid_t cocoa_pid_from_wid(CGWindowID wid) {
  return cocoa_helper_function(wid, RES_PID);
}

unsigned long cocoa_get_wid_or_pid(bool wid) {
  return cocoa_helper_function(wid, RES_WID + RES_PID);
}

void cocoa_wid_to_top(CGWindowID wid) {
  CFIndex appCount = [[[NSWorkspace sharedWorkspace] runningApplications] count];
  for (CFIndex i = 0; i < appCount; i++) {
    NSWorkspace *sharedWS = [NSWorkspace sharedWorkspace];
    NSArray *runningApps = [sharedWS runningApplications];
    NSRunningApplication *currentApp = [runningApps objectAtIndex:i];
    if (cocoa_pid_from_wid(wid) == [currentApp processIdentifier]) {
      NSRunningApplication *appWithPID = currentApp;
      NSUInteger options = NSApplicationActivateAllWindows;
      options |= NSApplicationActivateIgnoringOtherApps;
      [appWithPID activateWithOptions:options];
      break;
    }
  }
}
