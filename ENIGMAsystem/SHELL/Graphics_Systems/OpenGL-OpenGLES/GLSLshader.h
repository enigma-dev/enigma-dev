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

#include "OpenGLHeaders.h"

//#include <functional> //For std::hash
#include <vector>
#include <string>
#include <unordered_map>
using std::string;
using std::vector;
using std::unordered_map;

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

    extern vector<enigma::Shader*> shaders;
    extern vector<enigma::ShaderProgram*> shaderprograms;

    extern unsigned bound_shader;
    bool UATypeUIComp(UAType i, unsigned int j);
    bool UATypeIComp(UAType i, int j);
    bool UATypeFComp(UAType i, float j);
}

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_uniform(uniter,location,usize)\
    string name = enigma::shaderprograms[enigma::bound_shader]->name;\
    char str[128];\
    if (name == ""){\
        sprintf(str, "Program[%i]", enigma::bound_shader);\
    }else{\
        sprintf(str, "Program[%s = %i]", name.c_str(), enigma::bound_shader);\
    }\
    if (location < 0) { printf("%s - Uniform location < 0 given (%i)!\n", str, location); return; }\
    std::unordered_map<GLint,enigma::Uniform>::iterator uniter = enigma::shaderprograms[enigma::bound_shader]->uniforms.find(location);\
    if (uniter == enigma::shaderprograms[enigma::bound_shader]->uniforms.end()){\
        printf("%s - Uniform at location %i not found!\n", str, location);\
        return;\
    }else if ( uniter->second.size != usize ){\
        printf("%s - Uniform [%s] at location %i with %i arguments is accesed by a function with %i arguments!\n", str, uniter->second.name.c_str(), location, uniter->second.size, usize);\
    }

  #define get_attribute(atiter,location)\
    string name = enigma::shaderprograms[enigma::bound_shader]->name;\
    char str[128];\
    if (name == ""){\
        sprintf(str, "Program[%i]", enigma::bound_shader);\
    }else{\
        sprintf(str, "Program[%s = %i]", name.c_str(), enigma::bound_shader);\
    }\
    if (location < 0) { printf("%s - Attribute location < 0 given (%i)!\n", str, location); return; }\
    std::unordered_map<GLint,enigma::Attribute>::iterator atiter = enigma::shaderprograms[enigma::bound_shader]->attributes.find(location);\
    if (atiter == enigma::shaderprograms[enigma::bound_shader]->attributes.end()){\
        printf("%s - Attribute at location %i not found!\n", str, location);\
        return;\
    }

    #define get_program(ptiter,program,err)\
    if (program < 0) { printf("Program id [%i] < 0 given!\n", program); return err; }\
    if (program >= enigma::shaderprograms.size()) { printf("Program id [%i] > size() [%i] given!\n", program, enigma::shaderprograms.size()); return err; }\
    if (enigma::shaderprograms[program] == nullptr) { printf("Program with id [%i] is deleted!\n", program); return err; }\
    enigma::ShaderProgram* ptiter = enigma::shaderprograms[program];
#else
    #define get_uniform(uniter,location,usize)\
    if (location < 0) return; \
    std::unordered_map<GLint,enigma::Uniform>::iterator uniter = enigma::shaderprograms[enigma::bound_shader]->uniforms.find(location);\
    if (uniter == enigma::shaderprograms[enigma::bound_shader]->uniforms.end()){\
        return;\
    }

    #define get_attribute(atiter,location)\
    if (location < 0) return; \
    std::unordered_map<GLint,enigma::Attribute>::iterator atiter = enigma::shaderprograms[enigma::bound_shader]->attributes.find(location);\
    if (atiter == enigma::shaderprograms[enigma::bound_shader]->attributes.end()){\
        return;\
    }

    #define get_program(ptiter,program,err)\
    if (program < 0) { return err; }\
    if (program >= enigma::shaderprograms.size()) { return err; }\
    if (enigma::shaderprograms[program] == nullptr) { return err; }\
    enigma::ShaderProgram* ptiter = enigma::shaderprograms[program];
#endif

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

#endif
