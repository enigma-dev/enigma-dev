/**
* @file LoadLibrary.h
* @brief Header file of the library interface for linking to the compiler.
*
* Write a description about the file here...
*
* @section License
*
* Copyright (C) 2013 Robert B. Colton
*
* This file is a part of the ENIGMA Development Environment.
*
* ENIGMA is free software: you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation, version 3 of the license or any later version.
*
* This application and its source code is distributed AS-IS, WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along
* with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED

#include <string>
using namespace std;

#include "CompilerSource/OS_Switchboard.h"
#include "link.h"

enum {
  emode_run,
  emode_debug,
  emode_design,
  emode_compile,
  emode_rebuild
};

typedef void(*GenericFunction)();
void* LoadPluginLib();
void* FreePluginLib();
GenericFunction LoadPluginFnc(void *library, string funcname);

#endif // LOADLIBRARY_H_INCLUDED
