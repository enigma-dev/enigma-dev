/* Copyright (C) 2020 Greg Williamson
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
*/

#include "sog.h"
#include "egm-events.h"

namespace egm {

std::unique_ptr<Project> SOGFileFormat::LoadProject(const fs::path& fName) const {
  auto proj = std::make_unique<buffers::Project>();

  if (!FolderExists(fName)) {
    errStream << "Error: " << fName << " does not exist" << std::endl;
    return nullptr;
  }

  buffers::Game* game = proj->mutable_game();
  
  // Add tree root
  buffers::TreeNode* game_root = game->mutable_root();
  game_root->set_name("/");
  game_root->set_folder(true);

  // Add our object
  buffers::TreeNode* obj_node = game_root->add_child();
  obj_node->set_name("test_object");
  buffers::resources::Object* obj = obj_node->mutable_object();
  obj->set_sprite_name("");
  obj->set_solid(false);
  obj->set_visible(true);
  obj->set_depth(0);
  obj->set_persistent(false);
  obj->set_parent_name("");
  obj->set_sprite_name("");
  obj->set_mask_name("");
  
  // Load it's events
  const google::protobuf::Descriptor* desc = obj->GetDescriptor();
  LoadObjectEvents(fName, obj, desc->FindFieldByName("egm_events"), _event_data);
  
  // Add our room
  buffers::TreeNode* rm_node = game_root->add_child();
  rm_node->set_name("test_room");
  buffers::resources::Room* rm = rm_node->mutable_room();
  rm->set_caption ("");
  rm->set_width(640);
  rm->set_height(480);
  rm->set_speed(30);
  rm->set_persistent(false);
  rm->set_color(0xFFFFC040);
  rm->set_show_color(true);
  rm->set_creation_code("");
  
  // Add obj to room
  buffers::resources::Room::Instance* inst = rm->add_instances();
  inst->set_id(100001);
  inst->set_x(0);
  inst->set_y(0);
  inst->set_object_type("test_object");

  return proj;

}

bool SOGFileFormat::WriteProject(Project* project, const fs::path& fName) const {
  if (!CreateDirectory(fName))
    return false;
  
  bool foundObj = false;
  
  if (project->has_game()) {
    const auto& game = project->game();
    if (game.has_root()) {
      const auto& root = game.root();
      // Opening sog will add a room but we dont want saving to add one
      if (root.child_size() != 0 && root.child_size() <= 2) {
        for (const auto& child : root.child()) {
          if (child.has_object()) {
            const auto& obj = child.object();
            WriteObjectEvents(fName, obj.egm_events(), _event_data);
            foundObj = true;
            break;
          } else continue;
        }
      } else {
        errStream << "Error: Wrong number of resources: \"" << root.child_size() << "\" for a *single* object game" << std::endl;
        return false;
      }
    } else {
      errStream << "Error: project is missing root node" << std::endl;
      return false;
    }
  } else {
    errStream << "Error: project is missing game" << std::endl;
    return false;
  }
  
  if (!foundObj) errStream << "Error: unable to find any objects" << std::endl;
  
  return foundObj;
}

}
