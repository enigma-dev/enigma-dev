/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#include "resources/Sprite.h"

struct Keywords {
  int success; // Boolean: Indicates if the code was correct
  string* words[5]; // If not NULL, an array of lines as in the keywords properties file
}
