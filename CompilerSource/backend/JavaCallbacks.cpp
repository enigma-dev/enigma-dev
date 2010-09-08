/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura, IsmAvatar                                  **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <stdio.h>

void javano_signal() { puts("ERROR: Call to IDE-supplied function not substantiated by signaled memory block"); }
void javano_signal_i(int) { puts("ERROR: Call to IDE-supplied function not substantiated by signaled memory block"); }
void javano_signal_cstr(const char*) { puts("ERROR: Call to IDE-supplied function not substantiated by signaled memory block"); }

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
