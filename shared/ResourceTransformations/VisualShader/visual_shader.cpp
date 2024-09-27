/*********************************************************************************/
/*                                                                               */
/*  Copyright (C) 2024 Saif Kandil (k0T0z)                                       */
/*                                                                               */
/*  This file is a part of the ENIGMA Development Environment.                   */
/*                                                                               */
/*                                                                               */
/*  ENIGMA is free software: you can redistribute it and/or modify it under the  */
/*  terms of the GNU General Public License as published by the Free Software    */
/*  Foundation, version 3 of the license or any later version.                   */
/*                                                                               */
/*  This application and its source code is distributed AS-IS, WITHOUT ANY       */
/*  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS    */
/*  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more        */
/*  details.                                                                     */
/*                                                                               */
/*  You should have recieved a copy of the GNU General Public License along      */
/*  with this code. If not, see <http://www.gnu.org/licenses/>                   */
/*                                                                               */
/*  ENIGMA is an environment designed to create games and other programs with a  */
/*  high-level, fully compilable language. Developers of ENIGMA or anything      */
/*  associated with ENIGMA are in no way responsible for its users or            */
/*  applications created by its users, or damages caused by the environment      */
/*  or programs made in the environment.                                         */
/*                                                                               */
/*********************************************************************************/

#include "visual_shader.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "visual_shader_nodes.h"

bool t_are_almost_equal(const float& a, const float& b) {
  // Check for exact equality first, required to handle "infinity" values.
  if (a == b) {
    return true;
  }

  // Then check for approximate equality.
  float tolerance{std::numeric_limits<float>::epsilon() * abs(a)};
  if (tolerance < std::numeric_limits<float>::epsilon()) {
    tolerance = std::numeric_limits<float>::epsilon();
  }

  return std::fabs(a - b) < tolerance;
}

VisualShader::VisualShader() {
  // Initialize the graphs with the output node.
  VisualShaderNodeOutput output;
  std::shared_ptr<VisualShaderNode> output_ptr = std::make_shared<VisualShaderNodeOutput>(output);

  VisualShader::graph.nodes[VisualShader::NODE_ID_OUTPUT].node = output_ptr;

  VisualShader::graph.nodes[VisualShader::NODE_ID_OUTPUT].coordinate = {400, 150};
}

int VisualShader::get_valid_node_id() const {
  const VisualShader::Graph* g{&graph};
  // Find the largest key in the map and generate a new ID that is one more than
  // the largest key.
  const int min_valid_id{VisualShader::NODE_ID_OUTPUT + 1};
  return g->nodes.size() ? std::max(min_valid_id, g->nodes.rbegin()->first + 1) : min_valid_id;
}

bool VisualShader::add_node(const std::shared_ptr<VisualShaderNode>& node, const TVector2& coordinate, const int& id) {
  if (!node) {
    std::cout << "Invalid VisualShaderNode" << std::endl;
    return false;
  }

  /* Start the ids from 1 as 0 is reserved for the Output node. */
  if (id < VisualShader::NODE_ID_OUTPUT + 1) {
    std::cout << "Invalid VisualShaderNode ID" << std::endl;
    return false;
  }

  VisualShader::Graph* g{&graph};

  auto it{g->nodes.find(id)};

  /* If the id already exists, return. */
  if (it != g->nodes.end()) {
    std::cout << "Node ID already exists" << std::endl;
    return false;
  }

  Node n;
  n.node = node;
  n.coordinate = coordinate;

  g->nodes[id] = n;  // Add the node to the graph.

  return true;
}

int VisualShader::find_node_id(const std::shared_ptr<VisualShaderNode>& node) const {
  const VisualShader::Graph* g{&graph};

  for (const std::pair<const int, VisualShader::Node>& p : g->nodes) {
    if (p.second.node == node) {
      return p.first;
    }
  }

  return NODE_ID_INVALID;
}

bool VisualShader::remove_node(const int& id) {
  /* Start the ids from 1 as 0 is reserved for the Output node. */
  if (id < VisualShader::NODE_ID_OUTPUT + 1) {
    std::cout << "Invalid VisualShaderNode ID" << std::endl;
    return false;
  }

  VisualShader::Graph* g{&graph};

  auto it{g->nodes.find(id)};

  /* If the id is invalid, return. */
  if (it == g->nodes.end()) {
    std::cout << "Node ID already exists" << std::endl;
    return false;
  }

  g->nodes.erase(id);

  // Delete any connections that are connected to the node.
  int i{0};

  for (const Connection& c : g->connections) {
    if (c.from_node != id && c.to_node != id) {
      continue;
    }

    if (c.from_node == id) {
      g->nodes.at(c.to_node).prev_connected_nodes.erase(g->nodes[c.to_node].prev_connected_nodes.begin() + id);
      g->nodes.at(c.to_node).node->set_input_port_connected(c.to_port, false);
    } else if (c.to_node == id) {
      g->nodes.at(c.from_node).next_connected_nodes.erase(g->nodes[c.from_node].next_connected_nodes.begin() + id);
    }

    g->connections.erase(g->connections.begin() + i);  // Remove the connection c.
    i++;
  }

  return true;
}

std::shared_ptr<VisualShaderNode> VisualShader::get_node(const int& id) const {
  const VisualShader::Graph* g{&graph};

  auto it{g->nodes.find(id)};

  /* If the id is invalid, return. */
  if (it == g->nodes.end()) {
    std::cout << "Node ID does not exist" << std::endl;
    return std::shared_ptr<VisualShaderNode>();
  }

  return it->second.node;
}

TVector2 VisualShader::get_node_coordinate(const int& id) const {
  const VisualShader::Graph* g{&graph};

  auto it{g->nodes.find(id)};

  /* If the id is invalid, return. */
  if (it == g->nodes.end()) {
    std::cout << "Node ID does not exist" << std::endl;
    return TVector2();
  }

  return it->second.coordinate;
}

