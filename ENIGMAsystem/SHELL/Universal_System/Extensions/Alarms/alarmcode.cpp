// ENIGMA Alarm Extension
// Copyright 2011 Josh Ventura
// Licensed under the GNU General Public License, Version 3 or later.

#include "Universal_System/Object_Tiers/collisions_object.h"
#include "Universal_System/instance_system.h"
#include "implement.h"
#include "include.h"

namespace enigma {
  namespace extension_cast {
    extension_alarm *as_extension_alarm(object_basic*);
  }
}

namespace enigma_user
{

void action_set_alarm(int steps, int alarmno)
{
  extern bool argument_relative;
  if (argument_relative)
    enigma::extension_cast::as_extension_alarm(enigma::instance_event_iterator->inst)->alarm[alarmno] += (steps);
  else
    enigma::extension_cast::as_extension_alarm(enigma::instance_event_iterator->inst)->alarm[alarmno] = (steps);
}

}

namespace enigma {
extension_alarm::extension_alarm() { for (int i = 0; i < 12; i++) alarm[i] = -1; }
}
