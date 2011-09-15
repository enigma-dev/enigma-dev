/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifdef JAVASTRUCT_H
#  undef String
#  undef boolean
#  undef byte
#else
#  define JAVASTRUCT_H
#endif
#define String const char*
#define boolean int
#define byte unsigned char
