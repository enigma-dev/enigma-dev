/** Copyright (C) 2013 Robert B. Colton
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

#include "../General/DirectXHeaders.h"
#include "DX10shader.h"
#include <math.h>

#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

#include <vector>
using std::vector;

#include <iostream>
#include <fstream>
using namespace std;

struct Shader{
  const char* log;

  Shader(int type) 
  {

  }
 
  ~Shader()
  {

  }
};

struct ShaderProgram{

  ShaderProgram()
  {

  }

  ~ShaderProgram()
  {

  }
};

vector<Shader*> shaders(0);
vector<ShaderProgram*> shaderprograms(0);

namespace enigma_user
{

int shader_create(int type)
{
  unsigned int id = shaders.size();
  shaders.push_back(new Shader(type));
  return id;
}

}

unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;
    
    unsigned long pos=file.tellg();
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);
    
    return len;
}

namespace enigma_user
{

int shader_load(int id, const char* fname)
{

}

bool shader_compile(int id)
{

}

const char* shader_compile_output(int id)
{
  return shaders[id]->log;
}

void shader_free(int id)
{
  delete shaders[id];
}

int shader_program_create()
{
  unsigned int id = shaderprograms.size();
  shaderprograms.push_back(new ShaderProgram());
  return id;
}

bool shader_program_link(int id)
{

}

bool shader_program_validate(int id)
{

}

void shader_program_attach(int id, int sid)
{

}

void shader_program_detach(int id, int sid)
{

}

void shader_program_use(int id)
{

}

void shader_program_reset()
{

}

void shader_program_free(int id)
{
  delete shaderprograms[id];
}

}

