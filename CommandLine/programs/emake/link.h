/**
* @file link.h
* @brief Header file of the plugin link to the compiler.
*
* @section License
*
* Copyright (C) 2013 Robert B. Colton
* This file is part of ENIGMA.
*
* ENIGMA is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation, version 3 of the License, or (at your option) any later version.
*
* ENIGMA is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
* PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* ENIGMA. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef ENIGMALINK_H_INCLUDED
#define ENIGMALINK_H_INCLUDED

#include <string>
#include <stdio.h>
using namespace std;

#include "backend/EnigmaStruct.h"
#include "backend/JavaCallbacks.h"
#include "syntax_error.h"
#include "OS_Switchboard.h"

#if CURRENT_PLATFORM_ID == OS_LINUX
#include <dlfcn.h>
#else

#endif

extern void (*libSetMakeDirectory)(const char* dir);
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