bool VisualShader::set_node_coordinate(const int& id, const TVector2& coordinate) {
  VisualShader::Graph* g{&graph};

  auto it{g->nodes.find(id)};

  /* If the id is invalid, return. */
  if (it == g->nodes.end()) {
    std::cout << "Node ID does not exist" << std::endl;
    return false;
  }

  it->second.coordinate = coordinate;

  return true;
}

bool VisualShader::is_port_types_compatible(const int& p1, const int& p2) const {
  return std::max(0, p1 - (int)VisualShaderNode::PortType::PORT_TYPE_BOOLEAN) ==
         std::max(0, p2 - (int)VisualShaderNode::PortType::PORT_TYPE_BOOLEAN);
}

bool VisualShader::is_nodes_connected_relatively(const int& node, const int& target) const {
  const VisualShader::Graph* g{&graph};

  bool result{false};

  const VisualShader::Node& n{g->nodes.at(node)};

  for (const int& pcn : n.prev_connected_nodes) {
    if (pcn == target) {
      return true;
    }

    result = is_nodes_connected_relatively(pcn, target);
    if (result) {
      break;
    }
  }

  return result;
}

bool VisualShader::can_connect_nodes(const int& from_node, const int& from_port, const int& to_node,
                                     const int& to_port) const {
  if (from_node == to_node) {
    return false;
  }

  const VisualShader::Graph* g{&graph};

  if (g->nodes.find(from_node) == g->nodes.end()) {
    return false;
  }

  if (from_port < 0 || from_port >= g->nodes.at(from_node).node->get_expanded_output_port_count()) {
    return false;
  }

  if (g->nodes.find(to_node) == g->nodes.end()) {
    return false;
  }

  if (to_port < 0 || to_port >= g->nodes.at(to_node).node->get_input_port_count()) {
    return false;
  }

  VisualShaderNode::PortType from_port_type{g->nodes.at(from_node).node->get_output_port_type(from_port)};
  VisualShaderNode::PortType to_port_type{g->nodes.at(to_node).node->get_input_port_type(to_port)};

  if (!is_port_types_compatible(from_port_type, to_port_type)) {
    return false;
  }

  for (const Connection& c : g->connections) {
    if (c.from_node == from_node && c.from_port == from_port && c.to_node == to_node && c.to_port == to_port) {
      return false;
    }
  }

  if (is_nodes_connected_relatively(from_node, to_node)) {
    return false;
  }

  return true;
}

bool VisualShader::connect_nodes(const int& from_node, const int& from_port, const int& to_node, const int& to_port) {
  if (from_node == to_node) {
    return false;
  }

  VisualShader::Graph* g{&graph};

  if (g->nodes.find(from_node) == g->nodes.end()) {
    return false;
  }

  if (from_port < 0 || from_port >= g->nodes.at(from_node).node->get_expanded_output_port_count()) {
    return false;
  }

  if (g->nodes.find(to_node) == g->nodes.end()) {
    return false;
  }

  if (to_port < 0 || to_port >= g->nodes.at(to_node).node->get_input_port_count()) {
    return false;
  }

  VisualShaderNode::PortType from_port_type{g->nodes.at(from_node).node->get_output_port_type(from_port)};
  VisualShaderNode::PortType to_port_type{g->nodes.at(to_node).node->get_input_port_type(to_port)};
  bool are_compatible{is_port_types_compatible(from_port_type, to_port_type)};

  if (!are_compatible) {
    std::cout << "Incompatible port types." << std::endl;
    return false;
  }

  // Check if the connection already exists.
  for (const Connection& c : g->connections) {
    if (c.from_node == from_node && c.from_port == from_port && c.to_node == to_node && c.to_port == to_port) {
      return false;
    }
  }

  Connection c;
  c.from_node = from_node;
  c.from_port = from_port;
  c.to_node = to_node;
  c.to_port = to_port;
  g->connections.emplace_back(c);
  g->nodes.at(from_node).next_connected_nodes.emplace_back(to_node);
  g->nodes.at(to_node).prev_connected_nodes.emplace_back(from_node);
  g->nodes.at(from_node).node->set_output_port_connected(from_port, true);
  g->nodes.at(to_node).node->set_input_port_connected(to_port, true);

  return true;
}

bool VisualShader::disconnect_nodes(const int& from_node, const int& from_port, const int& to_node,
                                    const int& to_port) {
  VisualShader::Graph* g{&graph};

  int i{0};

  for (const Connection& c : g->connections) {
    if (c.from_node == from_node && c.from_port == from_port && c.to_node == to_node && c.to_port == to_port) {
      g->connections.erase(g->connections.begin() + i);  // Remove the connection c.
      g->nodes.at(from_node).next_connected_nodes.erase(g->nodes.at(from_node).next_connected_nodes.begin() + to_node);
      g->nodes.at(to_node).prev_connected_nodes.erase(g->nodes.at(to_node).prev_connected_nodes.begin() + from_node);

      g->nodes.at(from_node).node->set_output_port_connected(from_port, false);
      g->nodes.at(to_node).node->set_input_port_connected(to_port, false);
      return true;
    }
    i++;
  }

  return false;
}

VisualShader::Connection VisualShader::get_connection(const int& id) const {
  const VisualShader::Graph* g{&graph};

  if (id < 0 || id >= (int)g->connections.size()) {
    return Connection();
  }

  return g->connections.at(id);
}

bool VisualShader::generate_shader() const {
  const VisualShader::Graph* g{&graph};
  static const std::string func_name{"fragment"};

  std::string global_code;
  std::string global_code_per_node;
  std::string shader_code;
  std::unordered_set<std::string> global_processed;

  // Store the connections for faster generation.
  std::map<ConnectionKey, const Connection*> input_connections;
  std::map<ConnectionKey, const Connection*> output_connections;

  std::string func_code;
  std::unordered_set<int> processed;

  for (const Connection& c : g->connections) {
    ConnectionKey from_key;
    from_key.node = c.from_node;
    from_key.port = c.from_port;

    output_connections[from_key] = &c;

    ConnectionKey to_key;
    to_key.node = c.to_node;
    to_key.port = c.to_port;

    input_connections[to_key] = &c;
  }

  func_code += "\nvoid " + func_name + "() {" + std::string("\n");

  // Generate the code for each node.
  bool status{generate_shader_for_each_node(global_code, global_code_per_node, func_code, input_connections,
                                            output_connections, VisualShader::NODE_ID_OUTPUT, processed,
                                            global_processed)};

  if (!status) {
    std::cout << "Failed to generate shader for node " << VisualShader::NODE_ID_OUTPUT << std::endl;
    return false;
  }

  func_code += std::string("}") + "\n\n";
  shader_code += func_code;

  std::string generated_code{global_code};
  generated_code += global_code_per_node;

  generated_code += shader_code;

  VisualShader::set_code(generated_code);

  return true;
}

