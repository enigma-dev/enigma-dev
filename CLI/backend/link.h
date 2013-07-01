/**
* @file ENIGMAlink.h
* @brief Header file of the plugin link to the compiler.
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

#ifndef LINK_H_INCLUDED
#define LINK_H_INCLUDED

#include <string>
#include <stdio.h>
using namespace std;

#include "backend/EnigmaStruct.h"
#include "backend/JavaCallbacks.h"
#include "syntax_error.h"
#include <dlfcn.h>

extern int (*compileEGMf)(EnigmaStruct *es, const char* exe_filename, int mode);
extern const char* (*next_available_resource)();
extern const char* (*first_available_resource)();
extern bool (*resource_isFunction)();
extern int (*resource_argCountMin)();
extern int (*resource_argCountMax)(); //Returns the maximum number of arguments to the function
extern int (*resource_overloadCount)(); //Returns the number of times the function was declared in the parsed sources
extern const char* (*resource_parameters)(int i); //Returns a simple string of parameters and defaults that would serve as the prototype of this function
extern int (*resource_isTypeName)(); //Returns whether the resource can be used as a typename.
extern int (*resource_isGlobal)(); //Returns whether the resource is nothing but a global variable.
extern bool (*resources_atEnd)(); //Returns whether we're really done iterating the list
extern const char* (*libInit)(EnigmaCallbacks* ecs);
extern void (*libFree)();
extern syntax_error* (*definitionsModified)(const char* wscode, const char* targetYaml);
extern syntax_error* (*syntaxCheck)(int script_count, const char* *script_names, const char* code);

#endif // ENIGMALINK_H_INCLUDED
