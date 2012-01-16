// ENIGMA Alarm Extension
// Copyright 2011 Josh Ventura
// Licensed under the GNU General Public License, Version 3 or later.

#include "Universal_System/Extensions/recast.h"
#include "implement.h"
#include "include.h"

declare_recast(enigma::extension_alarm);

extern bool argument_relative;
 
void action_set_alarm(int steps, int alarmno)
{
  if (argument_relative)
    recast_current_instance()->alarm[alarmno] += (steps);
  else
    recast_current_instance()->alarm[alarmno] = (steps);
}
namespace enigma {
extension_alarm::extension_alarm() { for (int i = 0; i < 12; i++) alarm[i] = -1; }
}