std::string VisualShader::generate_preview_shader(const int& node_id, const int& port) const {
  const VisualShader::Graph* g{&graph};
  static const std::string func_name{"fragment"};

  const std::shared_ptr<VisualShaderNode> n{get_node(node_id)};

  if (!n) {
    std::cout << "Invalid node" << std::endl;
    return std::string();
  }

  std::string global_code;
  std::string global_code_per_node;
  std::string shader_code;
  std::unordered_set<std::string> global_processed;

  // Store the connections for faster generation.
  std::map<ConnectionKey, const Connection*> input_connections;
  std::map<ConnectionKey, const Connection*> output_connections;

  std::unordered_set<int> processed;

  for (const Connection& c : g->connections) {
    ConnectionKey from_key;
    from_key.node = c.from_node;
    from_key.port = c.from_port;

    output_connections[from_key] = &c;

    ConnectionKey to_key;
    to_key.node = c.to_node;
    to_key.port = c.to_port;

    input_connections[to_key] = &c;
  }

  shader_code += "\nvoid " + func_name + "() {" + std::string("\n");

  // Generate the code for each node.
  bool status{generate_shader_for_each_node(global_code, global_code_per_node, shader_code, input_connections,
                                            output_connections, node_id, processed, global_processed)};
  if (!status) {
    std::cout << "Failed to generate shader for node " << node_id << std::endl;
    return std::string();
  }

  switch (n->get_output_port_type(port)) {
		case VisualShaderNode::PORT_TYPE_SCALAR: 
			shader_code += std::string("\t") + "COLOR.rgb = vec3(var_from_n" + std::to_string(node_id) + "_p" + std::to_string(port) + ");" + std::string("\n");
		  break;
		case VisualShaderNode::PORT_TYPE_SCALAR_INT: 
			shader_code += std::string("\t") + "COLOR.rgb = vec3(float(var_from_n" + std::to_string(node_id) + "_p" + std::to_string(port) + "));" + std::string("\n");
		  break;
		case VisualShaderNode::PORT_TYPE_SCALAR_UINT: 
			shader_code += std::string("\t") + "COLOR.rgb = vec3(float(var_from_n" + std::to_string(node_id) + "_p" + std::to_string(port) + "));" + std::string("\n");
		  break;
		case VisualShaderNode::PORT_TYPE_BOOLEAN: 
			shader_code += std::string("\t") + "COLOR.rgb = vec3(var_from_n" + std::to_string(node_id) + "_p" + std::to_string(port) + " ? 1.0 : 0.0);" + std::string("\n");
		  break;
		case VisualShaderNode::PORT_TYPE_VECTOR_2D: 
			shader_code += std::string("\t") + "COLOR.rgb = vec3(var_from_n" + std::to_string(node_id) + "_p" + std::to_string(port) + ", 0.0);" + std::string("\n");
		  break;
		case VisualShaderNode::PORT_TYPE_VECTOR_3D: 
			shader_code += std::string("\t") + "COLOR.rgb = var_from_n" + std::to_string(node_id) + "_p" + std::to_string(port) + ";" + std::string("\n");
		  break;
		case VisualShaderNode::PORT_TYPE_VECTOR_4D: 
			shader_code += std::string("\t") + "COLOR.rgb = var_from_n" + std::to_string(node_id) + "_p" + std::to_string(port) + ".xyz;" + std::string("\n");
		  break;
		default:
			shader_code += std::string("\t") + "COLOR.rgb = vec3(0.0);" + std::string("\n");
		  break;
	}

  shader_code += std::string("}") + "\n\n";

  std::string generated_code{global_code};
  generated_code += global_code_per_node;

  generated_code += shader_code;

  return generated_code;
}

