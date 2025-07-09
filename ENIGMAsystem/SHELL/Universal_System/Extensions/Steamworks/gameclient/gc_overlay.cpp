/** Copyright (C) 2023-2024 Saif Kandil (k0T0z)
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

#include "gc_overlay.h"

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

GCOverlay::GCOverlay()
    : m_CallbackGameOverlayActivated(this, &GCOverlay::on_game_overlay_activated), overlay_activated_(false) {}

void GCOverlay::on_game_overlay_activated(GameOverlayActivated_t* pCallback) {
  if (pCallback->m_bActive) {
    GCOverlay::overlay_activated_ = true;
    DEBUG_MESSAGE("Overlay activated successfully.", M_INFO);
  } else {
    GCOverlay::overlay_activated_ = false;
    DEBUG_MESSAGE("Overlay deactivated successfully.", M_INFO);
  }
}

bool GCOverlay::overlay_activated() { return GCOverlay::overlay_activated_; }

////////////////////////////////////////////////////////
// Private fields & functions
////////////////////////////////////////////////////////

}  // namespace steamworks_gc
