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

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include "backend/ideprint.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

inline void writes(const char* x, FILE *f) {
  fwrite(&x,4,1,f);
}

#include "languages/lang_CPP.h"
int lang_CPP::module_write_shaders(EnigmaStruct *es, FILE *gameModule)
{
  // Now we're going to add shaders
  edbg << es->shaderCount << " Adding Shaders to Game Module: " << flushl;
  
  //Magic Number
  fwrite("SHR ",4,1,gameModule);
  
  //Indicate how many
  int shader_count = es->shaderCount;
  fwrite(&shader_count,4,1,gameModule);
  
  int shader_maxid = 0;
  for (int i = 0; i < shader_count; i++)
    if (es->shaders[i].id > shader_maxid)
      shader_maxid = es->shaders[i].id;
  fwrite(&shader_maxid,4,1,gameModule);
  
  for (int i = 0; i < shader_count; i++)
  {
    writei(es->shaders[i].id,gameModule); //id
    
    writes(es->shaders[i].vcode,gameModule); //Vertex code of the shader
    writes(es->shaders[i].fcode,gameModule); //Fragment code of the shader
    writes(es->shaders[i].type,gameModule);    //Language of the shader
    writei(es->shaders[i].precompile,gameModule); //Compile the shader after loading

  }
 
  edbg << "Done writing shaders." << flushl;
  return 0;
}