bool VisualShader::generate_shader_for_each_node(std::string& global_code, std::string& global_code_per_node,
                                                 std::string& func_code,
                                                 const std::map<ConnectionKey, const Connection*>& input_connections,
                                                 const std::map<ConnectionKey, const Connection*>& output_connections,
                                                 const int& node_id, std::unordered_set<int>& processed,
                                                 std::unordered_set<std::string>& global_processed) const {
  const VisualShader::Graph* g{&graph};
  const std::shared_ptr<VisualShaderNode> n{g->nodes.at(node_id).node};

  // Check inputs recursively.
  int input_port_count{n->get_input_port_count()};
  for (int i{0}; i < input_port_count; i++) {
    ConnectionKey key;
    key.node = node_id;
    key.port = i;

    if (input_connections.find(key) == input_connections.end()) {
      continue;
    }

    const Connection* c{input_connections.at(key)};

    int from_node{c->from_node};

    if (processed.find(from_node) != processed.end()) {
      continue;
    }

    bool status{generate_shader_for_each_node(global_code, global_code_per_node, func_code, input_connections,
                                              output_connections, from_node, processed, global_processed)};
    if (!status) {
      std::cout << "Failed to generate shader for node " << from_node << std::endl;
      return false;
    }
  }

  // Generate the global code for the current node.
  std::shared_ptr<VisualShaderNodeInput> input{std::dynamic_pointer_cast<VisualShaderNodeInput>(n)};
  bool skip_global{input != nullptr};

  if (!skip_global) {
    std::string type_str{
        typeid(*n)
            .name()};  // This gives (length of string name of the class) + (the string name of the class). Ex.: 26VisualShaderNodeValueNoise
    global_code += n->generate_global(node_id);
    if (global_processed.find(type_str) == global_processed.end()) {
      global_code_per_node += n->generate_global_per_node(node_id);
    }
    global_processed.insert(type_str);
  }

  // Generate the code for the current node.
  std::string node_name{"// " + n->get_caption() + ":" + std::to_string(node_id) + "\n"};
  std::string node_code;
  std::vector<std::string> input_vars;

  input_vars.resize(n->get_input_port_count());

  for (int i{0}; i < input_port_count; i++) {
    ConnectionKey key;
    key.node = node_id;
    key.port = i;

    // Check if the input is not connected.
    if (input_connections.find(key) != input_connections.end()) {
      const Connection* c{input_connections.at(key)};

      int from_node{c->from_node};
      int from_port{c->from_port};

      VisualShaderNode::PortType to_port_type{n->get_input_port_type(i)};
      VisualShaderNode::PortType from_port_type{g->nodes.at(from_node).node->get_output_port_type(from_port)};

      std::string from_var{"var_from_n" + std::to_string(from_node) + "_p" + std::to_string(from_port)};

      if (to_port_type == from_port_type) {
        input_vars.at(i) = from_var;
      } else {
        switch (to_port_type) {
          case VisualShaderNode::PortType::PORT_TYPE_SCALAR: {
            switch (from_port_type) {
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT: {
                input_vars.at(i) = "float(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT: {
                input_vars.at(i) = "float(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN: {
                input_vars.at(i) = "(" + from_var + " ? 1.0 : 0.0)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
                input_vars.at(i) = from_var + ".x";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
                input_vars.at(i) = from_var + ".x";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
                input_vars.at(i) = from_var + ".x";
              } break;
              default:
                break;
            }
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT: {
            switch (from_port_type) {
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR: {
                input_vars.at(i) = "int(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT: {
                input_vars.at(i) = "int(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN: {
                input_vars.at(i) = "(" + from_var + " ? 1 : 0)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
                input_vars.at(i) = "int(" + from_var + ".x)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
                input_vars.at(i) = "int(" + from_var + ".x)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
                input_vars.at(i) = "int(" + from_var + ".x)";
              } break;
              default:
                break;
            }
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT: {
            switch (from_port_type) {
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR: {
                input_vars.at(i) = "uint(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT: {
                input_vars.at(i) = "uint(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN: {
                input_vars.at(i) = "(" + from_var + " ? 1u : 0u)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
                input_vars.at(i) = "uint(" + from_var + ".x)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
                input_vars.at(i) = "uint(" + from_var + ".x)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
                input_vars.at(i) = "uint(" + from_var + ".x)";
              } break;
              default:
                break;
            }
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN: {
            switch (from_port_type) {
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR: {
                input_vars.at(i) = from_var + " > 0.0 ? true : false";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT: {
                input_vars.at(i) = from_var + " > 0 ? true : false";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT: {
                input_vars.at(i) = from_var + " > 0u ? true : false";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
                input_vars.at(i) = "all(bvec2(" + from_var + "))";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
                input_vars.at(i) = "all(bvec3(" + from_var + "))";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
                input_vars.at(i) = "all(bvec4(" + from_var + "))";
              } break;
              default:
                break;
            }
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
            switch (from_port_type) {
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR: {
                input_vars.at(i) = "vec2(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT: {
                input_vars.at(i) = "vec2(float(" + from_var + "))";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT: {
                input_vars.at(i) = "vec2(float(" + from_var + "))";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN: {
                input_vars.at(i) = "vec2(" + from_var + " ? 1.0 : 0.0)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D:
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
                input_vars.at(i) = "vec2(" + from_var + ".xy)";
              } break;
              default:
                break;
            }
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
            switch (from_port_type) {
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR: {
                input_vars.at(i) = "vec3(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT: {
                input_vars.at(i) = "vec3(float(" + from_var + "))";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT: {
                input_vars.at(i) = "vec3(float(" + from_var + "))";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN: {
                input_vars.at(i) = "vec3(" + from_var + " ? 1.0 : 0.0)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
                input_vars.at(i) = "vec3(" + from_var + ", 0.0)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
                input_vars.at(i) = "vec3(" + from_var + ".xyz)";
              } break;
              default:
                break;
            }
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
            switch (from_port_type) {
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR: {
                input_vars.at(i) = "vec4(" + from_var + ")";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT: {
                input_vars.at(i) = "vec4(float(" + from_var + "))";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT: {
                input_vars.at(i) = "vec4(float(" + from_var + "))";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN: {
                input_vars.at(i) = "vec4(" + from_var + " ? 1.0 : 0.0)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
                input_vars.at(i) = "vec4(" + from_var + ", 0.0, 0.0)";
              } break;
              case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
                input_vars.at(i) = "vec4(" + from_var + ", 0.0)";
              } break;
              default:
                break;
            }
          } break;
          default:
            break;
        }  // end of switch (to_port_type)
      }  // end of if (to_port_type == from_port_type)
    } else {
      // Add the default value.

      TVariant defval{n->get_input_port_default_value(i)};

      switch (defval.index()) {
        case TVARIANT_FLOAT_INDEX: {  // float
          float val{std::get<float>(defval)};
          input_vars.at(i) = "var_to_n" + std::to_string(node_id) + "_p" + std::to_string(i);
          std::ostringstream oss;
          oss << std::string("\t") + "float " << input_vars.at(i) << " = " << std::fixed << std::setprecision(5) << val
              << ";" << std::string("\n");
          node_code += oss.str();
        } break;
        case TVARIANT_INT_INDEX: {  // int
          int val{std::get<int>(defval)};
          input_vars.at(i) = "var_to_n" + std::to_string(node_id) + "_p" + std::to_string(i);
          if (n->get_input_port_type(i) == VisualShaderNode::PORT_TYPE_SCALAR_UINT) {
            node_code +=
                std::string("\t") + "uint " + input_vars.at(i) + " = " + std::to_string(val) + "u;" + std::string("\n");
          } else {
            node_code +=
                std::string("\t") + "int " + input_vars.at(i) + " = " + std::to_string(val) + ";" + std::string("\n");
          }
        } break;
        case TVARIANT_VECTOR_2D_INDEX: {  // TVector2
          TVector2 val{std::get<TVector2>(defval)};
          input_vars.at(i) = "var_to_n" + std::to_string(node_id) + "_p" + std::to_string(i);
          std::ostringstream oss;
          oss << std::string("\t") + "vec2 " << input_vars.at(i) << " = " << std::fixed << std::setprecision(5)
              << "vec2(" << val.x << ", " << val.y << ");" << std::string("\n");
          node_code += oss.str();
        } break;
        case TVARIANT_VECTOR_3D_INDEX: {  // TVector3
          TVector3 val{std::get<TVector3>(defval)};
          input_vars.at(i) = "var_to_n" + std::to_string(node_id) + "_p" + std::to_string(i);
          std::ostringstream oss;
          oss << std::string("\t") + "vec3 " << input_vars.at(i) << " = " << std::fixed << std::setprecision(5)
              << "vec3(" << val.x << ", " << val.y << ", " << val.z << ");" << std::string("\n");
          node_code += oss.str();
        } break;
        case TVARIANT_VECTOR_4D_INDEX: {  // TVector4
          TVector4 val{std::get<TVector4>(defval)};
          input_vars.at(i) = "var_to_n" + std::to_string(node_id) + "_p" + std::to_string(i);
          std::ostringstream oss;
          oss << std::string("\t") + "vec4 " << input_vars.at(i) << " = " << std::fixed << std::setprecision(5)
              << "vec4(" << val.x << ", " << val.y << ", " << val.z << ", " << val.w << ");" << std::string("\n");
          node_code += oss.str();
        } break;
        case TVARIANT_BOOL_INDEX: {  // bool
          bool val{std::get<bool>(defval)};
          input_vars.at(i) = "var_to_n" + std::to_string(node_id) + "_p" + std::to_string(i);
          node_code += std::string("\t") + "bool " + input_vars.at(i) + " = " + (val ? "true" : "false") + ";" +
                       std::string("\n");
        } break;
        default:
          break;
      }  // end of switch
    }  // end of else
  }  // end of for (int i = 0; i < input_port_count; i++)

  int output_port_count{n->get_output_port_count()};
  int initial_output_count{output_port_count};

  std::unordered_map<int, bool> expanded_output_ports;

  for (int i{0}; i < initial_output_count; i++) {
    bool expanded{false};

    if (n->is_output_port_expandable(i) && n->is_output_port_expanded(i)) {
      expanded = true;

      switch (n->get_output_port_type(i)) {
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
          output_port_count += 2;
        } break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
          output_port_count += 3;
        } break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
          output_port_count += 4;
        } break;
        default:
          break;
      }
    }
    expanded_output_ports[i] = expanded;
  }

  std::vector<std::string> output_vars;
  output_vars.resize(output_port_count);

  if (n->is_simple_decl()) {
    // Generate less code for some simple_decl nodes.
    for (int i{0}, j{0}; i < initial_output_count; i++, j++) {
      std::string to_var{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(j)};
      switch (n->get_output_port_type(i)) {
        case VisualShaderNode::PortType::PORT_TYPE_SCALAR:
          output_vars.at(i) = "float " + to_var;
          break;
        case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT:
          output_vars.at(i) = "int " + to_var;
          break;
        case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT:
          output_vars.at(i) = "uint " + to_var;
          break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D:
          output_vars.at(i) = "vec2 " + to_var;
          break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D:
          output_vars.at(i) = "vec3 " + to_var;
          break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D:
          output_vars.at(i) = "vec4 " + to_var;
          break;
        case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN:
          output_vars.at(i) = "bool " + to_var;
          break;
        default:
          break;
      }
      if (expanded_output_ports[i]) {
        switch (n->get_output_port_type(i)) {
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
            j += 2;
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
            j += 3;
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
            j += 4;
          } break;
          default:
            break;
        }
      }
    }

  } else {
    for (int i{0}, j{0}; i < initial_output_count; i++, j++) {
      output_vars.at(i) = "var_from_n" + std::to_string(node_id) + "_p" + std::to_string(j);
      switch (n->get_output_port_type(i)) {
        case VisualShaderNode::PortType::PORT_TYPE_SCALAR:
          func_code += std::string("\t") + "float " + output_vars.at(i) + ";" + std::string("\n");
          break;
        case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT:
          func_code += std::string("\t") + "int " + output_vars.at(i) + ";" + std::string("\n");
          break;
        case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT:
          func_code += std::string("\t") + "uint " + output_vars.at(i) + ";" + std::string("\n");
          break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D:
          func_code += std::string("\t") + "vec2 " + output_vars.at(i) + ";" + std::string("\n");
          break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D:
          func_code += std::string("\t") + "vec3 " + output_vars.at(i) + ";" + std::string("\n");
          break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D:
          func_code += std::string("\t") + "vec4 " + output_vars.at(i) + ";" + std::string("\n");
          break;
        case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN:
          func_code += std::string("\t") + "bool " + output_vars.at(i) + ";" + std::string("\n");
          break;
        default:
          break;
      }
      if (expanded_output_ports[i]) {
        switch (n->get_output_port_type(i)) {
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
            j += 2;
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
            j += 3;
          } break;
          case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
            j += 4;
          } break;
          default:
            break;
        }
      }
    }
  }

  node_code += n->generate_code(node_id, input_vars, output_vars);

  if (!node_code.empty()) {
    // Add the node code to the function code buffer.
    func_code += node_name + node_code;
  }

  for (int i{0}; i < output_port_count; i++) {
    if (!expanded_output_ports[i]) {
      continue;
    }

    switch (n->get_output_port_type(i)) {
      case VisualShaderNode::PORT_TYPE_VECTOR_2D: {
        if (n->is_output_port_connected(i + 1)) {
          // Red component.
          std::string r{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 1)};
          func_code += std::string("\t") + "float " + r + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".r;" + std::string("\n");
          output_vars.at(i + 1) = r;
        }

        if (n->is_output_port_connected(i + 2)) {
          // Green component.
          std::string g{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 2)};
          func_code += std::string("\t") + "float " + g + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".g;" + std::string("\n");
          output_vars.at(i + 2) = g;
        }

        i += 2;
      } break;
      case VisualShaderNode::PORT_TYPE_VECTOR_3D: {
        if (n->is_output_port_connected(i + 1)) {
          // Red component.
          std::string r{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 1)};
          func_code += std::string("\t") + "float " + r + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".r;" + std::string("\n");
          output_vars.at(i + 1) = r;
        }

        if (n->is_output_port_connected(i + 2)) {
          // Green component.
          std::string g{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 2)};
          func_code += std::string("\t") + "float " + g + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".g;" + std::string("\n");
          output_vars.at(i + 2) = g;
        }

        if (n->is_output_port_connected(i + 3)) {
          // Blue component.
          std::string b{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 3)};
          func_code += std::string("\t") + "float " + b + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".b;" + std::string("\n");
          output_vars.at(i + 3) = b;
        }

        i += 3;
      } break;
      case VisualShaderNode::PORT_TYPE_VECTOR_4D: {
        if (n->is_output_port_connected(i + 1)) {
          // Red component.
          std::string r{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 1)};
          func_code += std::string("\t") + "float " + r + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".r;" + std::string("\n");
          output_vars.at(i + 1) = r;
        }

        if (n->is_output_port_connected(i + 2)) {
          // Green component.
          std::string g{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 2)};
          func_code += std::string("\t") + "float " + g + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".g;" + std::string("\n");
          output_vars.at(i + 2) = g;
        }

        if (n->is_output_port_connected(i + 3)) {
          // Blue component.
          std::string b{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 3)};
          func_code += std::string("\t") + "float " + b + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".b;" + std::string("\n");
          output_vars.at(i + 3) = b;
        }

        if (n->is_output_port_connected(i + 4)) {
          // Alpha component.
          std::string a{"var_from_n" + std::to_string(node_id) + "_p" + std::to_string(i + 4)};
          func_code += std::string("\t") + "float " + a + " = var_from_n" + std::to_string(node_id) + "_" +
                       std::to_string(i) + ".a;" + std::string("\n");
          output_vars.at(i + 4) = a;
        }

        i += 4;
      } break;
      default:
        break;
    }
  }

  if (!node_code.empty()) {
    func_code += "\n\n";
  }

  processed.insert(node_id);

  return true;
}

