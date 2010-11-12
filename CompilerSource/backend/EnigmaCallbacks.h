/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#include "sub/Image.h"

struct EnigmaCallbacks
{
 //Opens the EnigmaFrame
 void (*coo) ();

 //Appends a given text to the frame log
 void (*coa) (const char *);

 //Clears the frame log
 void (*cock) ();

 //Sets the progress bar (0-100)
 void (*cop) (int);

 //Applies a given text to the progress bar
 void (*cot) (const char *);

 //Compresses data. Note image width/height unused.
 Image* (*ccd) (char *, int);
};
