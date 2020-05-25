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

NSWindow *cocoa_window_from_wid(CGWindowID wid) {
  return [NSApp windowWithWindowNumber:wid];
}

CGWindowID cocoa_wid_from_window(NSWindow *window) {
  return [window windowNumber];
}

bool cocoa_wid_exists(CGWindowID wid) {
  bool result = false;
  const CGWindowLevel kScreensaverWindowLevel = CGWindowLevelForKey(kCGScreenSaverWindowLevelKey);
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);
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
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);
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

const char *cocoa_wids_from_pid(pid_t pid) {
  NSString *wids = [[NSString alloc] init];
  const CGWindowLevel kScreensaverWindowLevel = CGWindowLevelForKey(kCGScreenSaverWindowLevelKey);
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);
  CFIndex windowCount = 0;
  if ((windowCount = CFArrayGetCount(windowArray))) {
    for (CFIndex i = 0; i < windowCount; i++) {
      NSDictionary *windowInfoDictionary =
      (__bridge NSDictionary *)((CFDictionaryRef)CFArrayGetValueAtIndex(windowArray, i));
      NSNumber *ownerPID = (NSNumber *)(windowInfoDictionary[(id)kCGWindowOwnerPID]);
      NSNumber *level = (NSNumber *)(windowInfoDictionary[(id)kCGWindowLayer]);
      if (level.integerValue < kScreensaverWindowLevel) {
        NSNumber *windowID = windowInfoDictionary[(id)kCGWindowNumber];
        if (pid == ownerPID.integerValue) {
          wids = [wids stringByAppendingString:[@(windowID.integerValue) stringValue]];
          wids = [wids stringByAppendingString:@"|"];
        }
      }
    }
  }
  if ([wids length] > 0) {
    wids = [wids substringWithRange:NSMakeRange(0, [wids length] - 1)];
  }
  const char *result = [wids UTF8String];
  CFRelease(windowArray);
  [wids release];
  return result;
}

unsigned long cocoa_get_wid_or_pid(bool wid) {
  unsigned long result;
  const CGWindowLevel kScreensaverWindowLevel = CGWindowLevelForKey(kCGScreenSaverWindowLevelKey);
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);
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
    NSWorkspace *sharedWS = [NSWorkspace sharedWorkspace];
    NSArray *runningApps = [sharedWS runningApplications];
    NSRunningApplication *currentApp = [runningApps objectAtIndex:i];
    pid_t currentPid = [currentApp processIdentifier];
    if (cocoa_pid_from_wid(wid) == currentPid) {
      NSRunningApplication *appWithPID = currentApp;
      NSUInteger options = NSApplicationActivateAllWindows;
      options |= NSApplicationActivateIgnoringOtherApps;
      [appWithPID activateWithOptions:options];
      break;
    }
  }
}

void cocoa_wid_set_pwid(CGWindowID wid, CGWindowID pwid) {
  [cocoa_window_from_wid(pwid) setChildWindowWithNumber:wid];
}
