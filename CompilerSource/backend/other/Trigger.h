/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_TRIGGER_H
#define ENIGMA_TRIGGER_H

#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

struct Trigger
{
  String name;
  String condition;
  int checkStep;
  String constant;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
