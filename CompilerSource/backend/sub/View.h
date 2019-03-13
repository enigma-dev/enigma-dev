/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_VIEW_H
#define ENIGMA_VIEW_H

#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

struct View
{
  boolean visible;
  int viewX;
  int viewY;
  int viewW;
  int viewH;
  int portX;
  int portY;
  int portW;
  int portH;
  int borderH;
  int borderV;
  int speedH;
  int speedV;
	int objectId;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
