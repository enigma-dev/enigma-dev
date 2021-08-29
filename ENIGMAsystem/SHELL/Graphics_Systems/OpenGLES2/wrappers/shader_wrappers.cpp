#include "shader_wrappers.h"

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

void glUniform1uiv(int location, int size, const GLuint* value)
{
  glUniform1iv(location,size, (const GLint*)value);
  
}

void glUniform2uiv(int location, int size, const GLuint* value)
{
  glUniform2iv(location,size,(const GLint*)value);
  
}

void glUniform3uiv(int location, int size,const GLuint* value)
{
  glUniform3iv(location,size,(const GLint*)value);
  
}

void glUniform4uiv(int location, int size, const GLuint* value)
{
  glUniform4iv(location,size,(const GLint*)value);  
}
