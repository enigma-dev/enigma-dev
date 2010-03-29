/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#include "../JavaStruct.h"

struct Include
{
  String filename;
  String filepath;
  boolean isOriginal;
  int size;
  byte *data;
  int exportThisInclude;
  String exportFolder;
  boolean overwriteExisting;
  boolean freeMemAfterExport;
  boolean removeAtGameEnd;
};
