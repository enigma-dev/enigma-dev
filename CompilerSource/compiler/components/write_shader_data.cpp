/** Copyright (C) 2013 Robert B. Colton, Josh Ventura
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

#include "makedir.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/GameData.h"
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include <math.h> //log2 to calculate passes.

#define flushl '\n' << flush
#define flushs flush

#include "languages/lang_CPP.h"

#define SHADERSTRUCT "struct ShaderStruct { \
    string fragment; \
    string vertex; \
    string type; \
    bool precompile; \
  }; "

static string esc(string str) {
  string res;
  res.reserve(str.length());
  for (size_t i = 0; i < str.length(); ++i) {
    char c = str[i];
    if (c == '\n') { res += "\\n"; continue; }
    if (c == '\r') { res += "\\r"; continue; }
    if (c == '\\') { res += "\\\\"; continue; }
    if (c == '\"') { res += "\\\""; continue; }
    res.append(1, c);
  }
  return res;
}

int lang_CPP::compile_writeShaderData(const GameData &game, parsed_object *EGMglobal)
{
  ofstream wto((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_shaderarrays.h").c_str(),ios_base::out);

  wto << license << "#include \"Universal_System/shaderstruct.h\"\n" << "namespace enigma {\n";
  wto << "  ShaderStruct shaderstructarray[] = {\n";

  int idmax = 0;
  for (const auto &shader : game.shaders) {
    while (idmax < shader.id()) {
      ++idmax, wto << "ShaderStruct(),\n";
    }
    string vertexcode  =  shader.vertex_code();
    string fragmentcode = shader.fragment_code();
    //TODO: Replace quotations with escape sequences.
    wto << "    { "
        << '"' << esc(vertexcode)   << "\", "
        << '"' << esc(fragmentcode) << "\", "
        << '"' << shader.type() << "\", "
        << (shader.precompile()? "true" : "false")
        << " },\n";
    idmax += 1;
  }

  wto << "  };\n";
  wto << "} // Namespace enigma\n";
  wto.close();

  return 0;
}
