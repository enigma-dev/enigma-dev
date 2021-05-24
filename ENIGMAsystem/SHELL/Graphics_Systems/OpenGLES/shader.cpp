

#include "Graphics_Systems/OpenGL-Common/shader.h"
#include "Graphics_Systems/OpenGL-Common/GLSLshader.h"

#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GStextures.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <cstring>      /* memcpy */
#include <math.h>

#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

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
    if (location < 0) { DEBUG_MESSAGE(std::string(str) + " - Uniform location < 0 given (" + std::to_string(location) + ")!", MESSAGE_TYPE::M_ERROR); return; }\
    std::unordered_map<GLint,enigma::Uniform>::iterator uniter = enigma::shaderprograms[enigma::bound_shader]->uniforms.find(location);\
    if (uniter == enigma::shaderprograms[enigma::bound_shader]->uniforms.end()){\
        DEBUG_MESSAGE(std::string(str) + " - Uniform at location " + std::to_string(location) + "  not found!", MESSAGE_TYPE::M_ERROR);\
        return;\
    }else if ( uniter->second.size != usize ){\
        DEBUG_MESSAGE(std::string(str) + " - Uniform [" + uniter->second.name + "] at location " + std::to_string(location) + " with " +  std::to_string(uniter->second.size) + " arguments is accesed by a function with " + std::to_string(usize) + " arguments!", MESSAGE_TYPE::M_ERROR);\
    }

  #define get_attribute(atiter,location)\
    string name = enigma::shaderprograms[enigma::bound_shader]->name;\
    char str[128];\
    if (name == ""){\
        sprintf(str, "Program[%i]", enigma::bound_shader);\
    }else{\
        sprintf(str, "Program[%s = %i]", name.c_str(), enigma::bound_shader);\
    }\
    if (location < 0) { DEBUG_MESSAGE(std::string(str) + " - Attribute location < 0 given (" + std::to_string(location) + ")!", MESSAGE_TYPE::M_ERROR); return; }\
    std::unordered_map<GLint,enigma::Attribute>::iterator atiter = enigma::shaderprograms[enigma::bound_shader]->attributes.find(location);\
    if (atiter == enigma::shaderprograms[enigma::bound_shader]->attributes.end()){\
        DEBUG_MESSAGE(std::string(str) + " - Attribute at location " + std::to_string(location) + " not found!", MESSAGE_TYPE::M_ERROR);\
        return;\
    }

    #define get_program(ptiter,program,err)\
    if (program < 0) { DEBUG_MESSAGE("Program id [" + std::to_string(program) + "] < 0 given!", MESSAGE_TYPE::M_ERROR); return err; }\
    if (size_t(program) >= enigma::shaderprograms.size()) { DEBUG_MESSAGE("Program id [" + std::to_string(program) +"] > size() [" + std::to_string(enigma::shaderprograms.size()) +"] given!", MESSAGE_TYPE::M_ERROR); return err; }\
    if (enigma::shaderprograms[program] == nullptr) { DEBUG_MESSAGE("Program with id [" + std::to_string(program) + "] is deleted!", MESSAGE_TYPE::M_ERROR); return err; }\
    auto& ptiter = enigma::shaderprograms[program];
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
    if (size_t(program) >= enigma::shaderprograms.size()) { return err; }\
    if (enigma::shaderprograms[program] == nullptr) { return err; }\
    auto& ptiter = enigma::shaderprograms[program];
#endif

namespace enigma_user{
	
	void glUniform1ui(int location, unsigned v0)
	{
		glUniform1i(location ,v0);
		
	}
	
	void glUniform2ui(int location, unsigned v0, unsigned v1)
	{
		glUniform2i(location , v0,v1);
		
	}
	
	void glUniform3ui(int location, unsigned v0, unsigned v1, unsigned v2)
	{
		glUniform3i(location,v0,v1,v2);
		
	}
	
	void glUniform4ui(int location, unsigned v0, unsigned v1, unsigned v2, unsigned v3)
	{
		glUniform4i(location, v0, v1, v2, v3);
		
	}
	
	//for unsigned int uniforms 
	
	void glUniform1uiv(int location, int size, const GLint* value)
	{
		glUniform1iv(location,size,value);
		
	}
	
	void glUniform2uiv(int location, int size, const GLint* value)
	{
		glUniform2iv(location,size,value);
		
	}
	
	void glUniform3uiv(int location, int size,const GLint* value)
	{
		glUniform3iv(location,size,value);
		
	}
	
	void glUniform4uiv(int location, int size, const GLint* value)
	{
		glUniform4iv(location,size,value);
		
	}

	
	
	
	
}//namespace enigma_user