/** Copyright (C) 2020 Josh Ventura
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

#ifdef AST_DEBUG_TRACKING

#include "ast.h"
#include <unordered_set>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <cstring>

namespace enigma::parsing {
namespace ast_debug {

struct NodeInfo {
  Node* ptr;
  const char* type_name;
  void* creation_stack[10];  // Store return addresses
  size_t stack_depth;
  bool destroyed;
  
  NodeInfo(Node* p, const char* type) 
    : ptr(p), type_name(type), stack_depth(0), destroyed(false) {
    // Capture stack trace using __builtin_return_address
    // Note: This is compiler-specific and may not work on all platforms
    for (size_t i = 0; i < 10; ++i) {
      creation_stack[i] = __builtin_return_address(i + 1);
      if (!creation_stack[i]) break;
      stack_depth = i + 1;
    }
  }
};

static std::unordered_set<Node*> tracked_nodes;
static std::mutex tracking_mutex;
static size_t node_counter = 0;

void TrackNodeCreation(Node* node, const char* type_name) {
  if (!node) return;
  
  std::lock_guard<std::mutex> lock(tracking_mutex);
  auto result = tracked_nodes.insert(node);
  if (result.second) {
    node_counter++;
    std::cerr << "[AST_DEBUG] Created node #" << node_counter 
              << " at " << std::hex << std::showbase << (void*)node 
              << std::dec << " type=" << (type_name ? type_name : "unknown") 
              << " total=" << tracked_nodes.size() << std::endl;
  } else {
    std::cerr << "[AST_DEBUG] WARNING: Node at " << std::hex << std::showbase 
              << (void*)node << std::dec << " already tracked!" << std::endl;
  }
}

void TrackNodeDestruction(Node* node) {
  if (!node) return;
  
  std::lock_guard<std::mutex> lock(tracking_mutex);
  auto it = tracked_nodes.find(node);
  if (it != tracked_nodes.end()) {
    tracked_nodes.erase(it);
    std::cerr << "[AST_DEBUG] Destroyed node at " << std::hex << std::showbase 
              << (void*)node << std::dec << " remaining=" << tracked_nodes.size() 
              << std::endl;
  } else {
    std::cerr << "[AST_DEBUG] WARNING: Destruction of untracked node at " 
              << std::hex << std::showbase << (void*)node << std::dec << std::endl;
  }
}

bool ValidateNode(Node* node) {
  if (!node) return false;
  
  std::lock_guard<std::mutex> lock(tracking_mutex);
  auto it = tracked_nodes.find(node);
  if (it == tracked_nodes.end()) {
    std::cerr << "[AST_DEBUG] ERROR: Node at " << std::hex << std::showbase 
              << (void*)node << std::dec << " is NOT in tracking set!" << std::endl;
    return false;
  }
  return true;
}

void DumpNodeTracking() {
  std::lock_guard<std::mutex> lock(tracking_mutex);
  std::cerr << "[AST_DEBUG] Currently tracking " << tracked_nodes.size() 
            << " nodes:" << std::endl;
  for (Node* node : tracked_nodes) {
    std::cerr << "  Node at " << std::hex << std::showbase << (void*)node 
              << std::dec << std::endl;
  }
}

void ClearTracking() {
  std::lock_guard<std::mutex> lock(tracking_mutex);
  tracked_nodes.clear();
  node_counter = 0;
  std::cerr << "[AST_DEBUG] Cleared tracking data" << std::endl;
}

}  // namespace ast_debug
}  // namespace enigma::parsing

#endif  // AST_DEBUG_TRACKING
