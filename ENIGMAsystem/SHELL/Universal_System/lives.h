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

#ifndef ENIGMA_LIVES_H
#define ENIGMA_LIVES_H

#include "var4.h"
#include "multifunction_variant.h"

namespace enigma
{
  
struct livesv: multifunction_variant<livesv> {
  INHERIT_OPERATORS(livesv)
  void function(const evariant &oldval);
  livesv(): multifunction_variant<livesv>(3) {}
};
void reset_lives();
// Updates the status of whether the "lives" variable
// has been set to zero. Set the "lives" variable status
// to "false". Only call this once per step.
// Even if the value of "lives" is greater than zero
// now, the function may still return true if it has
// previously been set to zero or less and it was positive before that.
bool update_lives_status_and_return_zeroless();

} //namespace enigma

namespace enigma_user
{
  extern enigma::livesv lives;
} //namespace enigma_user

#endif //ENIGMA_LIVES_H

