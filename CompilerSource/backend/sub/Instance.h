/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_INSTANCE_H
#define ENIGMA_INSTANCE_H

#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

struct Instance
{
  int x;
  int y;
  int objectId;
  int id;
  String creationCode;
  String preCreationCode;
  boolean locked;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
