/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

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
