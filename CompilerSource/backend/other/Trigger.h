/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef _TRIGGER__H
#define _TRIGGER__H

#include "backend/JavaStruct.h"

struct Trigger
{
  String name;
  String condition;
  int checkStep;
  String constant;
};

#endif
