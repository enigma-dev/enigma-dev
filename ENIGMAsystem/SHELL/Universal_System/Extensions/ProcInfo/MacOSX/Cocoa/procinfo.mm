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

// TODO: create helper functions for everything below this line...

bool cocoa_wid_exists(CGWindowID wid) {
  bool result = false;
  const CGWindowLevel kScreensaverWindowLevel = CGWindowLevelForKey(kCGScreenSaverWindowLevelKey);
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListExcludeDesktopElements |
  kCGWindowListOptionIncludingWindow, (CGWindowID)wid);
  CFIndex windowCount = 0;
  if ((windowCount = CFArrayGetCount(windowArray))) {
    for (CFIndex i = 0; i < windowCount; i++) {
      NSDictionary *windowInfoDictionary =
      (__bridge NSDictionary *)((CFDictionaryRef)CFArrayGetValueAtIndex(windowArray, i));
      NSNumber *ownerPID = (NSNumber *)(windowInfoDictionary[(id)kCGWindowOwnerPID]);
      NSNumber *level = (NSNumber *)(windowInfoDictionary[(id)kCGWindowLayer]);
      if (level.integerValue < kScreensaverWindowLevel) {
        NSNumber *windowID = windowInfoDictionary[(id)kCGWindowNumber];
        if (wid == windowID.integerValue) {
          result = true;
          break;
        }
      }
    }
  }
  CFRelease(windowArray);
  return result;
}

pid_t cocoa_pid_from_wid(CGWindowID wid) {
  pid_t pid;
  const CGWindowLevel kScreensaverWindowLevel = CGWindowLevelForKey(kCGScreenSaverWindowLevelKey);
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListExcludeDesktopElements | 
  kCGWindowListOptionIncludingWindow, (CGWindowID)wid);
  CFIndex windowCount = 0;
  if ((windowCount = CFArrayGetCount(windowArray))) {
    for (CFIndex i = 0; i < windowCount; i++) {
      NSDictionary *windowInfoDictionary = 
      (__bridge NSDictionary *)((CFDictionaryRef)CFArrayGetValueAtIndex(windowArray, i));
      NSNumber *ownerPID = (NSNumber *)(windowInfoDictionary[(id)kCGWindowOwnerPID]);
      NSNumber *level = (NSNumber *)(windowInfoDictionary[(id)kCGWindowLayer]);
      if (level.integerValue < kScreensaverWindowLevel) {
        NSNumber *windowID = windowInfoDictionary[(id)kCGWindowNumber];
        if (wid == windowID.integerValue) {
          pid = ownerPID.integerValue;
          break;
        }
      }
    }
  }
  CFRelease(windowArray);
  return pid;
}

unsigned long cocoa_get_wid_or_pid(bool wid) {
  unsigned long result;
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
      if (level.integerValue == 0) {
        NSNumber *windowID = windowInfoDictionary[(id)kCGWindowNumber];
        result = wid ? windowID.integerValue : ownerPID.integerValue;
        break;
      }
    }
  }
  CFRelease(windowArray);
  return result;
}

void cocoa_wid_to_top(CGWindowID wid) {
  CFIndex appCount = [[[NSWorkspace sharedWorkspace] runningApplications] count];
  for (CFIndex i = 0; i < appCount; i++) {
    if (cocoa_pid_from_wid(wid) == [[[[NSWorkspace sharedWorkspace] runningApplications] objectAtIndex:i] processIdentifier]) {
      NSRunningApplication *appWithPID = [[[NSWorkspace sharedWorkspace] runningApplications] objectAtIndex:i];
      [appWithPID activateWithOptions:NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps];
      break;
    }
  }
}