std::string VisualShader::get_code() const { return VisualShader::code; }

std::vector<int> VisualShader::get_nodes() const {
  const VisualShader::Graph* g{&graph};

  std::vector<int> used_ids;

  for (const std::pair<const int, VisualShader::Node>& p : g->nodes) {
    used_ids.emplace_back(p.first);
  }

  return used_ids;
}

std::vector<VisualShader::Connection> VisualShader::get_connections() const {
  const VisualShader::Graph* g{&graph};

  return g->connections;
}

void VisualShader::set_code(const std::string& code) const { VisualShader::code = code; }

VisualShaderNode::VisualShaderNode() : simple_decl(true) {
  // std::cout << "VisualShaderNode::VisualShaderNode()" << std::endl;
}

bool VisualShaderNode::is_simple_decl() const { return simple_decl; }

std::string VisualShaderNode::generate_global([[maybe_unused]] const int& id) const { return std::string(); }

std::string VisualShaderNode::generate_global_per_node([[maybe_unused]] const int& id) const { return std::string(); }

std::string VisualShaderNode::generate_global_per_func([[maybe_unused]] const int& id) const { return std::string(); }

int VisualShaderNode::get_default_input_port([[maybe_unused]] const VisualShaderNode::PortType& type) const {
  return 0;
}

