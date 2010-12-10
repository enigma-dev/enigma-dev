// Copyright 2010 Josh Ventura, IsmAvatar
// This code is part of ENIGMA. It is released under the terms of the
// GNU General Public License, version 3 or later.

#define e_offsetof(local_member)(char*)&((enigma::object_locals*)&enigma::ENIGMA_global_instance)->local_member - (char*)&enigma::ENIGMA_global_instance
ptrdiff_t alarmStructOffset = e_offsetof(alarm);
