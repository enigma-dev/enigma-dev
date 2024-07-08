/********************************************************************************\
**                                                                              **
**  Copyright (C) 2024 Saif Kandil (k0T0z)                                      **
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

#include "visual_shader.h"

#include <iostream>
#include <algorithm>

VisualShader::VisualShader() {
    // Initialize the graphs with the output node.
    VisualShaderNodeOutput output;
    std::shared_ptr<VisualShaderNode> output_ptr = std::make_shared<VisualShaderNodeOutput>(output);

    VisualShader::graph.nodes[VisualShader::NODE_ID_OUTPUT].node = output_ptr;

    VisualShader::graph.nodes[VisualShader::NODE_ID_OUTPUT].position = {400, 150};
}

int VisualShader::get_valid_node_id() const {
    const VisualShader::Graph *g {&graph};
    // Find the largest key in the map and generate a new ID that is one more than the largest key.
    const int min_valid_id {VisualShader::NODE_ID_OUTPUT+1};
    return g->nodes.size() ? std::max(min_valid_id, g->nodes.rbegin()->first + 1) : min_valid_id;
}

void VisualShader::add_node(const std::shared_ptr<VisualShaderNode>& node, const EVector2& position, const int& id) {
    if (!node) {
        std::cout << "Invalid VisualShaderNode" << std::endl;
        return;
    }

    /* Start the ids from 1 as 0 is reserved for the Output node. */
    if (id < VisualShader::NODE_ID_OUTPUT+1) {
        std::cout << "Invalid VisualShaderNode ID" << std::endl;
        return;
    }

    VisualShader::Graph *g {&graph};

    auto it {g->nodes.find(id)};

    /* If the id already exists, return. */
    if (it != g->nodes.end()) {
        std::cout << "Node ID already exists" << std::endl;
        return;
    }

    Node n;
    n.node = node;
    n.position = position;

    g->nodes[id] = n; // Add the node to the graph.
}

void VisualShader::remove_node(const int& id) {
	/* Start the ids from 1 as 0 is reserved for the Output node. */
    if (id < VisualShader::NODE_ID_OUTPUT+1) {
        std::cout << "Invalid VisualShaderNode ID" << std::endl;
        return;
    }

	Graph *g {&graph};
    
    auto it {g->nodes.find(id)};

    /* If the id is invalid, return. */
    if (it == g->nodes.end()) {
        std::cout << "Node ID already exists" << std::endl;
        return;
    }

	g->nodes.erase(id);

    // Delete any connections that are connected to the node.
    int i {0};

    for (const Connection& c : g->connections) {
        if (c.from_node != id && c.to_node != id) {
            continue;
        }

        if (c.from_node == id) {
            g->nodes[c.to_node].prev_connected_nodes.erase(g->nodes[c.to_node].prev_connected_nodes.begin() + id);
            g->nodes[c.to_node].node->set_input_port_connected(c.to_port, false);
        } else if (c.to_node == id) {
            g->nodes[c.from_node].next_connected_nodes.erase(g->nodes[c.from_node].next_connected_nodes.begin() + id);
        }

        g->connections.erase(g->connections.begin() + i); // Remove the connection c.
        i++;
    }
}

std::shared_ptr<VisualShaderNode> VisualShader::get_node(const int& id) const {
    const VisualShader::Graph *g {&graph};

    auto it {g->nodes.find(id)};

    /* If the id is invalid, return. */
    if (it == g->nodes.end()) {
        std::cout << "Node ID does not exist" << std::endl;
        return std::shared_ptr<VisualShaderNode>();
    }

    return it->second.node;
}

bool VisualShader::generate_shader() const {
    static const std::string func_name { "fragment" };

    std::string shader_code;

    // Store the connections for faster generation.
    // VMap<ConnectionKey, const List<Connection>::Element *> input_connections;
	// VMap<ConnectionKey, const List<Connection>::Element *> output_connections;

    // Store the connections for faster generation.
    std::map<ConnectionKey, const Connection *> input_connections;
    std::map<ConnectionKey, const Connection *> output_connections;

    std::string func_code;
    std::unordered_set<int> processed;

    for (const Connection& c : graph.connections) {
        ConnectionKey from_key;
        from_key.node = c.from_node;
        from_key.port = c.from_port;

        output_connections.at(from_key) = &c;

        ConnectionKey to_key;
        to_key.node = c.to_node;
        to_key.port = c.to_port;

        input_connections.at(to_key) = &c;
    }

    func_code += "\nvoid " + func_name + "() {\n";

    // Generate the code for each node.
    bool status {generate_shader_for_each_node(func_code, input_connections, output_connections, VisualShader::NODE_ID_OUTPUT, processed)};

    if (!status) {
        std::cout << "Failed to generate shader for node " << VisualShader::NODE_ID_OUTPUT << std::endl;
        return false;
    }

    func_code += "}\n";
    shader_code += func_code;

    std::string generated_code;

    generated_code += shader_code;

    VisualShader::set_code(generated_code);

    return true;
}

