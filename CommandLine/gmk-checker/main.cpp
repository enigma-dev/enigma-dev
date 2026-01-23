/** Copyright (C) 2024
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

#include "file-format.h"
#include "event_reader/event_parser.h"
#include "strings_util.h"
#include "treenode.pb.h"
#include "game.pb.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>
#include <functional>

using namespace buffers;
using namespace buffers::resources;
using namespace egm;

// Check if a string is a valid C++ identifier
static bool IsValidCppIdentifier(const std::string& name) {
  if (name.empty()) return false;
  
  // First character must be letter or underscore
  if (!std::isalpha(name[0]) && name[0] != '_') {
    return false;
  }
  
  // Remaining characters must be alphanumeric or underscore
  for (size_t i = 1; i < name.length(); i++) {
    if (!std::isalnum(name[i]) && name[i] != '_') {
      return false;
    }
  }
  
  return true;
}

// Get invalid characters in a name
static std::string GetInvalidChars(const std::string& name) {
  std::string invalid;
  for (char c : name) {
    if (!std::isalnum(c) && c != '_') {
      if (invalid.find(c) == std::string::npos) {
        invalid += c;
      }
    }
  }
  return invalid;
}

// Produce a valid C++ identifier from a resource name (for repair).
static std::string SanitizeName(const std::string& name) {
  if (name.empty()) return "_";
  std::string out;
  for (size_t i = 0; i < name.size(); i++) {
    char c = name[i];
    if (std::isalnum(c) || c == '_')
      out += c;
    else if (c == ' ' || c == '-' || c == '+' || c == '.' || c == ',')
      out += '_';
    else
      out += '_';
  }
  if (out.empty()) return "_";
  if (!std::isalpha(out[0]) && out[0] != '_')
    out = "_" + out;
  return out;
}

// Structure to hold resource information
struct ResourceInfo {
  std::string type_name;
  std::string name;
  int id;
  std::string invalid_chars;
  TreeNode* node = nullptr;
};

// Collect all resources from the tree
static void CollectResources(TreeNode* node, std::vector<ResourceInfo>& resources, 
                             std::map<std::string, std::vector<ResourceInfo*>>& name_map,
                             std::map<std::pair<std::string, int>, std::vector<ResourceInfo*>>& id_map) {
  if (node->has_folder()) {
    for (TreeNode& child : *node->mutable_folder()->mutable_children()) {
      CollectResources(&child, resources, name_map, id_map);
    }
  } else {
    // This is a resource node
    std::string type_name;
    int id = -1;
    std::string name = std::string(node->name());
    
    // Determine type and get ID from the resource
    if (node->has_object()) {
      type_name = "object";
      id = node->object().id();
    } else if (node->has_sprite()) {
      type_name = "sprite";
      id = node->sprite().id();
    } else if (node->has_sound()) {
      type_name = "sound";
      id = node->sound().id();
    } else if (node->has_background()) {
      type_name = "background";
      id = node->background().id();
    } else if (node->has_script()) {
      type_name = "script";
      id = node->script().id();
    } else if (node->has_path()) {
      type_name = "path";
      id = node->path().id();
    } else if (node->has_font()) {
      type_name = "font";
      id = node->font().id();
    } else if (node->has_timeline()) {
      type_name = "timeline";
      id = node->timeline().id();
    } else if (node->has_room()) {
      type_name = "room";
      id = node->room().id();
    } else if (node->has_shader()) {
      type_name = "shader";
      id = node->shader().id();
    } else if (node->has_include()) {
      type_name = "include";
      id = node->include().id();
    } else {
      // Skip unknown resource types (settings, unknown, etc.)
      // Settings doesn't have an ID, and unknown resources aren't relevant
      return;
    }
    
    // Only add resources with valid IDs (>= 0) and a valid type
    // Note: ID 0 is valid, so we check >= 0
    if (id >= 0 && !type_name.empty()) {
      ResourceInfo info;
      info.type_name = type_name;
      info.name = name;
      info.id = id;
      info.invalid_chars = GetInvalidChars(name);
      info.node = node;

      resources.push_back(info);
      ResourceInfo* info_ptr = &resources.back();

      // Add to name map for duplicate detection
      name_map[name].push_back(info_ptr);

      // Add to ID map for duplicate ID detection
      id_map[{type_name, id}].push_back(info_ptr);
    }
  }
}

// Check if a character is valid in a C++ identifier
static bool IsIdentifierChar(char c) {
  return std::isalnum(c) || c == '_';
}

// Determine if a code reference should be to an object or sprite based on context
// Uses GM naming conventions and function signatures to infer the expected type
static std::string DetermineResourceTypeFromContext(const std::string& code, size_t pos, const std::string& identifier) {
  // Look backwards and forwards for context
  size_t searchStart = (pos > 200) ? pos - 200 : 0;
  size_t searchEnd = std::min(pos + identifier.length() + 200, code.length());
  std::string context = code.substr(searchStart, searchEnd - searchStart);
  size_t relPos = pos - searchStart;
  
  // Convert to lowercase for case-insensitive matching
  std::string contextLower = context;
  std::transform(contextLower.begin(), contextLower.end(), contextLower.begin(), ::tolower);
  
  // Helper to check if identifier is a parameter to a function call
  auto isFunctionParameter = [&](const std::string& funcName, bool caseSensitive = false) -> bool {
    const std::string& searchStr = caseSensitive ? context : contextLower;
    size_t funcPos = searchStr.find(funcName);
    while (funcPos != std::string::npos) {
      // Find the opening parenthesis for this function
      size_t parenStart = context.find('(', funcPos);
      if (parenStart != std::string::npos && parenStart < relPos) {
        // Find matching closing parenthesis
        int depth = 1;
        size_t parenEnd = parenStart + 1;
        while (parenEnd < context.length() && depth > 0) {
          if (context[parenEnd] == '(') depth++;
          else if (context[parenEnd] == ')') depth--;
          parenEnd++;
        }
        
        // Check if identifier is within these parentheses
        if (parenEnd > relPos + identifier.length()) {
          // Extract the parameter list
          std::string params = context.substr(parenStart + 1, parenEnd - parenStart - 2);
          // Check if identifier appears as a whole word in params
          size_t idPos = params.find(identifier);
          while (idPos != std::string::npos) {
            bool isStart = (idPos == 0 || !IsIdentifierChar(params[idPos - 1]));
            bool isEnd = (idPos + identifier.length() >= params.length() || 
                         !IsIdentifierChar(params[idPos + identifier.length()]));
            if (isStart && isEnd) {
              return true; // Found as a parameter
            }
            idPos = params.find(identifier, idPos + 1);
          }
        }
      }
      funcPos = searchStr.find(funcName, funcPos + 1);
    }
    return false;
  };
  
  // Check for function calls with "sprite" in the name (sprite parameter)
  static const std::vector<std::string> spriteFunctions = {
    "draw_sprite", "sprite_exists", "sprite_get_", "sprite_create", "sprite_replace",
    "sprite_delete", "sprite_duplicate", "sprite_assign", "sprite_merge",
    "sprite_add", "sprite_set_", "sprite_change_", "sprite_save", "sprite_load",
    "font_add_sprite", "font_replace_sprite", "gui_style_set_sprite",
    "sprite_get_width", "sprite_get_height", "sprite_get_xoffset", "sprite_get_yoffset",
    "sprite_get_number", "sprite_get_bbox", "sprite_get_bbox_relative"
  };
  
  for (const auto& func : spriteFunctions) {
    if (isFunctionParameter(func)) {
      return "sprite";
    }
  }
  
  // Check for function calls with "instance", "object", "collision" (object parameter)
  static const std::vector<std::string> objectFunctions = {
    "place_meeting", "collision_", "instance_create", "instance_find",
    "instance_exists", "instance_number", "instance_position", "instance_activate",
    "instance_deactivate", "instance_change", "instance_destroy", "instance_nearest",
    "object_exists", "object_set_", "object_get_", "object_is_ancestor",
    "mp_grid_add_instances", "action_create_object", "instance_place",
    "position_meeting", "position_empty", "position_free"
  };
  
  for (const auto& func : objectFunctions) {
    if (isFunctionParameter(func)) {
      return "object";
    }
  }
  
  // Check for function calls with "room" in the name (room parameter)
  static const std::vector<std::string> roomFunctions = {
    "room_goto", "room_exists", "room_set_", "room_get_", "room_tile_add",
    "room_instance_add", "room_duplicate", "room_assign", "room_restart",
    "room_goto_absolute", "room_goto_first", "room_goto_previous", "room_goto_next",
    "room_next", "room_previous"
  };
  
  for (const auto& func : roomFunctions) {
    if (isFunctionParameter(func)) {
      return "room";
    }
  }
  
  // Check for function calls with "background" in the name (background parameter)
  static const std::vector<std::string> backgroundFunctions = {
    "draw_background", "background_exists", "background_get_", "background_set_",
    "background_add", "background_replace", "background_delete", "background_duplicate",
    "background_assign", "background_create_color", "background_create_gradient",
    "background_get_width", "background_get_height", "background_get_texture",
    "background_get_name", "draw_background_tiled", "draw_background_ext",
    "draw_background_tiled_ext", "background_index"
  };
  
  for (const auto& func : backgroundFunctions) {
    if (isFunctionParameter(func)) {
      return "background";
    }
  }
  
  // Check for GM built-in variables and assignments
  // Pattern: variable = identifier or identifier = variable
  // Also check for: variable = identifier or identifier.something
  static const std::vector<std::string> spriteVars = {
    "sprite_index", "sprite_name", "mask_index", "image_index"
  };
  
  for (const auto& var : spriteVars) {
    // Check for assignment: var = identifier (with word boundaries)
    std::string pattern1 = var + " = " + identifier;
    std::string pattern2 = var + "=" + identifier;
    std::string pattern3 = var + " = " + identifier + ";";
    std::string pattern4 = var + "=" + identifier + ";";
    
    auto checkPattern = [&](const std::string& pattern) {
      size_t p = contextLower.find(pattern);
      if (p != std::string::npos) {
        // Verify it's a whole word match
        if (p == 0 || !IsIdentifierChar(contextLower[p - 1])) {
          return true;
        }
      }
      return false;
    };
    
    if (checkPattern(pattern1) || checkPattern(pattern2) || 
        checkPattern(pattern3) || checkPattern(pattern4)) {
      return "sprite";
    }
  }
  
  static const std::vector<std::string> objectVars = {
    "object_index", "object_type", "other", "self"
  };
  
  for (const auto& var : objectVars) {
    std::string pattern1 = var + " = " + identifier;
    std::string pattern2 = var + "=" + identifier;
    std::string pattern3 = var + " = " + identifier + ";";
    std::string pattern4 = var + "=" + identifier + ";";
    
    auto checkPattern = [&](const std::string& pattern) {
      size_t p = contextLower.find(pattern);
      if (p != std::string::npos) {
        if (p == 0 || !IsIdentifierChar(contextLower[p - 1])) {
          return true;
        }
      }
      return false;
    };
    
    if (checkPattern(pattern1) || checkPattern(pattern2) || 
        checkPattern(pattern3) || checkPattern(pattern4)) {
      return "object";
    }
  }
  
  // Check for room and background built-in variables
  static const std::vector<std::string> roomVars = {
    "room", "room_index"
  };
  
  for (const auto& var : roomVars) {
    std::string pattern1 = var + " = " + identifier;
    std::string pattern2 = var + "=" + identifier;
    std::string pattern3 = var + " = " + identifier + ";";
    std::string pattern4 = var + "=" + identifier + ";";
    
    auto checkPattern = [&](const std::string& pattern) {
      size_t p = contextLower.find(pattern);
      if (p != std::string::npos) {
        if (p == 0 || !IsIdentifierChar(contextLower[p - 1])) {
          return true;
        }
      }
      return false;
    };
    
    if (checkPattern(pattern1) || checkPattern(pattern2) || 
        checkPattern(pattern3) || checkPattern(pattern4)) {
      return "room";
    }
  }
  
  static const std::vector<std::string> backgroundVars = {
    "background_index", "background_name"
  };
  
  for (const auto& var : backgroundVars) {
    std::string pattern1 = var + " = " + identifier;
    std::string pattern2 = var + "=" + identifier;
    std::string pattern3 = var + " = " + identifier + ";";
    std::string pattern4 = var + "=" + identifier + ";";
    
    auto checkPattern = [&](const std::string& pattern) {
      size_t p = contextLower.find(pattern);
      if (p != std::string::npos) {
        if (p == 0 || !IsIdentifierChar(contextLower[p - 1])) {
          return true;
        }
      }
      return false;
    };
    
    if (checkPattern(pattern1) || checkPattern(pattern2) || 
        checkPattern(pattern3) || checkPattern(pattern4)) {
      return "background";
    }
  }
  
  // Check for "with" statement: with(identifier) - always object
  size_t withPos = contextLower.rfind("with(", relPos);
  if (withPos != std::string::npos) {
    size_t withParen = context.find('(', withPos);
    if (withParen != std::string::npos && withParen < relPos) {
      size_t withParenEnd = context.find(')', withParen);
      if (withParenEnd == std::string::npos || withParenEnd > relPos + identifier.length()) {
        return "object";
      }
    }
  }
  
  // Check for collision event context: myevent_collision_identifier
  std::string collisionPattern = "collision_" + identifier;
  if (contextLower.find(collisionPattern) != std::string::npos) {
    return "object";
  }
  
  // Check for common patterns: identifier.sprite_index vs identifier.object_index
  // Also check: identifier.something where "something" indicates the type
  size_t dotPos = context.find(identifier + ".", relPos - identifier.length());
  if (dotPos != std::string::npos && dotPos < relPos) {
    size_t afterDot = dotPos + identifier.length() + 1;
    if (afterDot < context.length()) {
      std::string afterDotStr = context.substr(afterDot, 30);
      std::string afterDotLower = afterDotStr;
      std::transform(afterDotLower.begin(), afterDotLower.end(), afterDotLower.begin(), ::tolower);
      
      // Sprite-related properties
      if (afterDotLower.find("sprite") == 0 || 
          afterDotLower.find("image_") == 0 ||
          afterDotLower.find("mask") == 0) {
        return "sprite";
      }
      
      // Object-related properties
      if (afterDotLower.find("object") == 0 || 
          afterDotLower.find("instance_") == 0 ||
          afterDotLower.find("x") == 0 || afterDotLower.find("y") == 0 ||
          afterDotLower.find("hspeed") == 0 || afterDotLower.find("vspeed") == 0 ||
          afterDotLower.find("solid") == 0 || afterDotLower.find("visible") == 0 ||
          afterDotLower.find("depth") == 0 || afterDotLower.find("persistent") == 0) {
        return "object";
      }
      
      // Room-related properties
      if (afterDotLower.find("room") == 0 ||
          afterDotLower.find("width") == 0 || afterDotLower.find("height") == 0 ||
          afterDotLower.find("speed") == 0 || afterDotLower.find("caption") == 0) {
        return "room";
      }
      
      // Background-related properties
      if (afterDotLower.find("background") == 0) {
        return "background";
      }
    }
  }
  
  // Check for comparison operators: identifier == something or something == identifier
  // If comparing with sprite_index, it's a sprite; if with object_index, it's an object
  std::string beforeId = context.substr(0, relPos);
  std::string afterId = context.substr(relPos + identifier.length());
  
  if (beforeId.find("sprite_index") != std::string::npos || 
      afterId.find("sprite_index") != std::string::npos) {
    return "sprite";
  }
  
  if (beforeId.find("object_index") != std::string::npos || 
      afterId.find("object_index") != std::string::npos) {
    return "object";
  }
  
  if (beforeId.find("room") != std::string::npos || 
      afterId.find("room") != std::string::npos) {
    return "room";
  }
  
  if (beforeId.find("background_index") != std::string::npos || 
      afterId.find("background_index") != std::string::npos) {
    return "background";
  }
  
  return ""; // Unknown/ambiguous
}

// Replace all occurrences of identifier in code, respecting word boundaries and context
// Returns the number of replacements made
static int ReplaceIdentifierInCode(std::string& code, 
                                    const std::string& orig, 
                                    const std::string& rep,
                                    const std::string& context = "",
                                    const std::map<std::string, std::string>* resource_types = nullptr) {
  if (orig.empty() || code.empty()) return 0;
  
  int count = 0;
  size_t pos = 0;
  while ((pos = code.find(orig, pos)) != std::string::npos) {
    // Check if this is a whole identifier (not part of another identifier)
    bool isStart = (pos == 0 || !IsIdentifierChar(code[pos - 1]));
    bool isEnd = (pos + orig.length() >= code.length() || !IsIdentifierChar(code[pos + orig.length()]));
    
    if (isStart && isEnd) {
      // If we have resource type info and there's ambiguity, check context
      std::string expected_type;
      if (resource_types) {
        auto it = resource_types->find(orig);
        if (it != resource_types->end()) {
          expected_type = it->second;
          // If both sprite and object exist with same name, use context to decide
          std::string context_type = DetermineResourceTypeFromContext(code, pos, orig);
          if (!context_type.empty() && context_type != expected_type) {
            // Context suggests different type, skip this replacement
            pos += orig.length();
            continue;
          }
        }
      }
      
      // This is a whole identifier match, replace it
      // Show context around the replacement for debugging
      size_t contextStart = (pos > 20) ? pos - 20 : 0;
      size_t contextEnd = std::min(pos + orig.length() + 20, code.length());
      std::string snippet = code.substr(contextStart, contextEnd - contextStart);
      if (contextStart > 0) snippet = "..." + snippet;
      if (contextEnd < code.length()) snippet = snippet + "...";
      
      std::cout << "  [REPLACE] " << orig << " -> " << rep;
      if (!context.empty()) std::cout << " in " << context;
      if (!expected_type.empty()) std::cout << " (type: " << expected_type << ")";
      std::cout << " (context: " << snippet << ")" << std::endl;
      
      code.replace(pos, orig.length(), rep);
      pos += rep.length();
      count++;
    } else {
      // This is part of another identifier, skip it
      pos += orig.length();
    }
  }
  return count;
}

// Update a resource reference string using the repair map. No-op if not in map.
// Returns true if a replacement was made
static bool UpdateRef(std::string* ref,
                      const std::map<std::string, std::string>& orig_to_repaired,
                      const std::string& context = "") {
  if (ref->empty()) return false;
  auto it = orig_to_repaired.find(*ref);
  if (it != orig_to_repaired.end()) {
    std::cout << "  [REF] " << *ref << " -> " << it->second;
    if (!context.empty()) std::cout << " in " << context;
    std::cout << std::endl;
    *ref = it->second;
    return true;
  }
  return false;
}

// Validate that a replacement target exists in the resource tree
static bool ValidateReplacementTarget(TreeNode* root, const std::string& name) {
  if (root->has_folder()) {
    for (const TreeNode& child : root->folder().children()) {
      if (child.name() == name) {
        return true;  // Found the target resource
      }
      if (child.has_folder() && ValidateReplacementTarget(const_cast<TreeNode*>(&child), name)) {
        return true;
      }
    }
  }
  return false;
}

// Apply name repairs and ref updates throughout the project tree.
static int ApplyRepairs(TreeNode* node,
                         const std::map<std::string, std::string>& orig_to_repaired,
                         const std::map<std::string, std::string>* resource_types = nullptr) {
  int total_replacements = 0;
  
  // Build a sorted vector of replacements (longest first) to avoid partial matches
  std::vector<std::pair<std::string, std::string>> sorted_replacements;
  for (const auto& [orig, rep] : orig_to_repaired) {
    sorted_replacements.push_back({orig, rep});
  }
  std::sort(sorted_replacements.begin(), sorted_replacements.end(),
            [](const auto& a, const auto& b) {
              return a.first.length() > b.first.length();
            });
  
  std::cout << "=== Applying code replacements ===" << std::endl;
  if (node->has_folder()) {
    for (TreeNode& child : *node->mutable_folder()->mutable_children())
      total_replacements += ApplyRepairs(&child, orig_to_repaired, resource_types);
    return total_replacements;
  }
  // Resource names are already renamed in the first pass, so we skip that here
  if (node->has_object()) {
    auto* o = node->mutable_object();
    std::string objContext = "object " + std::string(node->name());
    UpdateRef(o->mutable_parent_name(), orig_to_repaired, objContext + " parent_name");
    UpdateRef(o->mutable_sprite_name(), orig_to_repaired, objContext + " sprite_name");
    UpdateRef(o->mutable_mask_name(), orig_to_repaired, objContext + " mask_name");
    for (auto& ev : *o->mutable_egm_events()) {
      // Update event arguments (e.g., Collision[oBlock] -> Collision[oBlock_2])
      for (int i = 0; i < ev.arguments_size(); i++) {
        std::string arg = std::string(ev.arguments(i));
        auto it = orig_to_repaired.find(arg);
        if (it != orig_to_repaired.end() && it->first != it->second) {
          std::cout << "  [EVENT_ARG] " << std::string(ev.id()) << " argument[" << i << "]: \"" 
                    << arg << "\" -> \"" << it->second << "\"" << std::endl;
          ev.set_arguments(i, it->second);
        }
      }
      
      std::string c(ev.code().data(), ev.code().size());
      std::string context = "object " + std::string(node->name()) + " event " + std::string(ev.id());
      for (const auto& [orig, rep] : sorted_replacements) {
        int count = ReplaceIdentifierInCode(c, orig, rep, context, resource_types);
        total_replacements += count;
      }
      ev.set_code(c);
    }
  }
  if (node->has_path()) {
    std::string pathContext = "path " + std::string(node->name());
    UpdateRef(node->mutable_path()->mutable_background_room_name(), orig_to_repaired, pathContext + " background_room_name");
  }
  if (node->has_room()) {
    auto* r = node->mutable_room();
    // Update room creation code
    if (!r->creation_code().empty()) {
      std::string c(r->creation_code().data(), r->creation_code().size());
      std::string context = "room " + std::string(node->name()) + " creation code";
      for (const auto& [orig, rep] : sorted_replacements) {
        int count = ReplaceIdentifierInCode(c, orig, rep, context, resource_types);
        total_replacements += count;
      }
      r->set_creation_code(c);
    }
    std::string roomContext = "room " + std::string(node->name());
    for (auto& bg : *r->mutable_backgrounds())
      UpdateRef(bg.mutable_background_name(), orig_to_repaired, roomContext + " background");
    for (auto& v : *r->mutable_views())
      UpdateRef(v.mutable_object_following(), orig_to_repaired, roomContext + " view object_following");
    for (auto& inst : *r->mutable_instances()) {
      UpdateRef(inst.mutable_object_type(), orig_to_repaired, roomContext + " instance object_type");
      // Update instance creation code
      if (!inst.creation_code().empty()) {
        std::string c(inst.creation_code().data(), inst.creation_code().size());
        std::string instName = inst.name().empty() ? std::to_string(inst.id()) : std::string(inst.name());
        std::string context = "room " + std::string(node->name()) + " instance " + instName + " creation code";
        for (const auto& [orig, rep] : sorted_replacements) {
          int count = ReplaceIdentifierInCode(c, orig, rep, context);
          total_replacements += count;
        }
        inst.set_creation_code(c);
      }
    }
    for (auto& t : *r->mutable_tiles())
      UpdateRef(t.mutable_background_name(), orig_to_repaired, roomContext + " tile background_name");
  }
  if (node->has_script()) {
    // Update script code
    auto* s = node->mutable_script();
    if (!s->code().empty()) {
      std::string c(s->code().data(), s->code().size());
      std::string context = "script " + std::string(node->name());
      for (const auto& [orig, rep] : sorted_replacements) {
        int count = ReplaceIdentifierInCode(c, orig, rep, context, resource_types);
        total_replacements += count;
      }
      s->set_code(c);
    }
  }
  if (node->has_timeline()) {
    // Update timeline moment code
    auto* t = node->mutable_timeline();
    for (auto& m : *t->mutable_moments()) {
      if (!m.code().empty()) {
        std::string c(m.code().data(), m.code().size());
        std::string context = "timeline " + std::string(node->name()) + " step " + std::to_string(m.step());
        for (const auto& [orig, rep] : sorted_replacements) {
          int count = ReplaceIdentifierInCode(c, orig, rep, context);
          total_replacements += count;
        }
        m.set_code(c);
      }
    }
  }
  if (node->has_shader()) {
    // Update shader code
    auto* s = node->mutable_shader();
    if (!s->vertex_code().empty()) {
      std::string c(s->vertex_code().data(), s->vertex_code().size());
      std::string context = "shader " + std::string(node->name()) + " vertex code";
      for (const auto& [orig, rep] : sorted_replacements) {
        int count = ReplaceIdentifierInCode(c, orig, rep, context, resource_types);
        total_replacements += count;
      }
      s->set_vertex_code(c);
    }
    if (!s->fragment_code().empty()) {
      std::string c(s->fragment_code().data(), s->fragment_code().size());
      std::string context = "shader " + std::string(node->name()) + " fragment code";
      for (const auto& [orig, rep] : sorted_replacements) {
        int count = ReplaceIdentifierInCode(c, orig, rep, context, resource_types);
        total_replacements += count;
      }
      s->set_fragment_code(c);
    }
  }
  
  return total_replacements;
}

static void SetResourceId(TreeNode* node, const std::string& type_name, int new_id) {
  if (type_name == "object") node->mutable_object()->set_id(new_id);
  else if (type_name == "sprite") node->mutable_sprite()->set_id(new_id);
  else if (type_name == "sound") node->mutable_sound()->set_id(new_id);
  else if (type_name == "background") node->mutable_background()->set_id(new_id);
  else if (type_name == "script") node->mutable_script()->set_id(new_id);
  else if (type_name == "path") node->mutable_path()->set_id(new_id);
  else if (type_name == "font") node->mutable_font()->set_id(new_id);
  else if (type_name == "timeline") node->mutable_timeline()->set_id(new_id);
  else if (type_name == "room") node->mutable_room()->set_id(new_id);
  else if (type_name == "shader") node->mutable_shader()->set_id(new_id);
  else if (type_name == "include") node->mutable_include()->set_id(new_id);
}

int main(int argc, char *argv[]) {
  bool repair = false;
  std::string output_path;
  std::string gmk_path;
  for (int i = 1; i < argc; i++) {
    std::string a = argv[i];
    if (a == "--repair" || a == "-r") {
      repair = true;
    } else if ((a == "--output" || a == "-o") && i + 1 < argc) {
      output_path = argv[++i];
    } else if (!a.empty() && a[0] != '-') {
      gmk_path = a;
      break;
    }
  }
  if (gmk_path.empty()) {
    std::cerr << "Usage: " << argv[0] << " [--repair|-r] [--output|-o PATH] <gmk_file>\n"
              << "  --repair    Fix invalid names, duplicates, and duplicate IDs; write EGM to --output.\n"
              << "  --output    Output path for repaired project (EGM directory). Default: <stem>_repaired.egm\n";
    return 1;
  }

  const std::filesystem::path gmkFile = gmk_path;
  
  // Initialize libEGM - need to find events.ey file
  std::filesystem::path eventsFile = "events.ey";
  if (!std::filesystem::exists(eventsFile)) {
    // Try relative to the executable or project root
    eventsFile = "../../events.ey";
    if (!std::filesystem::exists(eventsFile)) {
      std::cerr << "Error: Could not find events.ey file. Please run from project root or ensure events.ey is in the current directory." << std::endl;
      return 1;
    }
  }
  
  EventData event_data(ParseEventFile(eventsFile.u8string()));
  LibEGMInit(&event_data);
  BindOutputStreams(std::cout, std::cerr);

  // Load the project
  std::unique_ptr<buffers::Project> project = LoadProject(gmkFile);
  
  if (project == nullptr) {
    std::cerr << "Error: Failed to load GMK file \"" << gmkFile << "\"" << std::endl;
    return 1;
  }
  
  // Collect all resources
  std::vector<ResourceInfo> resources;
  std::map<std::string, std::vector<ResourceInfo*>> name_map;
  std::map<std::pair<std::string, int>, std::vector<ResourceInfo*>> id_map;
  
  CollectResources(project->mutable_game()->mutable_root(), resources, name_map, id_map);
  
  // Check for issues
  bool has_issues = false;
  
  // Check for invalid C++ identifier characters
  std::cout << "=== Checking for invalid C++ identifier characters ===" << std::endl;
  bool has_invalid = false;
  for (const auto& res : resources) {
    if (!IsValidCppIdentifier(res.name)) {
      has_invalid = true;
      has_issues = true;
      std::cout << "  [" << res.type_name << "] ID " << res.id << " name \"" << res.name 
                << "\" contains invalid characters: ";
      for (char c : res.invalid_chars) {
        std::cout << "'" << c << "' ";
      }
      std::cout << std::endl;
    }
  }
  if (!has_invalid) {
    std::cout << "  ✓ All resource names are valid C++ identifiers" << std::endl;
  }
  std::cout << std::endl;
  
  // Check for duplicate names
  std::cout << "=== Checking for duplicate resource names ===" << std::endl;
  bool has_duplicate_names = false;
  for (const auto& [name, res_list] : name_map) {
    if (res_list.size() > 1) {
      has_duplicate_names = true;
      has_issues = true;
      std::cout << "  Name \"" << name << "\" is used by " << res_list.size() << " resources:" << std::endl;
      for (const auto* res : res_list) {
        std::cout << "    - [" << res->type_name << "] ID " << res->id << std::endl;
      }
    }
  }
  if (!has_duplicate_names) {
    std::cout << "  ✓ No duplicate resource names found" << std::endl;
  }
  std::cout << std::endl;
  
  // Check for duplicate IDs (within same resource type)
  std::cout << "=== Checking for duplicate resource IDs (within same type) ===" << std::endl;
  bool has_duplicate_ids = false;
  for (const auto& [key, res_list] : id_map) {
    if (res_list.size() > 1) {
      has_duplicate_ids = true;
      has_issues = true;
      std::cout << "  [" << key.first << "] ID " << key.second << " is used by " << res_list.size() << " resources:" << std::endl;
      for (const auto* res : res_list) {
        std::cout << "    - \"" << res->name << "\"" << std::endl;
      }
    }
  }
  if (!has_duplicate_ids) {
    std::cout << "  ✓ No duplicate resource IDs found" << std::endl;
  }
  std::cout << std::endl;
  
  // Summary
  std::cout << "=== Summary ===" << std::endl;
  std::cout << "Total resources found: " << resources.size() << std::endl;
  if (!has_issues) {
    std::cout << "✓ No issues found - all resource names are valid and unique!" << std::endl;
    if (!repair) return 0;
    std::cout << "Converting GMK → EGM (--repair)..." << std::endl;
  } else if (!repair) {
    std::cout << "✗ Issues found - see above for details. Use --repair to fix and write EGM." << std::endl;
    return 1;
  }

  // --- Repair: build maps and apply (runs even with no issues when --repair) ---
  // Build resource type map (name -> type) for context-aware replacement
  std::map<std::string, std::string> resource_types;
  for (const auto& res : resources) {
    resource_types[res.name] = res.type_name;
  }
  
  // Check for duplicate names across different resource types (e.g., sprite and object with same name)
  std::map<std::string, std::set<std::string>> name_to_types;
  for (const auto& res : resources) {
    name_to_types[res.name].insert(res.type_name);
  }
  
  std::map<std::string, std::string> orig_to_repaired;
  std::map<std::string, int> sanitized_count;
  std::map<std::string, std::set<std::string>> sanitized_to_types;

  // First pass: sanitize names and track which types use each sanitized name
  for (const auto& res : resources) {
    std::string s = SanitizeName(res.name);
    sanitized_count[s]++;
    sanitized_to_types[s].insert(res.type_name);
  }
  
  // Build a set of all existing resource names (original names) to avoid conflicts
  std::set<std::string> existing_names;
  for (const auto& res : resources) {
    existing_names.insert(res.name);
  }
  
  // Track repaired names to ensure uniqueness
  std::map<std::string, int> repaired_name_count;
  std::map<std::string, std::set<std::string>> repaired_name_to_types;
  
  // Map each resource (by index) to its repaired name, not just by original name
  // This handles the case where multiple resources have the same original name
  std::map<size_t, std::string> resource_index_to_repaired;
  
  // Second pass: build repair map, handling cross-type duplicates
  // Process each resource individually to handle same-name duplicates
  for (size_t i = 0; i < resources.size(); i++) {
    const auto& res = resources[i];
    std::string s = SanitizeName(res.name);
    std::string repaired;
    
    // Count how many resources with this original name we've already processed
    int same_name_count = 0;
    for (size_t j = 0; j < i; j++) {
      if (resources[j].name == res.name) {
        same_name_count++;
      }
    }
    
    // If sanitized name has duplicates OR is used by multiple types (sprite+object conflict)
    if (sanitized_count[s] > 1 || sanitized_to_types[s].size() > 1) {
      // For cross-type conflicts (sprite + object), prefix sprites with "spr_"
      if (sanitized_to_types[s].size() > 1 && res.type_name == "sprite") {
        repaired = "spr_" + s;
        // If this still conflicts with existing names or other repaired names, add ID
        if (existing_names.find(repaired) != existing_names.end() || 
            repaired_name_count[repaired] > 0) {
          repaired = "spr_" + s + "_" + std::to_string(res.id);
        }
      } else if (sanitized_to_types[s].size() > 1 && res.type_name == "object") {
        // Objects keep the base name, but add ID if needed
        if (sanitized_count[s] > 1) {
          repaired = s + "_" + std::to_string(res.id);
        } else {
          repaired = s; // Object wins in sprite+object conflict
          // But ensure it doesn't conflict with existing names
          if (existing_names.find(repaired) != existing_names.end() && 
              existing_names.find(res.name) == existing_names.end()) {
            repaired = s + "_" + std::to_string(res.id);
          }
        }
      } else {
        // Same type duplicates, add ID
        repaired = s + "_" + std::to_string(res.id);
      }
    } else {
      repaired = s;
      // Even if no duplicates, check if repaired name conflicts with an existing different name
      if (existing_names.find(repaired) != existing_names.end() && repaired != res.name) {
        repaired = s + "_" + std::to_string(res.id);
      }
    }
    
    // If multiple resources share the same original name, make them unique
    if (same_name_count > 0) {
      // Add ID to make it unique among same-name resources
      repaired = s + "_" + std::to_string(res.id);
    }
    
    // Ensure final repaired name is unique among all repaired names
    std::string final_repaired = repaired;
    int suffix = 0;
    while (repaired_name_count[final_repaired] > 0) {
      suffix++;
      final_repaired = repaired + "_" + std::to_string(suffix);
    }
    repaired = final_repaired;
    
    // Track the repaired name to avoid new conflicts
    repaired_name_count[repaired]++;
    repaired_name_to_types[repaired].insert(res.type_name);
    
    // Store the mapping for this specific resource by index
    resource_index_to_repaired[i] = repaired;
    
    // Also update the name-based map (for code replacement)
    // If multiple resources have the same name, we'll use the first repaired name for code replacement
    // (This is a heuristic - ideally we'd do context-aware replacement, but for now this works)
    if (orig_to_repaired.find(res.name) == orig_to_repaired.end()) {
      orig_to_repaired[res.name] = repaired;
    }
  }

  std::cout << "\n=== Building replacement map ===" << std::endl;
  for (const auto& [orig, rep] : orig_to_repaired) {
    if (orig != rep) {
      std::cout << "  \"" << orig << "\" -> \"" << rep << "\"" << std::endl;
    }
  }
  std::cout << std::endl;

  // First pass: rename resource names themselves
  // We need to match resources by both name AND ID to handle duplicates correctly
  std::cout << "=== Renaming resources ===" << std::endl;
  int rename_count = 0;
  std::function<void(TreeNode*)> rename_resources = [&](TreeNode* node) {
    if (node->has_folder()) {
      for (TreeNode& child : *node->mutable_folder()->mutable_children())
        rename_resources(&child);
      return;
    }
    
    // Find the resource info that matches this node (by name and ID)
    std::string node_name = std::string(node->name());
    int node_id = -1;
    std::string node_type;
    
    if (node->has_object()) {
      node_id = node->object().id();
      node_type = "object";
    } else if (node->has_sprite()) {
      node_id = node->sprite().id();
      node_type = "sprite";
    } else if (node->has_sound()) {
      node_id = node->sound().id();
      node_type = "sound";
    } else if (node->has_background()) {
      node_id = node->background().id();
      node_type = "background";
    } else if (node->has_script()) {
      node_id = node->script().id();
      node_type = "script";
    } else if (node->has_path()) {
      node_id = node->path().id();
      node_type = "path";
    } else if (node->has_font()) {
      node_id = node->font().id();
      node_type = "font";
    } else if (node->has_timeline()) {
      node_id = node->timeline().id();
      node_type = "timeline";
    } else if (node->has_room()) {
      node_id = node->room().id();
      node_type = "room";
    } else if (node->has_shader()) {
      node_id = node->shader().id();
      node_type = "shader";
    } else if (node->has_include()) {
      node_id = node->include().id();
      node_type = "include";
    }
    
    // Find the matching resource info by name, ID, and type
    size_t matching_index = SIZE_MAX;
    for (size_t i = 0; i < resources.size(); i++) {
      const auto& res = resources[i];
      if (res.name == node_name && res.id == node_id && res.type_name == node_type) {
        // Check if this resource's node pointer matches
        if (res.node == node) {
          matching_index = i;
          break;
        }
        // If no node pointer match, use first match (shouldn't happen, but fallback)
        if (matching_index == SIZE_MAX) {
          matching_index = i;
        }
      }
    }
    
    if (matching_index != SIZE_MAX) {
      auto it = resource_index_to_repaired.find(matching_index);
      if (it != resource_index_to_repaired.end() && node_name != it->second) {
        std::cout << "  [RENAME] Resource [" << node_type << "] ID " << node_id 
                  << ": \"" << node_name << "\" -> \"" << it->second << "\"" << std::endl;
        node->set_name(it->second);
        rename_count++;
      }
    } else {
      // Fallback to name-based lookup if we can't find exact match
      auto it = orig_to_repaired.find(node_name);
      if (it != orig_to_repaired.end() && it->first != it->second) {
        std::cout << "  [RENAME] Resource (fallback): \"" << node_name << "\" -> \"" << it->second << "\"" << std::endl;
        node->set_name(it->second);
        rename_count++;
      }
    }
  };
  rename_resources(project->mutable_game()->mutable_root());
  std::cout << "Renamed " << rename_count << " resources" << std::endl;
  std::cout << std::endl;
  
  // Re-collect resources after renaming to check for duplicates
  std::vector<ResourceInfo> resources_after;
  std::map<std::string, std::vector<ResourceInfo*>> name_map_after;
  std::map<std::pair<std::string, int>, std::vector<ResourceInfo*>> id_map_after;
  CollectResources(project->mutable_game()->mutable_root(), resources_after, name_map_after, id_map_after);
  
  // Check for duplicate names after renaming
  bool has_duplicates_after = false;
  for (const auto& [name, res_list] : name_map_after) {
    if (res_list.size() > 1) {
      has_duplicates_after = true;
      std::cerr << "  ✗ ERROR: After renaming, name \"" << name << "\" is still used by " << res_list.size() << " resources:" << std::endl;
      for (const auto* res : res_list) {
        std::cerr << "    - [" << res->type_name << "] ID " << res->id << std::endl;
      }
    }
  }
  
  // Check for duplicate IDs after renaming
  for (const auto& [key, res_list] : id_map_after) {
    if (res_list.size() > 1) {
      has_duplicates_after = true;
      std::cerr << "  ✗ ERROR: After renaming, [" << key.first << "] ID " << key.second << " is still used by " << res_list.size() << " resources:" << std::endl;
      for (const auto* res : res_list) {
        std::cerr << "    - \"" << res->name << "\"" << std::endl;
      }
    }
  }
  
  if (has_duplicates_after) {
    std::cerr << "\n✗ ERROR: Duplicates still exist after renaming! This should not happen." << std::endl;
    return 1;
  }
  
  std::cout << "✓ Verified: No duplicate names or IDs after renaming" << std::endl;
  std::cout << std::endl;

  // Validate replacements after renaming
  std::cout << "=== Validating replacement targets ===" << std::endl;
  bool all_valid = true;
  for (const auto& [orig, rep] : orig_to_repaired) {
    if (orig != rep) {
      if (!ValidateReplacementTarget(project->mutable_game()->mutable_root(), rep)) {
        std::cerr << "  ✗ WARNING: Replacement target \"" << rep << "\" not found in resource tree!" << std::endl;
        std::cerr << "    Original name: \"" << orig << "\"" << std::endl;
        std::cerr << "    This may indicate the resource was renamed incorrectly." << std::endl;
        all_valid = false;
      } else {
        std::cout << "  ✓ Validated: \"" << orig << "\" -> \"" << rep << "\"" << std::endl;
      }
    }
  }
  if (all_valid) {
    std::cout << "  ✓ All replacement targets validated successfully" << std::endl;
  }
  std::cout << std::endl;

  // Update resource_types map with new names after renaming
  std::map<std::string, std::string> updated_resource_types;
  for (const auto& [orig, rep] : orig_to_repaired) {
    auto it = resource_types.find(orig);
    if (it != resource_types.end()) {
      updated_resource_types[rep] = it->second;
    }
  }
  // Also keep original mappings for resources that weren't renamed
  for (const auto& [name, type] : resource_types) {
    if (orig_to_repaired.find(name) == orig_to_repaired.end() || 
        orig_to_repaired.at(name) == name) {
      updated_resource_types[name] = type;
    }
  }
  
  // Second pass: update code references
  int total_replacements = ApplyRepairs(project->mutable_game()->mutable_root(), 
                                        orig_to_repaired,
                                        &updated_resource_types);
  
  std::cout << "\n=== Replacement Summary ===" << std::endl;
  std::cout << "Resources renamed: " << rename_count << std::endl;
  std::cout << "Total code replacements made: " << total_replacements << std::endl;
  std::cout << std::endl;

  // Fix duplicate IDs: assign new IDs to duplicates (keep first, renumber rest).
  std::map<std::string, int> max_id_by_type;
  for (const auto& res : resources)
    max_id_by_type[res.type_name] = std::max(max_id_by_type[res.type_name], res.id);
  for (const auto& [key, res_list] : id_map) {
    if (res_list.size() <= 1) continue;
    const std::string& type_name = key.first;
    for (size_t i = 1; i < res_list.size(); i++) {
      ResourceInfo* r = res_list[i];
      int new_id = ++max_id_by_type[type_name];
      SetResourceId(r->node, type_name, new_id);
      r->id = new_id;
    }
  }

  std::filesystem::path out = output_path.empty()
      ? gmkFile.parent_path() / (gmkFile.stem().string() + "_repaired.egm")
      : std::filesystem::path(output_path);
  {
    std::string ext = out.extension().empty() ? "" : ToLower(out.extension().u8string());
    if (ext != ".egm")
      out = std::filesystem::path(out.u8string() + (out.u8string().empty() || out.u8string().back() == '/' ? "" : "/"));
    if (ext != ".egm")
      out = out.parent_path() / (out.stem().string() + ".egm");
  }

  std::cout << "Writing repaired project to " << out << std::endl;
  if (!WriteProject(project.get(), out)) {
    std::cerr << "Error: Failed to write repaired project to \"" << out << "\"" << std::endl;
    return 1;
  }
  std::cout << "✓ Repaired project written successfully." << std::endl;
  return 0;
}
