/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2013 Robert B. Colton, Adriano Tumminelli
*** Copyright (C) 2014 Seth N. Hetu
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "../General/GScolors.h"
#include "../General/GSprimitives.h"
#include "../General/GSd3d.h"
#include "../General/GSstdraw.h"
#include "../General/GSblend.h"
#include "../General/GSsurface.h"
#include "../General/GSscreen.h"
#include "Universal_System/scalar.h"

namespace enigma_user {

enum {
  sh_vertex = 0,
  sh_fragment = 1,
  sh_tesscontrol = 2,
  sh_tessevaluation = 3,
  sh_geometry = 4
};

} // namespace enigma_user