bool VisualShaderNode::is_input_port_default([[maybe_unused]] const int& port) const { return false; }

void VisualShaderNode::set_input_port_default_value(const int& port, const TVariant& value,
                                                    const TVariant& prev_value) {
  TVariant v{value};

  if (prev_value.index() != 0) {  // std::monostate
    switch (value.index()) {
      case TVARIANT_FLOAT_INDEX: {  // float
        switch (prev_value.index()) {
          case TVARIANT_FLOAT_INDEX: {  // float
            v = prev_value;
          } break;
          case TVARIANT_INT_INDEX: {  // int
            v = std::get<float>(prev_value);
          } break;
          case TVARIANT_VECTOR_2D_INDEX: {  // TVector2
            TVector2 pv{std::get<TVector2>(prev_value)};
            v = pv.x;
          } break;
          case TVARIANT_VECTOR_3D_INDEX: {  // TVector3
            TVector3 pv{std::get<TVector3>(prev_value)};
            v = pv.x;
          } break;
          case TVARIANT_VECTOR_4D_INDEX: {  // TVector4
            TVector4 pv{std::get<TVector4>(prev_value)};
            v = pv.x;
          } break;
          default:
            break;
        }
      } break;
      case TVARIANT_INT_INDEX: {  // int
        switch (prev_value.index()) {
          case TVARIANT_FLOAT_INDEX: {  // float
            v = std::get<int>(prev_value);
          } break;
          case TVARIANT_INT_INDEX: {  // int
            v = prev_value;
          } break;
          case TVARIANT_VECTOR_2D_INDEX: {  // TVector2
            TVector2 pv{std::get<TVector2>(prev_value)};
            v = (int)pv.x;
          } break;
          case TVARIANT_VECTOR_3D_INDEX: {  // TVector3
            TVector3 pv{std::get<TVector3>(prev_value)};
            v = (int)pv.x;
          } break;
          case TVARIANT_VECTOR_4D_INDEX: {  // TVector4
            TVector4 pv{std::get<TVector4>(prev_value)};
            v = (int)pv.x;
          } break;
          default:
            break;
        }
      } break;
      case TVARIANT_VECTOR_2D_INDEX: {  // TVector2
        switch (prev_value.index()) {
          case TVARIANT_FLOAT_INDEX: {  // float
            float pv{std::get<float>(prev_value)};
            v = TVector2(pv, pv);
          } break;
          case TVARIANT_INT_INDEX: {  // int
            float pv{(float)std::get<int>(prev_value)};
            v = TVector2(pv, pv);
          } break;
          case TVARIANT_VECTOR_2D_INDEX: {  // TVector2
            v = prev_value;
          } break;
          case TVARIANT_VECTOR_3D_INDEX: {  // TVector3
            TVector3 pv{std::get<TVector3>(prev_value)};
            v = TVector2(pv.x, pv.y);
          } break;
          case TVARIANT_VECTOR_4D_INDEX: {  // TVector4
            TVector4 pv{std::get<TVector4>(prev_value)};
            v = TVector2(pv.x, pv.y);
          } break;
          default:
            break;
        }
      } break;
      case TVARIANT_VECTOR_3D_INDEX: {  // TVector3
        switch (prev_value.index()) {
          case TVARIANT_FLOAT_INDEX: {  // float
            float pv{std::get<float>(prev_value)};
            v = TVector3(pv, pv, pv);
          } break;
          case TVARIANT_INT_INDEX: {  // int
            float pv{(float)std::get<int>(prev_value)};
            v = TVector3(pv, pv, pv);
          } break;
          case TVARIANT_VECTOR_2D_INDEX: {  // TVector2
            TVector2 pv{std::get<TVector2>(prev_value)};
            v = TVector3(pv.x, pv.y, pv.y);
          } break;
          case TVARIANT_VECTOR_3D_INDEX: {  // TVector3
            v = prev_value;
          } break;
          case TVARIANT_VECTOR_4D_INDEX: {  // TVector4
            TVector4 pv{std::get<TVector4>(prev_value)};
            v = TVector3(pv.x, pv.y, pv.z);
          } break;
          default:
            break;
        }
      } break;
      case TVARIANT_VECTOR_4D_INDEX: {  // TVector4
        switch (prev_value.index()) {
          case TVARIANT_FLOAT_INDEX: {  // float
            float pv{std::get<float>(prev_value)};
            v = TVector4(pv, pv, pv, pv);
          } break;
          case TVARIANT_INT_INDEX: {  // int
            float pv{(float)std::get<int>(prev_value)};
            v = TVector4(pv, pv, pv, pv);
          } break;
          case TVARIANT_VECTOR_2D_INDEX: {  // TVector2
            TVector2 pv{std::get<TVector2>(prev_value)};
            v = TVector4(pv.x, pv.y, pv.y, pv.y);
          } break;
          case TVARIANT_VECTOR_3D_INDEX: {  // TVector3
            TVector3 pv{std::get<TVector3>(prev_value)};
            v = TVector4(pv.x, pv.y, pv.z, pv.z);
          } break;
          case TVARIANT_VECTOR_4D_INDEX: {  // TVector4
            v = prev_value;
          } break;
          default:
            break;
        }
      } break;
      default:
        break;
    }
  }

  default_input_values[port] = v;
}

