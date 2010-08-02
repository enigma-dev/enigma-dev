/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

/*\\\ This file contains prototypes for functions that must be defined by the audio
|*||| library wrapper modules. Each of these is used by other systems throughout the engine.
\*/// Accidental failure to implement them could cause error.

namespace enigma
{
  // This function is called at the beginning of the game.
  int audiosystem_initialize(); // In it, the audio system can make its startup calls.
  
  // This function is called for each sound in the game's module.
  int sound_add_from_buffer(int id, void* buffer, size_t size); // It should add the sound under the given ID.
  
  // This function is called at the end of the game, as it closes.
  void audiosystem_cleanup(); // It should free memory and shut down the audio library.
}
