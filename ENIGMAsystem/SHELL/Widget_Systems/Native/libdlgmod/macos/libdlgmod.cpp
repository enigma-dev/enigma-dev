/*

 MIT License

 Copyright © 2021-2024 Samuel Venable

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

#include <cstdlib>
#include <cstring>
#include <climits>
#include <cstdio>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <libdlgmod/libdlgmod.h>

#include <unistd.h>
#include <sys/stat.h>

#include <AppKit/AppKit.h>
#include <UniformTypeIdentifiers/UTType.h>

using std::string;
using std::vector;

enum BUTTON_TYPES {
  BUTTON_ABORT,
  BUTTON_IGNORE,
  BUTTON_OK,
  BUTTON_CANCEL,
  BUTTON_YES,
  BUTTON_NO,
  BUTTON_RETRY
};

int const btn_array_len = 7;
string btn_array[btn_array_len] = { "Abort", "Ignore", "OK", "Cancel", "Yes", "No", "Retry" };

void *owner = nullptr;
bool cancel_pressed = false;
int msgres;
int qstres;
int attemptres;
int errorres;
std::string strres;
bool done;

char *cocoa_widget_get_owner() {
  static string str;
  str = std::to_string((unsigned long long)owner);
  return (char *)str.c_str();
}

void cocoa_widget_set_owner(char *hwnd) {
  owner = (void *)strtoull(hwnd, nullptr, 10);
}

const char *cocoa_widget_get_button_name(int type) {
  return dialog_module::widget_get_button_name(type);
}

int cocoa_show_message(const char *str, bool has_cancel, const char *icon, const char *title) {
  cancel_pressed = false;

  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:str];

  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  if (has_cancel) [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [alert setAlertStyle:(NSAlertStyle)1];

  [alert beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {

    if (result == NSAlertFirstButtonReturn) {
      msgres = 1;
      [NSApp stopModal];
    }

    if (result == NSAlertSecondButtonReturn) {
      msgres = -1;
      [NSApp stopModal];
    }

  }];

  [NSApp runModalForWindow:[alert window]];

  [image release];
  [alert release];

  return msgres;
}

int cocoa_show_question(const char *str, bool has_cancel, const char *icon, const char *title) {
  cancel_pressed = false;

  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:str];

  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_YES)]];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_NO)]];
  if (has_cancel) [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [alert setAlertStyle:(NSAlertStyle)1];

  [alert beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {

    if (result == NSAlertFirstButtonReturn) {
      qstres = 1;
      [NSApp stopModal];
    }

    if (result == NSAlertSecondButtonReturn) {
      qstres = 0;
      [NSApp stopModal];
    }

    if (result == NSAlertThirdButtonReturn) {
      qstres = -1;
      [NSApp stopModal];
    }

  }];
    
  [NSApp runModalForWindow:[alert window]];

  [image release];
  [alert release];

  return qstres;
}

int cocoa_show_attempt(const char *str, const char *icon, const char *title) {
  cancel_pressed = false;

  NSString *myStr = [NSString stringWithUTF8String:str];
  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:@"Error"];

  if (![[NSString stringWithUTF8String:title] isEqualToString:@""])
    [alert setMessageText:[NSString stringWithUTF8String:title]];

  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_RETRY)]];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [alert setAlertStyle:(NSAlertStyle)2];

  [alert beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {

    if (result == NSAlertFirstButtonReturn) {
      attemptres = 0;
      [NSApp stopModal];
    }

    if (result == NSAlertSecondButtonReturn) {
      attemptres = -1;
      [NSApp stopModal];
    }

  }];

  [NSApp runModalForWindow:[alert window]];
    
  [image release];
  [alert release];

  return attemptres;
}

int cocoa_show_error(const char *str, bool _abort, const char *icon, const char *title) {
  cancel_pressed = false;

  NSString *myStr = [NSString stringWithUTF8String:str];
  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:@"Error"];

  if (![[NSString stringWithUTF8String:title] isEqualToString:@""])
    [alert setMessageText:[NSString stringWithUTF8String:title]];

  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_ABORT)]];
  if (!_abort) [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_IGNORE)]];
  [alert setAlertStyle:(NSAlertStyle)2];
    
  [alert beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {
        
    if (result == NSAlertFirstButtonReturn) {
      exit(0);
    }

    if (result == NSAlertSecondButtonReturn) {
      errorres = -1;
      [NSApp stopModal];
    }

  }];

  [NSApp runModalForWindow:[alert window]];

  [image release];
  [alert release];

  return errorres;
}

const char *cocoa_input_box(const char *str, const char *def, const char *icon, const char *title, bool numbers) {
  cancel_pressed = false;

  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:str];
  NSString *myDef = [NSString stringWithUTF8String:def];

  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];

  NSTextField *input = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 22)];
  [[input cell] setWraps:NO];
  [[input cell] setScrollable:YES];
  [input setMaximumNumberOfLines:1];
  [input setStringValue:myDef];
  [alert setAccessoryView:input];
  NSView *myAccessoryView = [alert accessoryView];
  [[alert window] setInitialFirstResponder:myAccessoryView];

  [alert beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {

    if (result == NSAlertFirstButtonReturn) {
      [input validateEditing];
      strres = [[input stringValue] UTF8String];
      cancel_pressed = false;
      [NSApp stopModal];
    } else if (result == NSAlertSecondButtonReturn) {
      strres.clear();
      cancel_pressed = true;
      [NSApp stopModal];
    }

  }];

  [NSApp runModalForWindow:[alert window]];

  [input release];
  [image release];
  [alert release];

  if (strcmp(strres.c_str(), "") == 0) {
    cancel_pressed = true;
  }

  return strres.c_str();
}

const char *cocoa_password_box(const char *str, const char *def, const char *icon, const char *title, bool numbers) {
  cancel_pressed = false;

  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:str];
  NSString *myDef = [NSString stringWithUTF8String:def];

  NSAlert *alert = [[NSAlert alloc] init];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:icon]];
  [alert setIcon:image];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  [alert addButtonWithTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];

  NSSecureTextField *input = [[NSSecureTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 22)];
  [[input cell] setWraps:NO];
  [[input cell] setScrollable:YES];
  [input setMaximumNumberOfLines:1];
  [input setStringValue:myDef];
  [alert setAccessoryView:input];
  NSView *myAccessoryView = [alert accessoryView];
  [[alert window] setInitialFirstResponder:myAccessoryView];

  [alert beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {

    if (result == NSAlertFirstButtonReturn) {
      [input validateEditing];
      strres = [[input stringValue] UTF8String];
      cancel_pressed = false;
      [NSApp stopModal];
    } else if (result == NSAlertSecondButtonReturn) {
      strres.clear();
      cancel_pressed = true;
      [NSApp stopModal];
    }

  }];

  [NSApp runModalForWindow:[alert window]];

  [input release];
  [image release];
  [alert release];

  if (strcmp(strres.c_str(), "") == 0) {
    cancel_pressed = true;
  }

  return strres.c_str();
}

std::string theOpenResult;
bool initOpenAccessory;
NSString *selectedOpenPattern;
int openPopIndex;
NSArray *openPatternItems;
const char *cocoa_get_open_filename(const char *filter, const char *fname, const char *dir, const char *title, bool mselect) {
  if (@available(macOS 11.0, *)) {
    cancel_pressed = false;
  
    NSOpenPanel *oFilePanel = [NSOpenPanel openPanel];
    [oFilePanel setMessage:[NSString stringWithUTF8String:title]];
    [oFilePanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir]]];
    [oFilePanel setCanChooseFiles:YES];
    [oFilePanel setCanChooseDirectories:NO];
    [oFilePanel setCanCreateDirectories:NO];
    [oFilePanel setResolvesAliases:YES];
  
    if (mselect)
      [oFilePanel setAllowsMultipleSelection:YES];
    else
      [oFilePanel setAllowsMultipleSelection:NO];
  
    NSView *openView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 431, 21)];
    NSPopUpButton *openPop = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(104, 0, 255, 22)];
    NSString *openFilter = [NSString stringWithUTF8String:filter];
    bool openShowAccessory = true;
    bool openAllowAllFiles = false;
  
    if ([openFilter length] == 0 ||
      ([openFilter rangeOfString:@"|"].location != NSNotFound &&
      [openFilter rangeOfString:@"|"].location == 0))
      openShowAccessory = false;
  
    if ([openFilter length] == 0 ||
      [openFilter rangeOfString:@"|"].location == NSNotFound) {
      openFilter = [openFilter stringByAppendingString:@"|"];
      openAllowAllFiles = true;
    }
  
    int openIndex = 0;
    int openCount = 0, openLength = [openFilter length];
    NSRange openRange = NSMakeRange(0, openLength);
  
    while (openRange.location != NSNotFound) {
      openRange = [openFilter rangeOfString: @"|" options:0 range:openRange];
  
      if (openRange.location != NSNotFound) {
        openRange = NSMakeRange(openRange.location + openRange.length, openLength - (openRange.location + openRange.length));
        openCount += 1;
      }
    }
  
    NSString *openPattern = openFilter;
    openPattern = [openPattern stringByReplacingOccurrencesOfString:@"*." withString:@""];
    openPattern = [openPattern stringByReplacingOccurrencesOfString:@" " withString:@""];
  
    NSArray *openArray1 = [openFilter componentsSeparatedByString:@"|"];
    NSArray *openArray2 = [openPattern componentsSeparatedByString:@"|"];
    NSMutableArray *openPatternArray = [[NSMutableArray alloc] init];
    NSMutableArray *openDescrArray = [[NSMutableArray alloc] init];
  
    for (openIndex = 0; openIndex <= openCount; openIndex += 1) {
      if (openIndex % 2) {
        [openPatternArray addObject:[openArray2 objectAtIndex:openIndex]];
      } else {
        [openDescrArray addObject:[openArray1 objectAtIndex:openIndex]];
      }
    }
  
    selectedOpenPattern = [openPatternArray objectAtIndex:0];
    openPatternItems = [selectedOpenPattern componentsSeparatedByString:@";"];
  
    NSMutableArray<UTType *> *contentTypes = [[NSMutableArray alloc] init];
  
    for (std::size_t i = 0; i < [openPatternItems count]; i++) {
      NSString *ext = [openPatternItems objectAtIndex:i];
      if (UTType *type = [UTType typeWithFilenameExtension:ext]) {
        [contentTypes addObject:type];
      }
    }
      
    [oFilePanel setAllowedContentTypes:contentTypes];
    [contentTypes release];
  
    if ([openPatternItems containsObject:@"*"] || openAllowAllFiles || !openShowAccessory) {
      [oFilePanel setAllowedContentTypes:@[]];
    }
  
    [openPop addItemsWithTitles:openDescrArray];
    [openPop selectItemWithTitle:[openDescrArray objectAtIndex:0]];
    [openView addSubview:openPop];
    [oFilePanel setAccessoryView:openView];
  
    if (!openShowAccessory)
      [oFilePanel setAccessoryView:nullptr];
  
    initOpenAccessory = false;
    openPopIndex = 0;
  
    [oFilePanel beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {
      if (result == NSModalResponseOK) {
        NSURL *theOpenURL;
        NSString *theOpenFile;
        int openURLSize = [[oFilePanel URLs] count];
  
        if (openURLSize > 1) {
          NSMutableArray *openFileArray = [[NSMutableArray alloc] init];
  
          for (int openURLIndex = 0; openURLIndex < openURLSize; openURLIndex += 1) {
            [openFileArray addObject:[[[oFilePanel URLs] objectAtIndex:openURLIndex] path]];
          }
  
          theOpenFile = [openFileArray componentsJoinedByString:@"\n"];
          [openFileArray release];
        } else
          theOpenFile = [[[oFilePanel URLs] objectAtIndex:0] path];
  
        theOpenResult = [theOpenFile UTF8String];
      } else if (result == NSModalResponseCancel)
        theOpenResult.clear();
  
    }];
  
    NSModalSession openSession = [NSApp beginModalSessionForWindow:oFilePanel];
  
    for (;;) {
  
      if ([NSApp runModalSession:openSession] != NSModalResponseContinue)
        break;
  
      if (![oFilePanel isAccessoryViewDisclosed] && !initOpenAccessory) {
        [oFilePanel setAccessoryViewDisclosed:YES];
        initOpenAccessory = true;
      }
  
      if (openShowAccessory) {
        if ([openPop indexOfSelectedItem] != openPopIndex) {
          selectedOpenPattern = [openPatternArray objectAtIndex:[openPop indexOfSelectedItem]];
          openPatternItems = [selectedOpenPattern componentsSeparatedByString:@";"];
              
          if ([openPatternItems containsObject:@"*"]) {
            [oFilePanel setAllowedContentTypes:@[]];
          } else {
            NSMutableArray<UTType *> *contentTypes = [[NSMutableArray alloc] init];
          
            for (std::size_t i = 0; i < [openPatternItems count]; i++) {
              NSString *ext = [openPatternItems objectAtIndex:i];
              if (UTType *type = [UTType typeWithFilenameExtension:ext]) {
                [contentTypes addObject:type];
              }
            }
                
            [oFilePanel setAllowedContentTypes:contentTypes];
            [contentTypes release];
          }
  
          openPopIndex = [openPop indexOfSelectedItem];
        }
      }
    }
  
    [NSApp endModalSession:openSession];
  
    [oFilePanel close];
  
    [openPatternArray release];
    [openDescrArray release];
    [openPop release];
    [openView release];
  
  } else {

    NSOpenPanel *oFilePanel = [NSOpenPanel openPanel];
    [oFilePanel setMessage:[NSString stringWithUTF8String:title]];
    [oFilePanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir]]];
    [oFilePanel setCanChooseFiles:YES];
    [oFilePanel setCanChooseDirectories:NO];
    [oFilePanel setCanCreateDirectories:NO];
    [oFilePanel setResolvesAliases:YES];
  
    if (mselect)
      [oFilePanel setAllowsMultipleSelection:YES];
    else
      [oFilePanel setAllowsMultipleSelection:NO];
  
    NSView *openView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 431, 21)];
    NSPopUpButton *openPop = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(104, 0, 255, 22)];
    NSString *openFilter = [NSString stringWithUTF8String:filter];
    bool openShowAccessory = true;
    bool openAllowAllFiles = false;
  
    if ([openFilter length] == 0 ||
      ([openFilter rangeOfString:@"|"].location != NSNotFound &&
       [openFilter rangeOfString:@"|"].location == 0))
      openShowAccessory = false;
  
    if ([openFilter length] == 0 ||
      [openFilter rangeOfString:@"|"].location == NSNotFound) {
      openFilter = [openFilter stringByAppendingString:@"|"];
      openAllowAllFiles = true;
    }
  
    int openIndex = 0;
    int openCount = 0, openLength = [openFilter length];
    NSRange openRange = NSMakeRange(0, openLength);
  
    while (openRange.location != NSNotFound) {
      openRange = [openFilter rangeOfString: @"|" options:0 range:openRange];
  
      if (openRange.location != NSNotFound) {
        openRange = NSMakeRange(openRange.location + openRange.length, openLength - (openRange.location + openRange.length));
        openCount += 1;
      }
    }
  
    NSString *openPattern = openFilter;
    openPattern = [openPattern stringByReplacingOccurrencesOfString:@"*." withString:@""];
    openPattern = [openPattern stringByReplacingOccurrencesOfString:@" " withString:@""];
  
    NSArray *openArray1 = [openFilter componentsSeparatedByString:@"|"];
    NSArray *openArray2 = [openPattern componentsSeparatedByString:@"|"];
    NSMutableArray *openPatternArray = [[NSMutableArray alloc] init];
    NSMutableArray *openDescrArray = [[NSMutableArray alloc] init];
  
    for (openIndex = 0; openIndex <= openCount; openIndex += 1) {
      if (openIndex % 2) {
        [openPatternArray addObject:[openArray2 objectAtIndex:openIndex]];
      } else {
        [openDescrArray addObject:[openArray1 objectAtIndex:openIndex]];
      }
    }
  
    selectedOpenPattern = [openPatternArray objectAtIndex:0];
    openPatternItems = [selectedOpenPattern componentsSeparatedByString:@";"];
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [oFilePanel setAllowedFileTypes:openPatternItems];
    #pragma clang diagnostic pop
    #pragma GCC diagnostic pop

  
    if ([openPatternItems containsObject:@"*"] || openAllowAllFiles || !openShowAccessory)
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
      #pragma clang diagnostic push
      #pragma clang diagnostic ignored "-Wdeprecated-declarations"
      [oFilePanel setAllowedFileTypes:nullptr];
      #pragma clang diagnostic pop
      #pragma GCC diagnostic pop
  
    [openPop addItemsWithTitles:openDescrArray];
    [openPop selectItemWithTitle:[openDescrArray objectAtIndex:0]];
    [openView addSubview:openPop];
    [oFilePanel setAccessoryView:openView];
  
    if (!openShowAccessory)
      [oFilePanel setAccessoryView:nullptr];
  
    initOpenAccessory = false;
    openPopIndex = 0;
  
    [oFilePanel beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {
      if (result == NSModalResponseOK) {
        NSURL *theOpenURL;
        NSString *theOpenFile;
        int openURLSize = [[oFilePanel URLs] count];
  
        if (openURLSize > 1) {
          NSMutableArray *openFileArray = [[NSMutableArray alloc] init];
  
          for (int openURLIndex = 0; openURLIndex < openURLSize; openURLIndex += 1) {
            [openFileArray addObject:[[[oFilePanel URLs] objectAtIndex:openURLIndex] path]];
          }
  
          theOpenFile = [openFileArray componentsJoinedByString:@"\n"];
          [openFileArray release];
        } else
          theOpenFile = [[[oFilePanel URLs] objectAtIndex:0] path];
  
        theOpenResult = [theOpenFile UTF8String];
      } else if (result == NSModalResponseCancel)
        theOpenResult.clear();
  
    }];
  
    NSModalSession openSession = [NSApp beginModalSessionForWindow:oFilePanel];
  
    for (;;) {
  
      if ([NSApp runModalSession:openSession] != NSModalResponseContinue)
        break;
  
      if (![oFilePanel isAccessoryViewDisclosed] && !initOpenAccessory) {
        [oFilePanel setAccessoryViewDisclosed:YES];
        initOpenAccessory = true;
      }
  
      if (openShowAccessory) {
        if ([openPop indexOfSelectedItem] != openPopIndex) {
          selectedOpenPattern = [openPatternArray objectAtIndex:[openPop indexOfSelectedItem]];
          openPatternItems = [selectedOpenPattern componentsSeparatedByString:@";"];
  
          if ([openPatternItems containsObject:@"*"])
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdeprecated-declarations"
            [oFilePanel setAllowedFileTypes:nullptr];
            #pragma clang diagnostic pop
            #pragma GCC diagnostic pop
          else
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdeprecated-declarations"
            [oFilePanel setAllowedFileTypes:openPatternItems];
            #pragma clang diagnostic pop
            #pragma GCC diagnostic pop
  
          openPopIndex = [openPop indexOfSelectedItem];
        }
      }
    }
  
    [NSApp endModalSession:openSession];
  
    [oFilePanel close];
  
    [openPatternArray release];
    [openDescrArray release];
    [openPop release];
    [openView release];
  
  }
  return theOpenResult.c_str();
}

std::string theSaveResult;
NSString *selectedSavePattern;
NSArray *savePatternItems;
int savePopIndex;
const char *cocoa_get_save_filename(const char *filter, const char *fname, const char *dir, const char *title) {
  if (@available(macOS 11.0, *)) {
    cancel_pressed = false;
  
    NSSavePanel *sFilePanel = [NSSavePanel savePanel];
    [sFilePanel setMessage:[NSString stringWithUTF8String:title]];
    [sFilePanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir]]];
    [sFilePanel setNameFieldStringValue:[[[NSString stringWithUTF8String:fname] lastPathComponent] stringByDeletingPathExtension]];
    [sFilePanel setCanCreateDirectories:YES];
  
    NSView *saveView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 433, 21)];
    NSPopUpButton *savePop = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(105, 0, 255, 22)];
    NSString *saveFilter = [NSString stringWithUTF8String:filter];
    bool saveShowAccessory = true;
    bool saveAllowAllFiles = false;
  
    if ([saveFilter length] == 0 ||
      ([saveFilter rangeOfString:@"|"].location != NSNotFound &&
      [saveFilter rangeOfString:@"|"].location == 0))
      saveShowAccessory = false;
  
    if ([saveFilter length] == 0 ||
      [saveFilter rangeOfString:@"|"].location == NSNotFound) {
      saveFilter = [saveFilter stringByAppendingString:@"|"];
      saveAllowAllFiles = true;
    }
  
    int saveIndex = 0;
    int saveCount = 0, saveLength = [saveFilter length];
    NSRange saveRange = NSMakeRange(0, saveLength);
  
    while (saveRange.location != NSNotFound) {
      saveRange = [saveFilter rangeOfString: @"|" options:0 range:saveRange];
  
      if (saveRange.location != NSNotFound) {
        saveRange = NSMakeRange(saveRange.location + saveRange.length, saveLength - (saveRange.location + saveRange.length));
        saveCount += 1;
      }
    }
  
    NSString *savePattern = saveFilter;
    savePattern = [savePattern stringByReplacingOccurrencesOfString:@"*." withString:@""];
    savePattern = [savePattern stringByReplacingOccurrencesOfString:@" " withString:@""];
  
    NSArray *saveArray1 = [saveFilter componentsSeparatedByString:@"|"];
    NSArray *saveArray2 = [savePattern componentsSeparatedByString:@"|"];
    NSMutableArray *savePatternArray = [[NSMutableArray alloc] init];
    NSMutableArray *saveDescrArray = [[NSMutableArray alloc] init];
  
    for (saveIndex = 0; saveIndex <= saveCount; saveIndex += 1) {
      if (saveIndex % 2) {
        [savePatternArray addObject:[saveArray2 objectAtIndex:saveIndex]];
      } else {
        [saveDescrArray addObject:[saveArray1 objectAtIndex:saveIndex]];
      }
    }
  
    selectedSavePattern = [savePatternArray objectAtIndex:0];
    savePatternItems = [selectedSavePattern componentsSeparatedByString:@";"];
  
    NSMutableArray<UTType *> *contentTypes = [[NSMutableArray alloc] init];
  
    for (std::size_t i = 0; i < [savePatternItems count]; i++) {
      NSString *ext = [savePatternItems objectAtIndex:i];
      if (UTType *type = [UTType typeWithFilenameExtension:ext]) {
        [contentTypes addObject:type];
      }
    }
            
    [sFilePanel setAllowedContentTypes:contentTypes];
    [contentTypes release];
  
    if ([savePatternItems containsObject:@"*"] || saveAllowAllFiles || !saveShowAccessory) {
      [sFilePanel setAllowedContentTypes:@[]];
    }
  
    [savePop addItemsWithTitles:saveDescrArray];
    [savePop selectItemWithTitle:[saveDescrArray objectAtIndex:0]];
    [saveView addSubview:savePop];
    [sFilePanel setAccessoryView:saveView];
  
    if (!saveShowAccessory)
      [sFilePanel setAccessoryView:nullptr];
  
    savePopIndex = 0;
  
    [sFilePanel beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {
      
      if (result == NSModalResponseOK) {
        NSURL *theSaveURL = [sFilePanel URL];
        NSString *theSaveFile = [theSaveURL path];
        theSaveResult = [theSaveFile UTF8String];
      } else if (result == NSModalResponseCancel)
        theSaveResult.clear();
      
    }];
  
    NSModalSession saveSession = [NSApp beginModalSessionForWindow:sFilePanel];
      
    for (;;) {
  
      if ([NSApp runModalSession:saveSession] != NSModalResponseContinue)
        break;
      
      if (saveShowAccessory) {
        if ([savePop indexOfSelectedItem] != savePopIndex) {
          selectedSavePattern = [savePatternArray objectAtIndex:[savePop indexOfSelectedItem]];
          savePatternItems = [selectedSavePattern componentsSeparatedByString:@";"];
  
          if ([savePatternItems containsObject:@"*"]) {
            [sFilePanel setAllowedContentTypes:@[]];
          } else {
            NSMutableArray<UTType *> *contentTypes = [[NSMutableArray alloc] init];
  
            for (std::size_t i = 0; i < [savePatternItems count]; i++) {
              NSString *ext = [savePatternItems objectAtIndex:i];
              if (UTType *type = [UTType typeWithFilenameExtension:ext]) {
                [contentTypes addObject:type];
              }
            }
                  
            [sFilePanel setAllowedContentTypes:contentTypes];
            [contentTypes release];
          }
  
          savePopIndex = [savePop indexOfSelectedItem];
        }
      }
  
    }
  
    [NSApp endModalSession:saveSession];
  
    [sFilePanel close];
  
    [savePatternArray release];
    [saveDescrArray release];
    [savePop release];
    [saveView release];
  
  } else {
  
    NSSavePanel *sFilePanel = [NSSavePanel savePanel];
    [sFilePanel setMessage:[NSString stringWithUTF8String:title]];
    [sFilePanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir]]];
    [sFilePanel setNameFieldStringValue:[[[NSString stringWithUTF8String:fname] lastPathComponent] stringByDeletingPathExtension]];
    [sFilePanel setCanCreateDirectories:YES];
  
    NSView *saveView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 433, 21)];
    NSPopUpButton *savePop = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(105, 0, 255, 22)];
    NSString *saveFilter = [NSString stringWithUTF8String:filter];
    bool saveShowAccessory = true;
    bool saveAllowAllFiles = false;
  
    if ([saveFilter length] == 0 ||
      ([saveFilter rangeOfString:@"|"].location != NSNotFound &&
       [saveFilter rangeOfString:@"|"].location == 0))
      saveShowAccessory = false;
  
    if ([saveFilter length] == 0 ||
      [saveFilter rangeOfString:@"|"].location == NSNotFound) {
      saveFilter = [saveFilter stringByAppendingString:@"|"];
      saveAllowAllFiles = true;
    }
  
    int saveIndex = 0;
    int saveCount = 0, saveLength = [saveFilter length];
    NSRange saveRange = NSMakeRange(0, saveLength);
  
    while (saveRange.location != NSNotFound) {
      saveRange = [saveFilter rangeOfString: @"|" options:0 range:saveRange];
  
      if (saveRange.location != NSNotFound) {
        saveRange = NSMakeRange(saveRange.location + saveRange.length, saveLength - (saveRange.location + saveRange.length));
        saveCount += 1;
      }
    }
  
    NSString *savePattern = saveFilter;
    savePattern = [savePattern stringByReplacingOccurrencesOfString:@"*." withString:@""];
    savePattern = [savePattern stringByReplacingOccurrencesOfString:@" " withString:@""];
  
    NSArray *saveArray1 = [saveFilter componentsSeparatedByString:@"|"];
    NSArray *saveArray2 = [savePattern componentsSeparatedByString:@"|"];
    NSMutableArray *savePatternArray = [[NSMutableArray alloc] init];
    NSMutableArray *saveDescrArray = [[NSMutableArray alloc] init];
  
    for (saveIndex = 0; saveIndex <= saveCount; saveIndex += 1) {
      if (saveIndex % 2) {
        [savePatternArray addObject:[saveArray2 objectAtIndex:saveIndex]];
      } else {
        [saveDescrArray addObject:[saveArray1 objectAtIndex:saveIndex]];
      }
    }
  
    selectedSavePattern = [savePatternArray objectAtIndex:0];
    savePatternItems = [selectedSavePattern componentsSeparatedByString:@";"];
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [sFilePanel setAllowedFileTypes:savePatternItems];
    #pragma clang diagnostic pop
    #pragma GCC diagnostic pop
  
    if ([savePatternItems containsObject:@"*"] || saveAllowAllFiles || !saveShowAccessory)
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
      #pragma clang diagnostic push
      #pragma clang diagnostic ignored "-Wdeprecated-declarations"
      [sFilePanel setAllowedFileTypes:nullptr];
      #pragma clang diagnostic pop
      #pragma GCC diagnostic pop
  
    [savePop addItemsWithTitles:saveDescrArray];
    [savePop selectItemWithTitle:[saveDescrArray objectAtIndex:0]];
    [saveView addSubview:savePop];
    [sFilePanel setAccessoryView:saveView];
  
    if (!saveShowAccessory)
      [sFilePanel setAccessoryView:nullptr];
  
    savePopIndex = 0;
  
    [sFilePanel beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {
        
      if (result == NSModalResponseOK) {
        NSURL *theSaveURL = [sFilePanel URL];
        NSString *theSaveFile = [theSaveURL path];
        theSaveResult = [theSaveFile UTF8String];
      } else if (result == NSModalResponseCancel)
        theSaveResult.clear();
        
    }];
  
    NSModalSession saveSession = [NSApp beginModalSessionForWindow:sFilePanel];
  
    for (;;) {
  
      if ([NSApp runModalSession:saveSession] != NSModalResponseContinue)
        break;
  
      if (saveShowAccessory) {
        if ([savePop indexOfSelectedItem] != savePopIndex) {
          selectedSavePattern = [savePatternArray objectAtIndex:[savePop indexOfSelectedItem]];
          savePatternItems = [selectedSavePattern componentsSeparatedByString:@";"];
  
          if ([savePatternItems containsObject:@"*"])
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdeprecated-declarations"
            [sFilePanel setAllowedFileTypes:nullptr];
            #pragma clang diagnostic pop
            #pragma GCC diagnostic pop
          else
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdeprecated-declarations"
            [sFilePanel setAllowedFileTypes:savePatternItems];
            #pragma clang diagnostic pop
            #pragma GCC diagnostic pop
  
          savePopIndex = [savePop indexOfSelectedItem];
        }
      }
    }
  
    [NSApp endModalSession:saveSession];
  
    [sFilePanel close];
  
    [savePatternArray release];
    [saveDescrArray release];
    [savePop release];
    [saveView release];
    
  }
  return theSaveResult.c_str();
}

std::string theFolderResult;
const char *cocoa_get_directory(const char *capt, const char *root) {
  cancel_pressed = false;

  NSOpenPanel *dirPanel = [NSOpenPanel openPanel];
  [dirPanel setMessage:[NSString stringWithUTF8String:capt]];
  [dirPanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:root]]];
  [dirPanel setCanChooseFiles:NO];
  [dirPanel setCanChooseDirectories:YES];
  [dirPanel setCanCreateDirectories:YES];
  [dirPanel setAllowsMultipleSelection:NO];
  [dirPanel setResolvesAliases:YES];

  [dirPanel beginSheetModalForWindow:(NSWindow *)owner completionHandler:^(NSInteger result) {

    if (result == NSModalResponseOK) {
      NSURL *theFolderURL = [[dirPanel URLs] objectAtIndex:0];
      NSString *theFolderPath = [[theFolderURL path] stringByAppendingString:@"/"];
      theFolderPath = [theFolderPath stringByReplacingOccurrencesOfString:@"//" withString:@"/"];
      theFolderResult = [theFolderPath UTF8String];
      [NSApp stopModal];
    } else if (result == NSModalResponseCancel) {
      theFolderResult.clear();
      [NSApp stopModal];
    }

  }];
    
  [NSApp runModalForWindow:dirPanel];

  [dirPanel close];

  return theFolderResult.c_str();
}

int rescol;
bool colorOKPressed;
int cocoa_get_color(int defcol, const char *title) {
  cancel_pressed = false;

  int redValue = defcol & 0xFF;
  int greenValue = (defcol >> 8) & 0xFF;
  int blueValue = (defcol >> 16) & 0xFF;

  NSInteger buttonWidth = 82, buttonHeight = 30, rightofButtons = 8;
  NSString *myColorTitle = [NSString stringWithUTF8String:title];
  NSColor *myDefCol = [NSColor colorWithCalibratedRed:redValue green:greenValue blue:blueValue alpha:1.0f];
  NSColorPanel *myColorPanel = [NSColorPanel sharedColorPanel];

  [myColorPanel setFrame:NSMakeRect(0, 0, 229, 399) display:YES animate:NO];
  if ([myColorTitle length] > 0) [myColorPanel setTitle:myColorTitle];
  NSSize myColorSize = [[myColorPanel contentView] frame].size;
  NSView *myViewView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, buttonHeight)];
  NSView *myButtonView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, buttonHeight)];
  NSButton *myOKButton = [[NSButton alloc] initWithFrame:NSMakeRect(myColorSize.width - buttonWidth - rightofButtons, 0, buttonWidth, buttonHeight)];
  NSButton *myCancelButton = [[NSButton alloc] initWithFrame:NSMakeRect(myColorSize.width - (buttonWidth * 2) - rightofButtons, 0, buttonWidth, buttonHeight)];

  [myOKButton setTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  [myOKButton setAlternateTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_OK)]];
  [myOKButton setBezelStyle:(NSBezelStyle)1];
  [myCancelButton setTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [myCancelButton setAlternateTitle:[NSString stringWithUTF8String:cocoa_widget_get_button_name(BUTTON_CANCEL)]];
  [myCancelButton setBezelStyle:(NSBezelStyle)1];
  [myButtonView addSubview:myOKButton];
  [myButtonView addSubview:myCancelButton];
  [myOKButton setKeyEquivalent:@"\r"];
  [myOKButton setNeedsDisplay:YES];
  [myViewView addSubview:myButtonView];
  [myColorPanel setColor:myDefCol];
  NSView *oldView = [myColorPanel contentView];
  [oldView retain];
  [myColorPanel setContentView:0];
  NSView *colorView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, myColorSize.height)];
  NSView *parentView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, myColorSize.height)];
  [colorView addSubview:oldView];
  [parentView addSubview:colorView];
  [parentView addSubview:myViewView];
  [myColorPanel setContentView:parentView];
  [[myColorPanel standardWindowButton:NSWindowCloseButton] setHidden:YES];
  [[myColorPanel standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
  [[myColorPanel standardWindowButton:NSWindowZoomButton] setHidden:YES];
  myColorPanel.styleMask &= ~NSWindowStyleMaskResizable;
  colorView.autoresizingMask =  NSViewMinYMargin;
  parentView.autoresizingMask = NSViewMaxYMargin;
  myViewView.autoresizingMask = NSViewWidthSizable | NSViewMinXMargin;
  myButtonView.autoresizingMask = NSViewMinXMargin;

  [myColorPanel setFrame:NSMakeRect(0, 0, 229, 399 + buttonHeight) display:YES animate:NO];
  [myColorPanel center];

  CGFloat r, g, b, a;
  NSColor *myColor = myDefCol;
  int redIntValue, greenIntValue, blueIntValue;
  NSString *redHexValue, *greenHexValue, *blueHexValue;
  NSColor *convertedColor;

  rescol = -1;
  colorOKPressed = false;

  [(NSWindow *)owner beginSheet:myColorPanel completionHandler:^(NSInteger result) {

  }];

  NSModalSession colorSession = [NSApp beginModalSessionForWindow:myColorPanel];

  for (;;) {

    if ([NSApp runModalSession:colorSession] != NSModalResponseContinue) {
      break;
    }

    if ([[myColorPanel standardWindowButton:NSWindowCloseButton] state] == NSControlStateValueOn)
      [[myColorPanel standardWindowButton:NSWindowCloseButton] setState:NSControlStateValueOff];

    if ([myOKButton state] == NSControlStateValueOn) {
      colorOKPressed = true;
      break;
    }

    if ([myCancelButton state] == NSControlStateValueOn) {
      break;
    }

  }

  [NSApp endModalSession:colorSession];
    
  if (colorOKPressed) {
    myColor = [myColorPanel color];
    convertedColor = [myColor colorUsingType:NSColorTypeComponentBased];

    if (convertedColor) {
      [convertedColor getRed:&r green:&g blue:&b alpha:&a];
      redIntValue = (int)(r * 255);
      greenIntValue = (int)(g * 255);
      blueIntValue = (int)(b * 255);

      rescol = (redIntValue & 0xFF) + ((greenIntValue & 0xFF) << 8) + ((blueIntValue & 0xFF) << 16);
    }
  }

  [myColorPanel setContentView:oldView];
  
  [myColorPanel close];

  [oldView release];
  [colorView release];
  [myOKButton release];
  [myCancelButton release];
  [myButtonView release];
  [myViewView release];
  [parentView release];

  return rescol;
}

namespace dialog_module {
  
  namespace {

    string caption;
    string current_icon;

    int const btn_array_len = 7;
    string btn_array[btn_array_len] = { "Abort", "Ignore", "OK", "Cancel", "Yes", "No", "Retry" };
    
    string remove_trailing_zeros(double numb) {
      string strnumb = std::to_string(numb);
      while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
      strnumb.pop_back();
      return strnumb;
    }

    bool file_exists(string fname) {
      struct stat sb;
      return (stat(fname.c_str(), &sb) == 0 &&
        S_ISREG(sb.st_mode) != 0);
    }

    string filename_absolute(string fname) {
      char rpath[PATH_MAX];
      char *result = realpath(fname.c_str(), rpath);
      if (result != nullptr) {
        if (file_exists(result)) return result;
      }
      return "";
    }
    
  } // anonymous namespace
  
  int show_message(char *str) {
    string str_str = str;
    return cocoa_show_message(str_str.c_str(), false, current_icon.c_str(), (caption.empty()) ? "Information" : caption.c_str());
  }
  
  int show_message_cancelable(char *str) {
    string str_str = str;
    return cocoa_show_message(str_str.c_str(), true, current_icon.c_str(), (caption.empty()) ? "Question" : caption.c_str());
  }
  
  int show_question(char *str) {
    string str_str = str;
    return cocoa_show_question(str_str.c_str(), false, current_icon.c_str(), (caption.empty()) ? "Question" : caption.c_str());
  }
  
  int show_question_cancelable(char *str) {
    string str_str = str;
    return cocoa_show_question(str_str.c_str(), true, current_icon.c_str(), (caption.empty()) ? "Question" : caption.c_str());
  }
  
  int show_attempt(char *str) {
    string str_str = str;
    return cocoa_show_attempt(str_str.c_str(), current_icon.c_str(), (caption.empty()) ? "Error" : caption.c_str());
  }
  
  int show_error(char *str, bool abort) {
    string str_str = str;
    int result = cocoa_show_error(str_str.c_str(), abort, current_icon.c_str(), (caption.empty()) ? "Error" : caption.c_str());
    return result;
  }
  
  char *get_string(char *str, char *def) {
    string str_str = str;
    string str_def = def;
    return (char *)cocoa_input_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption.empty()) ? "Input Query" : caption.c_str(), false);
  }
  
  char *get_password(char *str, char *def) {
    string str_str = str;
    string str_def = def;
    return (char *)cocoa_password_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption.empty()) ? "Input Query" : caption.c_str(), false);
  }
  
  double get_integer(char *str, double def) {
    double DIGITS_MIN = -999999999999999;
    double DIGITS_MAX = 999999999999999;

    if (def < DIGITS_MIN) def = DIGITS_MIN;
    if (def > DIGITS_MAX) def = DIGITS_MAX;

    string str_str = str;
    string str_def = remove_trailing_zeros(def);
    double result = strtod(cocoa_input_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption.empty()) ? "Input Query" : caption.c_str(), true), nullptr);

    if (result < DIGITS_MIN) result = DIGITS_MIN;
    if (result > DIGITS_MAX) result = DIGITS_MAX;

    return result;
  }
  
  double get_passcode(char *str, double def) {
    double DIGITS_MIN = -999999999999999;
    double DIGITS_MAX = 999999999999999;

    if (def < DIGITS_MIN) def = DIGITS_MIN;
    if (def > DIGITS_MAX) def = DIGITS_MAX;

    string str_str = str;
    string str_def = remove_trailing_zeros(def);
    double result = strtod(cocoa_password_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption.empty()) ? "Input Query" : caption.c_str(), true), nullptr);

    if (result < DIGITS_MIN) result = DIGITS_MIN;
    if (result > DIGITS_MAX) result = DIGITS_MAX;

    return result;
  }
  
  char *get_open_filename(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), "", "", false);
    return (char *)result.c_str();
  }
  
  char *get_open_filename_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str(), false);
    return (char *)result.c_str();
  }
  
  char *get_open_filenames(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), "", "", true);
    return (char *)result.c_str();
  }
  
  char *get_open_filenames_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str(), true);
    return (char *)result.c_str();
  }
  
  char *get_save_filename(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_save_filename(str_filter.c_str(), str_fname.c_str(), "", "");
    return (char *)result.c_str();
  }
  
  char *get_save_filename_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_save_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str());
    return (char *)result.c_str();
  }
  
  char *get_directory(char *dname) {
    string str_dname = dname;  static string result;
    result = cocoa_get_directory("", str_dname.c_str());
    return (char *)result.c_str();
  }
  
  char *get_directory_alt(char *capt, char *root) {
    string str_dname = root; string str_title = capt; static string result;
    result = cocoa_get_directory(str_title.c_str(), str_dname.c_str());
    return (char *)result.c_str();
  }
  
  int get_color(int defcol) {
    return cocoa_get_color(defcol, "");
  }
  
  int get_color_ext(int defcol, char *title) {
    string str_title = title;
    return cocoa_get_color(defcol, str_title.c_str());
  }
  
  char *widget_get_caption() {
    return (char *)caption.c_str();
  }
  
  void widget_set_caption(char *str) {
    caption = str;
  }
  
  char *widget_get_owner() {
    return cocoa_widget_get_owner();
  }
  
  void widget_set_owner(char *hwnd) {
    cocoa_widget_set_owner(hwnd);
  }

  char *widget_get_icon() {
    return (char *)current_icon.c_str();
  }

  void widget_set_icon(char *icon) {
    current_icon = filename_absolute(icon);
  }
  
  char *widget_get_system() {
    return (char *)"Cocoa";
  }
  
  void widget_set_system(char *sys) {
    
  }

  void widget_set_button_name(int type, char *name) {
    string str_name = name;
    btn_array[type] = str_name;
  }

  char *widget_get_button_name(int type) {
    return (char *)btn_array[type].c_str();
  }

  bool widget_get_canceled() {
    return cancel_pressed;
  }

} // namespace dialog_module
