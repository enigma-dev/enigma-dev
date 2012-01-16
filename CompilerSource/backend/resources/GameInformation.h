/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */



#include "backend/JavaStruct.h"

struct GameInformation
{
  int backgroundColor = 0xFFFFE100; //GM color
  boolean mimicGameWindow = false;
  String formCaption = "";
  int left;
  int top;
  int width;
  int height;
  boolean showBorder ;
  boolean allowResize;
  boolean stayOnTop;
  boolean pauseGame;
  String gameInfoStr;
};
