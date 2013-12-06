/** Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
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

#include "JavaStruct.h"
#include "util/Image.h"

struct EnigmaCallbacks
{
 //Opens the EnigmaFrame
 void (*dia_open) ();

 //Appends a given text to the frame log
 void (*dia_add) (const char *);

 //Clears the frame log
 void (*dia_clear) ();

 //Sets the progress bar (0-100)
 void (*dia_progress) (int);

 //Applies a given text to the progress bar
 void (*dia_progress_text) (const char *);


 //Sets the file from which data is redirected to frame log
 void (*output_redirect_file) (const char *);

 //Indicates file completion, dumps remainder to frame log
 void (*output_redirect_reset) ();


 //Executes a given command, returns errors or ret val
 int (*ide_execute) (const char*, const char**, bool);

 //Compresses data. Note image width/height unused
 Image* (*ide_compress_data) (char *, int);
};

//Opens the EnigmaFrame
extern void (*ide_dia_open) ();
//Appends a given text to the frame log
extern void (*ide_dia_add) (const char *);
//Clears the frame log
extern void (*ide_dia_clear) ();
//Sets the progress bar (0-100)
extern void (*ide_dia_progress) (int);
//Applies a given text to the progress bar
extern void (*ide_dia_progress_text) (const char *);

//Sets the file from which data is redirected to frame log
extern void (*ide_output_redirect_file) (const char *);
//Indicates file completion, dumps remainder to frame log
extern void (*ide_output_redirect_reset) ();

 //Executes a given command, returns errors or ret val
extern int (*ide_execute) (const char*, const char**, bool);
//Compresses data. Note image width/height unused
extern Image* (*ide_compress_data) (char *, int);