bool VisualShader::generate_shader_for_each_node(std::string& func_code, 
                                                    const std::map<ConnectionKey, const Connection *>& input_connections,
                                                    const std::map<ConnectionKey, const Connection *>& output_connections,
                                                    const int& node_id,
                                                    std::unordered_set<int>& processed) const {

    const std::shared_ptr<VisualShaderNode> n {graph.nodes.at(node_id).node};

    // Check inputs recursively.
    int input_port_count {n->get_input_port_count()};
    for (int i {0}; i < input_port_count; i++) {
        ConnectionKey key;
        key.node = node_id;
        key.port = i;

        if (input_connections.find(key) == input_connections.end()) {
            continue;
        }

        const Connection *c {input_connections.at(key)};

        int from_node {c->from_node};

        if (processed.find(from_node) == processed.end()) {
            continue;
        }

        bool status {generate_shader_for_each_node(func_code, input_connections, output_connections, from_node, processed)};
        if (!status) {
            std::cout << "Failed to generate shader for node " << from_node << std::endl;
            return false;
        }
    }

    // Generate the code for the current node.
    std::string node_name {"// " + n->get_caption() + ":" + std::to_string(node_id) + "\n"};
    std::string node_code;
    std::vector<std::string> input_vars;

    input_vars.resize(n->get_input_port_count());

    for (int i {0}; i < input_port_count ; i++) {
        ConnectionKey key;
        key.node = node_id;
        key.port = i;

        // Check if the input is not connected.
        if (input_connections.find(key) != input_connections.end()) {
            // Add the default value.
            continue;
        }

        const Connection *c {input_connections.at(key)};

        int from_node {c->from_node};
        int from_port {c->from_port};

        VisualShaderNode::PortType to_port_type {n->get_input_port_type(i)};
        VisualShaderNode::PortType from_port_type {graph.nodes.at(from_node).node->get_output_port_type(from_port)};

        std::string from_var {"var_from_" + std::to_string(from_node) + "_" + std::to_string(from_port)};

        if (to_port_type == from_port_type) {
            input_vars.at(i) = from_var;
        } else {
            switch (to_port_type) {
                case VisualShaderNode::PortType::PORT_TYPE_SCALAR:
                    {
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
                    }
                    break;
                case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT:
                    {
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
                    } 
                    break;
                case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT:
                    {
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
                    }
                    break;
                case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN:
                    {
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
                    }
                    break;
                case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D:
                    {
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
                    }
                    break;
                case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D:
                    {
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
                    }
                    break;
                case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D:
                    {
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
                    }
                    break;
                default:
                    break;
            }
        }
    }

    int output_port_count {n->get_output_port_count()};
    int initial_output_count {output_port_count};

    std::unordered_map<int, bool> expanded_output_ports;

    for (int i {0}; i < initial_output_count; i++) {
		bool expanded {false};

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
		expanded_output_ports.at(i) = expanded;
	}

    std::vector<std::string> output_vars;
    output_vars.resize(output_port_count);

    if (n->is_simple_decl()) {
        // Generate less code for some simple_decl nodes.
		for (int i {0}, j {0}; i < initial_output_count; i++, j++) {
			std::string to_var {"var_to_" + std::to_string(node_id) + "_" + std::to_string(j)};
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
				case VisualShaderNode::PortType::PORT_TYPE_TRANSFORM:
					output_vars.at(i) = "mat4 " + to_var;
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
		for (int i {0}, j {0}; i < initial_output_count; i++, j++) {
			output_vars.at(i) = "var_to_" + std::to_string(node_id) + "_" + std::to_string(j);
			switch (n->get_output_port_type(i)) {
				case VisualShaderNode::PortType::PORT_TYPE_SCALAR:
					func_code += "	float " + output_vars.at(i) + ";\n";
					break;
				case VisualShaderNode::PortType::PORT_TYPE_SCALAR_INT:
					func_code += "	int " + output_vars.at(i) + ";\n";
					break;
				case VisualShaderNode::PortType::PORT_TYPE_SCALAR_UINT:
					func_code += "	uint " + output_vars.at(i) + ";\n";
					break;
				case VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D:
					func_code += "	vec2 " + output_vars.at(i) + ";\n";
					break;
				case VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D:
					func_code += "	vec3 " + output_vars.at(i) + ";\n";
					break;
				case VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D:
					func_code += "	vec4 " + output_vars.at(i) + ";\n";
					break;
				case VisualShaderNode::PortType::PORT_TYPE_BOOLEAN:
					func_code += "	bool " + output_vars.at(i) + ";\n";
					break;
				case VisualShaderNode::PortType::PORT_TYPE_TRANSFORM:
					func_code += "	mat4 " + output_vars.at(i) + ";\n";
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

    node_code = n->generate_code(node_id, input_vars, output_vars);

    if (!node_code.empty()) {
        // Add the node code to the function code buffer.
        func_code += node_name + node_code;
    }

    for (int i {0} ; i < output_port_count; i++) {
        if (!expanded_output_ports[i]) {
            continue;
        }

        switch (n->get_output_port_type(i)) {
            case VisualShaderNode::PORT_TYPE_VECTOR_2D: {
                if (n->is_output_port_connected(i + 1)) {
                    // Red component.
                    std::string r {"var_to_" + std::to_string(node_id) + "" + std::to_string(i + 1)};
                    func_code += "	float " + r + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".r;\n";
                    output_vars.at(i + 1) = r;
                }

                if (n->is_output_port_connected(i + 2)) {
                    // Green component.
                    std::string g {"var_to_" + std::to_string(node_id) + "" + std::to_string(i + 2)};
                    func_code += "	float " + g + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".g;\n";
                    output_vars.at(i + 2) = g;
                }

                i += 2;
            } break;
            case VisualShaderNode::PORT_TYPE_VECTOR_3D: {
                if (n->is_output_port_connected(i + 1)) {
                    // Red component.
                    std::string r {"var_to_" + std::to_string(node_id) + "" + std::to_string(i + 1)};
                    func_code += "	float " + r + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".r;\n";
                    output_vars.at(i + 1) = r;
                }

                if (n->is_output_port_connected(i + 2)) {
                    // Green component.
                    std::string g {"var_to_" + std::to_string(node_id) + "" + std::to_string(i + 2)};
                    func_code += "	float " + g + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".g;\n";
                    output_vars.at(i + 2) = g;
                }

                if (n->is_output_port_connected(i + 3)) {
                    // Blue component.
                    std::string b { "var_to_" + std::to_string(node_id) + "" + std::to_string(i + 3)};
                    func_code += "	float " + b + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".b;\n";
                    output_vars.at(i + 3) = b;
                }

                i += 3;
            } break;
            case VisualShaderNode::PORT_TYPE_VECTOR_4D: {
                if (n->is_output_port_connected(i + 1)) {
                    // Red component.
                    std::string r {"var_to_" + std::to_string(node_id) + "" + std::to_string(i + 1)};
                    func_code += "	float " + r + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".r;\n";
                    output_vars.at(i + 1) = r;
                }

                if (n->is_output_port_connected(i + 2)) {
                    // Green component.
                    std::string g {"var_to_" + std::to_string(node_id) + "" + std::to_string(i + 2)};
                    func_code += "	float " + g + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".g;\n";
                    output_vars.at(i + 2) = g;
                }

                if (n->is_output_port_connected(i + 3)) {
                    // Blue component.
                    std::string b { "var_to_" + std::to_string(node_id) + "" + std::to_string(i + 3)};
                    func_code += "	float " + b + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".b;\n";
                    output_vars.at(i + 3) = b;
                }

                if (n->is_output_port_connected(i + 4)) {
                    // Alpha component.
                    std::string a {"var_to_" + std::to_string(node_id) + "" + std::to_string(i + 4)};
                    func_code += "	float " + a + " = var_to_" + std::to_string(node_id) + "" + std::to_string(i) + ".a;\n";
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

std::string VisualShader::get_code() const {
    return VisualShader::code;
}

void VisualShader::set_code(const std::string& code) const {
    VisualShader::code = code;
}

VisualShaderNode::VisualShaderNode() : simple_decl(true) {
    // std::cout << "VisualShaderNode::VisualShaderNode()" << std::endl;
}

bool VisualShaderNode::is_simple_decl() const {
	return simple_decl;
}

bool VisualShaderNode::is_output_port_expandable(const int& port) const {
	VisualShaderNode::PortType p {get_output_port_type(port)};
	if (get_output_port_count() == 1 && 
            (p == VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D || 
             p == VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D || 
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
	int count {get_output_port_count()};
	int c {count};
	for (int i {0}; i < count; i++) {
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
	for (size_t i {0}; i < values.size(); i++) {
		VisualShaderNode::expanded_output_ports.at(values.at(i)) = true;
	}
}

std::vector<int> VisualShaderNode::get_output_ports_expanded() const {
	std::vector<int> output_ports_expanded;
	for (int i {0}; i < get_output_port_count(); i++) {
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
		VisualShaderNode::connected_output_ports.at(port)++;
	} else {
		VisualShaderNode::connected_output_ports.at(port)--;
	}
}

bool VisualShaderNode::is_input_port_connected(const int& port) const {
	if (VisualShaderNode::connected_input_ports.find(port) == VisualShaderNode::connected_input_ports.end()) {
		return false;
	}
	return VisualShaderNode::connected_input_ports.at(port);
}

void VisualShaderNode::set_input_port_connected(const int& port, const bool& connected) {
	VisualShaderNode::connected_input_ports.at(port) = connected;
}

VisualShaderNodeInput::VisualShaderNodeInput() : input_name("[None]") {
    // std::cout << "VisualShaderNodeInput::VisualShaderNodeInput()" << std::endl;
}

std::string VisualShaderNodeInput::get_caption() const {
    return "Input";
}

VisualShaderNode::PortType VisualShaderNodeInput::get_input_type_by_name(const std::string& name) const {
    const VisualShaderNodeInput::Port* p {VisualShaderNodeInput::ports};

    int i {0};

    while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_MAX) {
        if (p[i].name == name) {
            return p[i].type;
        }
        i++;
    }

    return VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeInput::generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, [[maybe_unused]] const std::vector<std::string>& output_vars) const {
    std::string code;

    return code;
}

int VisualShaderNodeInput::get_input_port_count() const {
    return 0;
}

VisualShaderNode::PortType VisualShaderNodeInput::get_input_port_type([[maybe_unused]] const int& port) const {
    return VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeInput::get_input_port_name([[maybe_unused]] const int& port) const {
    return "";
}

int VisualShaderNodeInput::get_output_port_count() const {
    return 1;
}

VisualShaderNode::PortType VisualShaderNodeInput::get_output_port_type(const int& port) const {
    return port == 0 ? VisualShaderNodeInput::get_input_type_by_name(VisualShaderNodeInput::input_name) : VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeInput::get_output_port_name([[maybe_unused]] const int& port) const {
    return "";
}

VisualShaderNodeOutput::VisualShaderNodeOutput() {
    // std::cout << "VisualShaderNodeOutput::VisualShaderNodeOutput()" << std::endl;
}

std::string VisualShaderNodeOutput::get_caption() const {
    return "Output";
}

std::string VisualShaderNodeOutput::generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, [[maybe_unused]] const std::vector<std::string>& output_vars) const {
    std::string code;

    return code;
}

int VisualShaderNodeOutput::get_input_port_count() const {
    const VisualShaderNodeOutput::Port* p {VisualShaderNodeOutput::ports};

    int i {0}, count {0};

    while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_MAX) {
        count++;
        i++;
    }

    return count;
}

VisualShaderNode::PortType VisualShaderNodeOutput::get_input_port_type(const int& port) const {
    const VisualShaderNodeOutput::Port* p {VisualShaderNodeOutput::ports};

    int i {0}, count {0};

    while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_MAX) {
        if (count == port) {
            return p[i].type;
        }
        count++;
        i++;
    }

    return VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeOutput::get_input_port_name(const int& port) const {
    const VisualShaderNodeOutput::Port* p {VisualShaderNodeOutput::ports};

    int i {0}, count {0};

    while (p[i].type != VisualShaderNode::PortType::PORT_TYPE_MAX) {
        if (count == port) {
            return p[i].name;
        }
        count++;
        i++;
    }

    return "";
}

int VisualShaderNodeOutput::get_output_port_count() const {
    return 0;
}

VisualShaderNode::PortType VisualShaderNodeOutput::get_output_port_type([[maybe_unused]] const int& port) const {
    return VisualShaderNode::PortType::PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeOutput::get_output_port_name([[maybe_unused]] const int& port) const {
    return "";
}
