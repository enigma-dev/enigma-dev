/** Copyright (C) 2008-2013 Robert B. Colton
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

#ifndef _GLSLSHADER__H
#define _GLSLSHADER__H

#include <vector>
#include <string>
using std::string;
using std::vector;

#include "../General/OpenGLHeaders.h"

extern GLenum shadertypes[];

namespace enigma
{
    struct Shader{
        string log;
        GLuint shader;
        Shader(int type)
        {
            shader = glCreateShader(shadertypes[type]);
        }

        ~Shader()
        {
            glDeleteShader(shader);
        }
    };

    struct ShaderProgram{
        GLuint shaderprogram;

        GLuint uni_viewMatrix;
        GLuint uni_projectionMatrix;
        GLuint uni_modelMatrix;
        GLuint uni_mvMatrix;
        GLuint uni_mvpMatrix;
        GLuint uni_texSampler;

        GLuint uni_textureEnable;
        GLuint uni_colorEnable;

        GLuint uni_color;

        GLuint att_vertex;
        GLuint att_color;
        GLuint att_texture;

        ShaderProgram()
        {
            shaderprogram = glCreateProgram();
        }

        ~ShaderProgram()
        {
            glDeleteProgram(shaderprogram);
        }
    };
}

extern vector<enigma::Shader*> shaders;
extern vector<enigma::ShaderProgram*> shaderprograms;

#endif
