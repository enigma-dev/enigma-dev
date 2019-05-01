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

#ifndef ENIGMA_GLSLSHADER_H
#define ENIGMA_GLSLSHADER_H

//#include <functional> //For std::hash
#include <vector>
#include <string>
#include <unordered_map>
using std::string;
using std::vector;
using std::unordered_map;

#include "OpenGLHeaders.h"

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
        string name = "";
        GLint location = -1;
        GLenum type = GL_INVALID_VALUE;
        GLint arraySize = -1;
        int size = -1;
        vector<UAType> data;
    };

    struct Attribute{
        string name = "";
        GLint location = -1;
        GLenum type = GL_INVALID_VALUE;
        GLint arraySize = -1;
        int size = -1;
        bool enabled = false;
        bool normalize = false;
        int stride = -1;
        int offset = -1;
        int datatype = 0; //Difference is that "type" is the type inside the shader (like float for color), but "datatype" is the type for data (unsigned int for color)
        int datasize = -1;
        int vao = -1;
    };

    //This holds attribute parameters and works as a OpenGL VAO
    /*struct AttributeObject{
        GLuint vertexArrayObject;

        AttributeObject(){ glGenVertexArrays(1, &vertexArrayObject); }
        ~AttributeObject(){ glDeleteBuffers(1, &vertexArrayObject); }
        map<string,GLint> attribute_names;
        map<GLint,Attribute> attributes;
    };*/

    struct ShaderProgram{
        unordered_map<string,GLint> uniform_names;
        unordered_map<string,GLint> attribute_names;
        unordered_map<GLint,Uniform> uniforms;
        unordered_map<GLint,Attribute> attributes;
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
        GLint uni_alphaTestEnable;

        GLint uni_color;
        GLint uni_alphaTest;

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

//Specialize std::hash for attribute hashing
/*namespace std
{
    template<>
    struct hash<Attribute>
    {
        typedef Attribute argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& s) const
        {
            result_type const h1 ( std::hash<std::string>()(s.name) );
            result_type const h2 ( std::hash<std::string>()(s.location) );
            return h1 ^ (h2 << 1);
        }
    };
}*/

//extern vector<enigma::Shader*> shaders;
//extern vector<enigma::ShaderProgram*> shaderprograms;

#endif
