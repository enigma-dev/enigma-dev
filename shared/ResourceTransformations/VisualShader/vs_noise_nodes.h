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

#ifndef ENIGMA_VISUAL_SHADER_NOISE_NODES_H
#define ENIGMA_VISUAL_SHADER_NOISE_NODES_H

#include "visual_shader.h"

/*************************************/
/* Value Noise                       */
/*************************************/

class VisualShaderNodeValueNoise : public VisualShaderNode {
 public:
  VisualShaderNodeValueNoise();

  virtual std::string get_caption() const override;

  virtual int get_input_port_count() const override;
  virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
  virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

  virtual int get_output_port_count() const override;
  virtual VisualShaderNode::PortType get_output_port_type([[maybe_unused]] const int& port) const override;
  virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

  virtual std::string generate_global([[maybe_unused]] const int& id) const override;
  virtual std::string generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars,
                                    const std::vector<std::string>& output_vars) const override;

  void set_scale(const float& s);
  float get_scale() const;

  virtual std::vector<std::string> get_editable_properties() const override;

 private:
  float scale;
};

#endif  // ENIGMA_VISUAL_SHADER_NOISE_NODES_H
