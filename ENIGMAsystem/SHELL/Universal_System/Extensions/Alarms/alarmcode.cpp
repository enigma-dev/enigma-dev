// ENIGMA Alarm Extension
// Copyright 2011 Josh Ventura
// Licensed under the GNU General Public License, Version 3 or later.

#include "../recast.h"
#include "../../dnd.h"
#include "implement.h"
#include "include.h"

declare_recast(enigma::extension_alarm);

void action_set_alarm(int steps, int alarmno)
{
  if (argument_relative)
    recast_current_instance()->alarm[alarmno] += (steps);
  else
    recast_current_instance()->alarm[alarmno] = (steps);
}
