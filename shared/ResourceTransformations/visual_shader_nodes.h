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

#ifndef ENIGMA_VISUAL_SHADER_NODES_H
#define ENIGMA_VISUAL_SHADER_NODES_H

#include "visual_shader.h"

/********************/
/*     TEXTURES     */
/********************/
class VisualShaderNodeTexture : public VisualShaderNode {
    std::shared_ptr<int> texture; // Texture ID or ENIGMA has a texture class?

    public:
        enum Source {
            SOURCE_TEXTURE,
            SOURCE_SCREEN,
            SOURCE_2D_TEXTURE,
            SOURCE_2D_NORMAL,
            SOURCE_DEPTH,
            SOURCE_PORT,
            SOURCE_3D_NORMAL,
            SOURCE_ROUGHNESS,
            SOURCE_MAX,
        };

        enum TextureType {
            TYPE_DATA,
            TYPE_COLOR,
            TYPE_NORMAL_MAP,
            TYPE_MAX,
        };

        VisualShaderNodeTexture();

    private:
        VisualShaderNodeTexture::Source source;
        VisualShaderNodeTexture::TextureType texture_type;
};

#endif //ENIGMA_VISUAL_SHADER_NODES_H