TVariant VisualShaderNode::get_input_port_default_value(const int& port) const {
  if (VisualShaderNode::default_input_values.find(port) == VisualShaderNode::default_input_values.end()) {
    return TVariant();
  }

  return default_input_values.at(port);
}

bool VisualShaderNode::is_show_prop_names() const { return false; }

bool VisualShaderNode::is_output_port_expandable(const int& port) const {
  VisualShaderNode::PortType p{get_output_port_type(port)};
  if (get_output_port_count() == 1 &&
      (p == VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D || p == VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D ||
       p == VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D)) {
    return true;
  }
  return false;
}

bool VisualShaderNode::is_output_port_expanded(const int& port) const {
  if (VisualShaderNode::expanded_output_ports.find(port) == VisualShaderNode::expanded_output_ports.end()) {
    return false;
  }
  return VisualShaderNode::expanded_output_ports.at(port);
}

int VisualShaderNode::get_expanded_output_port_count() const {
  int count{get_output_port_count()};
  int c{count};
  for (int i{0}; i < count; i++) {
    if (VisualShaderNode::is_output_port_expandable(i) && VisualShaderNode::is_output_port_expanded(i)) {
      switch (get_output_port_type(i)) {
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D: {
          c += 2;
        } break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D: {
          c += 3;
        } break;
        case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D: {
          c += 4;
        } break;
        default:
          break;
      }
    }
  }
  return c;
}

void VisualShaderNode::set_output_ports_expanded(const std::vector<int>& values) {
  for (size_t i{0}; i < values.size(); i++) {
    VisualShaderNode::expanded_output_ports.at(values.at(i)) = true;
  }
}

std::vector<int> VisualShaderNode::get_output_ports_expanded() const {
  std::vector<int> output_ports_expanded;
  for (int i{0}; i < get_output_port_count(); i++) {
    if (is_output_port_expanded(i)) {
      output_ports_expanded.emplace_back(i);
    }
  }
  return output_ports_expanded;
}

void VisualShaderNode::set_output_port_expanded(const int& port, const bool& expanded) {
  VisualShaderNode::expanded_output_ports.at(port) = expanded;
}

bool VisualShaderNode::is_output_port_connected(const int& port) const {
  if (VisualShaderNode::connected_output_ports.find(port) == VisualShaderNode::connected_output_ports.end()) {
    return false;
  }
  return VisualShaderNode::connected_output_ports.at(port) > 0;
}

void VisualShaderNode::set_output_port_connected(const int& port, const bool& connected) {
  if (connected) {
    if (VisualShaderNode::connected_output_ports.find(port) == VisualShaderNode::connected_output_ports.end()) {
      VisualShaderNode::connected_output_ports[port] = 1;  // Initialize to 1.
    } else {
      VisualShaderNode::connected_output_ports.at(port)++;
    }
  } else {
    if (VisualShaderNode::connected_output_ports.find(port) != VisualShaderNode::connected_output_ports.end()) {
      VisualShaderNode::connected_output_ports.at(port)--;
    }
  }
}

