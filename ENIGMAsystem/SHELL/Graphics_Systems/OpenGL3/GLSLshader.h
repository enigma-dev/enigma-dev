/** Copyright (C) 2008-2013 Robert B. Colton, 2014 Harijs Grinbergs
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
#include <map>
using std::string;
using std::vector;
using std::map;

#include "../General/OpenGLHeaders.h"

extern GLenum shadertypes[];

namespace enigma
{
    struct Shader{
        string log;
        GLuint shader;
        int type;
        Shader(int type) : type(type)
        {
            shader = glCreateShader(shadertypes[type]);
        }

        ~Shader()
        {
            glDeleteShader(shader);
        }
    };

    union UAType{
        unsigned int ui;
        int i;
        float f;
    };

    struct Uniform{
        string name;
        GLint location;
        GLenum type;
        GLint arraySize;
        int size;
        vector<UAType> data;
    };

    struct Attribute{
        string name;
        GLint location;
        GLenum type;
        GLint arraySize;
        int size;
    };

    struct ShaderProgram{
        //These should be unordered maps, but they are only Cx11
        map<string,GLint> uniform_names;
        map<string,GLint> attribute_names;
        map<GLint,Uniform> uniforms;
        map<GLint,Attribute> attributes;
        string log;
		string name;
        GLuint shaderprogram;
        int uniform_count;
        int attribute_count;

        GLint uni_viewMatrix;
        GLint uni_projectionMatrix;
        GLint uni_modelMatrix;
        GLint uni_mvMatrix;
        GLint uni_mvpMatrix;
        GLint uni_normalMatrix;
        GLint uni_texSampler;

        GLint uni_textureEnable;
        GLint uni_colorEnable;
        GLint uni_lightEnable;

        GLint uni_color;

        //3D lights
        GLint uni_ambient_color;
        GLint uni_material_ambient;
        GLint uni_material_diffuse;
        GLint uni_material_specular;
        GLint uni_material_shininess;

        GLint uni_lights_active;
        GLint uni_light_position[8];
        GLint uni_light_ambient[8];
        GLint uni_light_diffuse[8];
        GLint uni_light_specular[8];
        GLint uni_light_cAttenuation[8];
        GLint uni_light_lAttenuation[8];
        GLint uni_light_qAttenuation[8];
        //lights end

        GLint att_vertex;
        GLint att_color;
        GLint att_texture;
        GLint att_normal;

        ShaderProgram()
        {
            shaderprogram = glCreateProgram();
			name = "";
        }

        ~ShaderProgram()
        {
            glDeleteProgram(shaderprogram);
        }
    };
}

//extern vector<enigma::Shader*> shaders;
//extern vector<enigma::ShaderProgram*> shaderprograms;

#endif
