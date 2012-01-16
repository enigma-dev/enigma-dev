/*
 * Copyright (C) 2008 IsmAvatar <cmagicj@nni.com>
 * Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
 *
 * This file is part of ENIGMA.
 *
 * ENIGMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ENIGMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "CocoaWindow.h"
#include "ObjectiveC.h"

#include "Universal_System/CallbackArrays.h"
#include "Universal_System/roomsystem.h"


int main(int argc,char** argv)
{
	enigma::parameters=new char* [argc];
	for (int i=0; i<argc; i++)
		enigma::parameters[i]=argv[i];

	return mainO(argc, argv);
}
