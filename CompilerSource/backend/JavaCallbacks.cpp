/** Copyright (C) 2008 Josh Ventura, IsmAvatar
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

#include <stdio.h>
#include "JavaStruct.h"
#include "util/Image.h"

#define ER_NoIdeFunc "ERROR: Call to IDE-supplied function not substantiated by signaled memory block"
static void javano_signal() { puts(ER_NoIdeFunc); }
static void javano_signal_i(int) { puts(ER_NoIdeFunc); }
static void javano_signal_cstr(const char*) { puts(ER_NoIdeFunc); }
static int javano_signal_exec(const char*, const char**, int) { puts(ER_NoIdeFunc); return -1; }
static Image *javano_signal_buffer(char *, int) { puts(ER_NoIdeFunc); return NULL; }

//Opens the EnigmaFrame
void (*ide_dia_open) () = javano_signal;
//Appends a given text to the frame log
void (*ide_dia_add) (const char *) = javano_signal_cstr;
//Clears the frame log
void (*ide_dia_clear) () = javano_signal;
//Sets the progress bar (0-100)
void (*ide_dia_progress) (int) = javano_signal_i;
//Applies a given text to the progress bar
void (*ide_dia_progress_text) (const char *) = javano_signal_cstr;

//Applies a given text to the progress bar
void (*ide_output_redirect_file) (const char *) = javano_signal_cstr;
//Opens the EnigmaFrame
void (*ide_output_redirect_reset) () = javano_signal;

//Executes a given command, returns errors or ret val
int (*ide_execute) (const char*, const char**, int) = javano_signal_exec;
//Compresses data. Note image width/height unused.
Image* (*ide_compress_data) (char *, int) = javano_signal_buffer;
