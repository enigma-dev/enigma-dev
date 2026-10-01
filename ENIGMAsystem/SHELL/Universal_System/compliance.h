/** This file is a part of the ENIGMA Development Environment.
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

#ifndef ENIGMA_COMPLIANCE_H
#define ENIGMA_COMPLIANCE_H

namespace enigma {

// The game's GM_COMPATIBILITY_VERSION (50-81 for GM5-GM8.1, 65535 for standard).
// The macro exists only where GAME_SETTINGS.h is included; engine sources are
// compiled once for every game, so they read this value, set in SHELLmain.cpp.
extern const int gm_compatibility_version;

inline bool gm8_compliance() { return gm_compatibility_version <= 81; }

}  // namespace enigma

#endif  // ENIGMA_COMPLIANCE_H
