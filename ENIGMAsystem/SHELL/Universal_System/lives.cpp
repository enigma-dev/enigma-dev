/** Copyright (C) 2013 forthevin
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

#include "lives.h"
#include <cmath>

namespace enigma {
  using enigma_user::lives;
  bool has_been_zeroless = false;

  void reset_lives()
  {
    lives.rval.d = -1;
    has_been_zeroless = false;
  }

  bool update_lives_status_and_return_zeroless()
  {
    bool was_zeroless = has_been_zeroless;
    has_been_zeroless = false;
    return was_zeroless;
  }
}

void enigma::livesv::function(const variant &oldval) {
  rval.d = round(rval.d);
  if (rval.d <= 0.0 && oldval.rval.d >= 1.0) {
    has_been_zeroless = true;
  }
}

namespace enigma_user
{
  enigma::livesv lives;
}

