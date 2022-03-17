/** Copyright (C) 2019 Samuel Venable
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#import <Cocoa/Cocoa.h>
#import "filedialogs.h"

const char *cocoa_dialog_caption() {
  NSWindow *window_handle = [[NSApplication sharedApplication] mainWindow];

  if (window_handle != nil)
    return [[window_handle title] UTF8String];

  return "";
}

int cocoa_show_message(const char *message, bool has_cancel, const char *title) {
  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:message];

  if ([[NSString stringWithUTF8String:title] isEqualToString:@""])
    myTitle = [NSString stringWithUTF8String:cocoa_dialog_caption()];

  NSAlert *alert = [[NSAlert alloc] init];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:@"OK"];
  if (has_cancel) [alert addButtonWithTitle:@"Cancel"];
  [alert setAlertStyle:(NSAlertStyle)1];
  NSModalResponse responseTag = [alert runModal];
  [alert release];

  if (responseTag == NSAlertFirstButtonReturn)
    return 1;

  return -1;
}

int cocoa_show_question(const char *message, bool has_cancel, const char *title) {
  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:message];

  if ([[NSString stringWithUTF8String:title] isEqualToString:@""])
    myTitle = [NSString stringWithUTF8String:cocoa_dialog_caption()];

  NSAlert *alert = [[NSAlert alloc] init];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:@"Yes"];
  [alert addButtonWithTitle:@"No"];
  if (has_cancel) [alert addButtonWithTitle:@"Cancel"];
  [alert setAlertStyle:(NSAlertStyle)1];
  NSModalResponse responseTag = [alert runModal];
  [alert release];

  if (responseTag == NSAlertFirstButtonReturn)
    return 1;

  if (responseTag == NSAlertSecondButtonReturn)
    return 0;

  return -1;
}

int cocoa_show_attempt(const char *errortext, const char *title) {
  NSString *myStr = [NSString stringWithUTF8String:errortext];
  NSAlert *alert = [[NSAlert alloc] init];
  [alert setMessageText:@"Error"];

  if (![[NSString stringWithUTF8String:title] isEqualToString:@""])
    [alert setMessageText:[NSString stringWithUTF8String:title]];

  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:@"Retry"];
  [alert addButtonWithTitle:@"Cancel"];
  [alert setAlertStyle:(NSAlertStyle)2];
  NSModalResponse responseTag = [alert runModal];
  [alert release];

  if (responseTag == NSAlertFirstButtonReturn)
    return 0;

  return -1;
}

int cocoa_show_error(const char *errortext, bool fatal, const char *title) {
  NSString *myStr = [NSString stringWithUTF8String:errortext];
  NSAlert *alert = [[NSAlert alloc] init];
  [alert setMessageText:@"Error"];

  if (![[NSString stringWithUTF8String:title] isEqualToString:@""])
    [alert setMessageText:[NSString stringWithUTF8String:title]];

  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:@"Abort"];
  if (!fatal) [alert addButtonWithTitle:@"Ignore"];
  [alert setAlertStyle:(NSAlertStyle)2];
  NSModalResponse responseTag = [alert runModal];
  [alert release];

  if (responseTag == NSAlertFirstButtonReturn || fatal)
    return 1;

  return -1;
}

const char *cocoa_input_box(const char *message, const char *def, const char *title) {
  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:message];
  NSString *myDef = [NSString stringWithUTF8String:def];

  if ([[NSString stringWithUTF8String:title] isEqualToString:@""])
    myTitle = [NSString stringWithUTF8String:cocoa_dialog_caption()];

  NSAlert *alert = [[NSAlert alloc] init];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:@"OK"];
  [alert addButtonWithTitle:@"Cancel"];

  NSTextField *input = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 22)];
  [[input cell] setWraps:NO];
  [[input cell] setScrollable:YES];
  [input setMaximumNumberOfLines:1];
  [input setStringValue:myDef];
  [alert setAccessoryView:input];
  NSView *myAccessoryView = [alert accessoryView];
  [[alert window] setInitialFirstResponder:myAccessoryView];
  NSModalResponse responseTag = [alert runModal];
  const char *result;

  if (responseTag == NSAlertFirstButtonReturn) {
    [input validateEditing];
    result = [[input stringValue] UTF8String];
  } else
    result = "";

  [input release];
  [alert release];

  return result;
}

const char *cocoa_password_box(const char *message, const char *def, const char *title) {
  NSString *myTitle = [NSString stringWithUTF8String:title];
  NSString *myStr = [NSString stringWithUTF8String:message];
  NSString *myDef = [NSString stringWithUTF8String:def];

  if ([[NSString stringWithUTF8String:title] isEqualToString:@""])
    myTitle = [NSString stringWithUTF8String:cocoa_dialog_caption()];

  NSAlert *alert = [[NSAlert alloc] init];
  [alert setMessageText:myTitle];
  [alert setInformativeText:myStr];
  [alert addButtonWithTitle:@"OK"];
  [alert addButtonWithTitle:@"Cancel"];

  NSSecureTextField *input = [[NSSecureTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 22)];
  [[input cell] setWraps:NO];
  [[input cell] setScrollable:YES];
  [input setMaximumNumberOfLines:1];
  [input setStringValue:myDef];
  [alert setAccessoryView:input];
  NSView *myAccessoryView = [alert accessoryView];
  [[alert window] setInitialFirstResponder:myAccessoryView];
  NSModalResponse responseTag = [alert runModal];
  const char *result;

  if (responseTag == NSAlertFirstButtonReturn) {
    [input validateEditing];
    result = [[input stringValue] UTF8String];
  } else
    result = "";

  [input release];
  [alert release];

  return result;
}

const char *cocoa_get_open_filename(const char *filter, const char *fname, const char *dir, const char *title, const bool mselect) {
  static const char *res;
  res = ngs::imgui::get_open_filename_ext(filter, fname, dir, title).c_str();
  return res;
}

const char *cocoa_get_save_filename(const char *filter, const char *fname, const char *dir, const char *title) {
  static const char *res;
  res = ngs::imgui::get_save_filename_ext(filter, fname, dir, title).c_str();
  return res;
}

const char *cocoa_get_directory(const char *capt, const char *root) {
  static const char *res;
  res = ngs::imgui::get_directory_alt(capt, root).c_str();
  return res;
}

int cocoa_get_color(int defcol, const char *title) {
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

  [myOKButton setTitle:@"OK"];
  [myOKButton setAlternateTitle:@"OK"];
  [myOKButton setBezelStyle:(NSBezelStyle)1];
  [myCancelButton setTitle:@"Cancel"];
  [myCancelButton setAlternateTitle:@"Cancel"];
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

  NSModalSession colorSession = [NSApp beginModalSessionForWindow:myColorPanel];
  NSColor *myColor = myDefCol;

  CGFloat r, g, b, a;
  int redIntValue, greenIntValue, blueIntValue;
  NSString *redHexValue, *greenHexValue, *blueHexValue;
  NSColor *convertedColor;

  int rescol = -1;
  bool colorOKPressed = false;

  for (;;) {
    if ([NSApp runModalSession:colorSession] != NSModalResponseContinue)
      break;
    
    if ([[myColorPanel standardWindowButton:NSWindowCloseButton] state] == NSControlStateValueOn) {
      [[myColorPanel standardWindowButton:NSWindowCloseButton] setState:NSControlStateValueOff];
      [NSApp endModalSession:colorSession];
    }

    if ([myOKButton state] == NSControlStateValueOn) {
      [NSApp endModalSession:colorSession];
      colorOKPressed = true;
      [myColorPanel close];
    }

    if ([myCancelButton state] == NSControlStateValueOn) {
      [NSApp endModalSession:colorSession];
      [myColorPanel close];
    }
  }

  if (colorOKPressed) {
    myColor = [myColorPanel color];
    convertedColor = [myColor colorUsingType:NSColorTypeComponentBased];

    if (convertedColor) {
      [convertedColor getRed:&r green:&g blue:&b alpha:&a];
      redIntValue = (int)(r * 255);
      greenIntValue = (int)(g * 255);
      blueIntValue = (int)(b * 255);

      rescol = (redIntValue & 0xFF) + ((greenIntValue & 0xFF) << 8) + ((blueIntValue & 0xFF) << 16);
      [myColorPanel close];
    }
  }

  [myColorPanel setContentView:oldView];

  [oldView release];
  [colorView release];
  [myOKButton release];
  [myCancelButton release];
  [myButtonView release];
  [myViewView release];
  [parentView release];

  return rescol;
}