bool VisualShaderNode::is_input_port_connected(const int& port) const {
  if (VisualShaderNode::connected_input_ports.find(port) == VisualShaderNode::connected_input_ports.end()) {
    return false;
  }
  return VisualShaderNode::connected_input_ports.at(port);
}

void VisualShaderNode::set_input_port_connected(const int& port, const bool& connected) {
  VisualShaderNode::connected_input_ports[port] = connected;
}

bool VisualShaderNode::has_output_port_preview([[maybe_unused]] const int& port) const { return true; }

VisualShaderNode::Category VisualShaderNode::get_category() const {
  std::cout << get_caption() + " is missing a category." << std::endl;
  return VisualShaderNode::Category::CATEGORY_NONE;
}

std::vector<std::string> VisualShaderNode::get_editable_properties() const { return std::vector<std::string>(); }

std::unordered_map<std::string, std::string> VisualShaderNode::get_editable_properties_names() const {
  return std::unordered_map<std::string, std::string>();
}

std::string VisualShaderNode::get_warning() const { return std::string(); }

/*************************************/
/* Input Node                        */
/*************************************/

VisualShaderNodeInput::VisualShaderNodeInput() : input_name("[None]") {
  // std::cout << "VisualShaderNodeInput::VisualShaderNodeInput()" << std::endl;
}

const VisualShaderNodeInput::Port VisualShaderNodeInput::ports[] = {

    {VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "uv", "UV"},
    {VisualShaderNode::PortType::PORT_TYPE_SCALAR, "time", "TIME"},

    {VisualShaderNode::PORT_TYPE_ENUM_SIZE, "", ""},  // End of list.

};

const VisualShaderNodeInput::Port VisualShaderNodeInput::preview_ports[] = {

    {VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "uv", "UV"},
    {VisualShaderNode::PortType::PORT_TYPE_SCALAR, "time", "TIME"},

    {VisualShaderNode::PORT_TYPE_ENUM_SIZE, "", ""},  // End of list.

};

std::string VisualShaderNodeInput::get_caption() const { return "Input"; }

VisualShaderNode::PortType VisualShaderNodeInput::get_input_type_by_name(const std::string& name) const {
  const VisualShaderNodeInput::Port* p{VisualShaderNodeInput::ports};

  int i{0};

  while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE) {
    if (p[i].name == name) {
      return p[i].type;
    }
    i++;
  }

  return VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeInput::generate_code([[maybe_unused]] const int& id,
                                                 [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                 [[maybe_unused]] const std::vector<std::string>& output_vars) const {
  const VisualShaderNodeInput::Port* p{VisualShaderNodeInput::ports};

  std::string code;

  int i{0};

  while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE) {
    if (p[i].name == input_name) {
      code = std::string("\t") + output_vars.at(0) + " = " + p[i].string_value + ";" + std::string("\n");
      break;
    }
    i++;
  }

  if (code.empty()) {
    code = std::string("\t") + output_vars.at(0) + " = 0.0;" + std::string("\n");
  }

  return code;
}

int VisualShaderNodeInput::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeInput::get_input_port_type([[maybe_unused]] const int& port) const {
  return VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeInput::get_input_port_name([[maybe_unused]] const int& port) const { return ""; }

int VisualShaderNodeInput::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeInput::get_output_port_type(const int& port) const {
  return port == 0 ? VisualShaderNodeInput::get_input_type_by_name(VisualShaderNodeInput::input_name)
                   : VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeInput::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

void VisualShaderNodeInput::set_input_name(const std::string& name) { input_name = name; }

std::string VisualShaderNodeInput::get_input_name() const { return input_name; }

std::string VisualShaderNodeInput::get_input_real_name() const {
  const VisualShaderNodeInput::Port* p{VisualShaderNodeInput::ports};

  int i{0};

  while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE) {
    if (p[i].name == input_name) {
      return p[i].string_value;
    }
    i++;
  }

  return "";
}

/*************************************/
/* Output Node                       */
/*************************************/

VisualShaderNodeOutput::VisualShaderNodeOutput() {
  // std::cout << "VisualShaderNodeOutput::VisualShaderNodeOutput()" <<
  // std::endl;
}

const VisualShaderNodeOutput::Port VisualShaderNodeOutput::ports[] = {

    {VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D, "Color", "COLOR.rgb"},
    {VisualShaderNode::PortType::PORT_TYPE_SCALAR, "Alpha", "COLOR.a"},

    {VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE, "", ""},
};

std::string VisualShaderNodeOutput::get_caption() const { return "Output"; }

std::string VisualShaderNodeOutput::generate_code([[maybe_unused]] const int& id,
                                                  [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                  [[maybe_unused]] const std::vector<std::string>& output_vars) const {
  const VisualShaderNodeOutput::Port* p{VisualShaderNodeOutput::ports};

  std::string code;

  int i{0}, count{0};

  while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE) {
    if (!input_vars.at(count).empty()) {
      std::string s{p[i].string_value};
      code += std::string("\t") + s + " = " + input_vars.at(count) + ";" + std::string("\n");
    }
    count++;
    i++;
  }

  return code;
}

int VisualShaderNodeOutput::get_input_port_count() const {
  const VisualShaderNodeOutput::Port* p{VisualShaderNodeOutput::ports};

  int i{0}, count{0};

  while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE) {
    count++;
    i++;
  }

  return count;
}

VisualShaderNode::PortType VisualShaderNodeOutput::get_input_port_type(const int& port) const {
  const VisualShaderNodeOutput::Port* p{VisualShaderNodeOutput::ports};

  int i{0}, count{0};

  while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE) {
    if (count == port) {
      return p[i].type;
    }
    count++;
    i++;
  }

  return VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeOutput::get_input_port_name(const int& port) const {
  const VisualShaderNodeOutput::Port* p{VisualShaderNodeOutput::ports};

  int i{0}, count{0};

  while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_ENUM_SIZE) {
    if (count == port) {
      return p[i].name;
    }
    count++;
    i++;
  }

  return "";
}

int VisualShaderNodeOutput::get_output_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeOutput::get_output_port_type([[maybe_unused]] const int& port) const {
  return VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeOutput::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }
