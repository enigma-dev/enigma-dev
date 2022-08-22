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
 void (*coo) ();

 //Appends a given text to the frame log
 void (*coa) (String);

 //Clears the frame log
 void (*cock) ();

 //Sets the progress bar (0-100)
 void (*cop) (int);

 //Applies a given text to the progress bar
 void (*cot) (String);

 //Sets the file from which data is redirected to frame log
 void (*cof) (String)

 //Indicates file completion, dumps remainder to frame log
 void (*ccf) (void)

 //Executes a given command, returns errors or ret val
 int (*cex) (String, String *, boolean)

 //Compresses data. Note image width/height unused
 Image* (*ccd) (char *, int);
};
