/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#include "DirectXHeaders.h"
#include "DXshader.h"
#include <math.h>

#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

#include <vector>
using std::vector;

#include <iostream>
#include <fstream>
using namespace std;

enum shadertypes
{
	
};

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

int shader_create(int type)
{

}

unsigned long getFileLength(ifstream& file)
{

}

int shader_load(int id, const char* fname)
{

}

bool shader_compile(int id)
{

}

const char* shader_compile_output(int id)
{

}

void shader_free(int id)
{

}

int shader_program_create()
{

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

}

