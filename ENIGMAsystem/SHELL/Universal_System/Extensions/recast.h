// This file is part of the ENIGMA Extensions System
// Copyright (C) 2011 Josh Ventura
// Released under the terms of the GNU General Public License, version 3 or later.

#include "Universal_System/object.h"
#include "Universal_System/instance_system.h"
#define declare_recast(extension_structure) \
  struct _recast_t: enigma::object_basic, virtual extension_structure { }; \
  typedef extension_structure _recast_wrapped_t;
#define recast(object) ((_recast_wrapped_t*)(_recast_t*)object)
#define recast_current_instance() ((_recast_wrapped_t*)(_recast_t*)(enigma::instance_event_iterator->inst))
