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

#include "Platforms/General/PFwindow.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"
#include "strings_util.h"
#include <stdlib.h>
#include <cstdio>
#include <string>

#ifdef DEBUG_MODE
#include "Universal_System/Resources/resource_data.h"
#include "Universal_System/Object_Tiers/object.h"
#include "Universal_System/debugscope.h"
#endif


#include <Cocoa/Cocoa.h>
#include "filedialogs.h"

using std::string;

namespace enigma {

bool widget_system_initialize() {
  return true;
}

} // namespave enigma

namespace {

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
  if (mselect) res = ngs::imgui::get_open_filenames_ext(filter, dir, title).c_str();
  else res = ngs::imgui::get_open_filename_ext(filter, dir, title).c_str();
  return res;
}

const char *cocoa_get_save_filename(const char *filter, const char *fname, const char *dir, const char *title) {
  static const char *res;
  res = ngs::imgui::get_save_filename_ext(filter, dir, title).c_str();
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


} // anonymous namespace

static string dialog_caption;
static string error_caption;

void show_debug_message_helper(string errortext, MESSAGE_TYPE type) {
  #ifdef DEBUG_MODE
  errortext += "\n\n" + enigma::debug_scope::GetErrors();
  #endif

  if (error_caption == "") error_caption = "Error";
  int result = cocoa_show_error(errortext.c_str(), (type == MESSAGE_TYPE::M_FATAL_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR), error_caption.c_str());
  if (result == 1) exit(0);
}

static string widget = enigma_user::ws_cocoa;

namespace enigma_user {
    
string widget_get_system() {
  return widget;
}

void widget_set_system(string sys) {
  // place holder
}

void show_info(string text, int bgcolor, int left, int top, int width, int height,
  bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
  bool pauseGame, string caption) {

}

int show_message(const string &message) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_message(message.c_str(), false, dialog_caption.c_str());
}

int show_message_cancelable(string message) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_message(message.c_str(), true, dialog_caption.c_str());
}

bool show_question(string message) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return (bool)cocoa_show_question(message.c_str(), false, dialog_caption.c_str());
}

int show_question_cancelable(string message) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_question(message.c_str(), true, dialog_caption.c_str());
}

int show_attempt(string errortext) {
  if (error_caption == "") error_caption = "Error";
  return cocoa_show_attempt(errortext.c_str(), error_caption.c_str());
}

string get_string(string message, string def) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_input_box(message.c_str(), def.c_str(), dialog_caption.c_str());
}

string get_password(string message, string def) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_password_box(message.c_str(), def.c_str(), dialog_caption.c_str());
}

double get_integer(string message, var def) {
  double val = (strtod(def.c_str(), NULL)) ? : (double)def;
  string integer = remove_trailing_zeros(val);
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  string result = cocoa_input_box(message.c_str(), integer.c_str(), dialog_caption.c_str());
  return !result.empty() ? strtod(result.c_str(), NULL) : 0;
}

double get_passcode(string message, var def) {
  double val = (strtod(def.c_str(), NULL)) ? : (double)def;
  string integer = remove_trailing_zeros(val);
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  string result = cocoa_password_box(message.c_str(), integer.c_str(), dialog_caption.c_str());
  return !result.empty() ? strtod(result.c_str(), NULL) : 0;
}

string get_open_filename(string filter, string fname) {
  return cocoa_get_open_filename(filter.c_str(), fname.c_str(), "", "", false);
}

string get_open_filenames(string filter, string fname) {
  return cocoa_get_open_filename(filter.c_str(), fname.c_str(), "", "", true);
}

string get_save_filename(string filter, string fname) {
  return cocoa_get_save_filename(filter.c_str(), fname.c_str(), "", "");
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_open_filename(filter.c_str(), fname.c_str(), dir.c_str(), title.c_str(), false);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_open_filename(filter.c_str(), fname.c_str(), dir.c_str(), title.c_str(), true);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_save_filename(filter.c_str(), fname.c_str(), dir.c_str(), title.c_str());
}

string get_directory(string dname) {
  return cocoa_get_directory("", dname.c_str());
}

string get_directory_alt(string capt, string root) {
  return cocoa_get_directory(capt.c_str(), root.c_str());
}

int get_color(int defcol) {
  return cocoa_get_color(defcol, "");
}

int get_color_ext(int defcol, string title) {
  return cocoa_get_color(defcol, title.c_str());
}

string widget_get_caption() {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  if (error_caption == "") error_caption = "Error";

  if (dialog_caption == cocoa_dialog_caption() && error_caption == "Error")
    return "";

  return dialog_caption;
}

void widget_set_caption(string title) {
  dialog_caption = title; error_caption = title;
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  if (error_caption == "") error_caption = "Error";
}

} // enigma_user
