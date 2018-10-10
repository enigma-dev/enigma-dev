/** Copyright (C) 2018 Robert B. Colton
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

#include "event.h"

using namespace buffers;

using std::map;
using TypeCase = TreeNode::TypeCase;

const map<TypeCase, p_type> event_type_map = {
	{TypeCase::kSprite, p2t_sprite},     {TypeCase::kSound, p2t_sound},   {TypeCase::kBackground, p2t_background},
	{TypeCase::kPath, p2t_path},         {TypeCase::kScript, p2t_script}, {TypeCase::kFont, p2t_font},
	{TypeCase::kTimeline, p2t_timeline}, {TypeCase::kObject, p2t_object}, {TypeCase::kRoom, p2t_room},
};
