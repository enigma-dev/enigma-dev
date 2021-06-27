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

#import "CocoaWindow.h"
#import <Cocoa/Cocoa.h>

#import <SDL2/SDL.h>
#import <SDL2/SDL_syswm.h>

unsigned long cocoa_window_identifier() {
  return [(NSWindow *)cocoa_window_handle() windowNumber];
}

unsigned long cocoa_window_get_identifier(void *hwnd) {
  return [(NSWindow *)hwnd windowNumber];
}
