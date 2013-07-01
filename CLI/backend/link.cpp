/**
* @file ENIGMAlink.cpp
* @brief Source file of the library link between the IDE and compiler.
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

#include "link.h"
#include "printcolors.h"

int dummy_compileEGMf(EnigmaStruct *es, const char* exe_filename, int mode)
{
    // show error
    print_error("dummy_compileEGMf");
    return -1;
}

const char* dummy_next_available_resource()
{
    // show error
    print_error("dummy_next_available_resource");
    return NULL;
}

const char* dummy_first_available_resource()
{
    // show error
    print_error("dummy_first_available_resource");
    return NULL;
}

bool dummy_resource_isFunction()
{
    // show error
    print_error("dummy_resource_isFunction");
    return false;
}

int dummy_resource_argCountMin()
{
    // show error
    print_error("dummy_resource_argCountMin");
    return 0;
}

int dummy_resource_argCountMax()
{
    // show error
    print_error("dummy_resource_argCountMax");
    return 0;
}

int dummy_resource_overloadCount()
{
    // show error
    print_error("dummy_resource_overloadCount");
    return 0;
}

const char* dummy_resource_parameters(int i)
{
    // show error
    print_error("dummy_resource_parameters");
    return "()";
}

int dummy_resource_isTypeName()
{
    // show error
    print_error("dummy_resource_isTypeName");
    return false;
}

int dummy_resource_isGlobal()
{
    // show error
    print_error("dummy_resource_isGlobal");
    return false;
}

bool dummy_resources_atEnd()
{
    // show error
    print_error("dummy_resources_atEnd");
    return true;
}

const char* dummy_libInit(EnigmaCallbacks* ecs)
{
    // show error
    print_error("dummy_libInit");
    return "";
}

void dummy_libFree()
{
    // show error
    print_error("dummy_libFree");
    return;
}

syntax_error dummy_syerr;
syntax_error *dummy_definitionsModified(const char* wscode, const char* targetYaml)
{
    // show error
    print_error("dummy_definitionsModified");
    return &dummy_syerr;
}

syntax_error *dummy_syntaxCheck(int script_count, const char* *script_names, const char* code)
{
    // show error
    print_error("dummy_syntaxCheck");
    return &dummy_syerr;
}

int (*compileEGMf)(EnigmaStruct *es, const char* exe_filename, int mode) = dummy_compileEGMf;
const char* (*next_available_resource)() = dummy_next_available_resource;
const char* (*first_available_resource)() = dummy_first_available_resource;
bool (*resource_isFunction)() = dummy_resource_isFunction;
int (*resource_argCountMin)() = dummy_resource_argCountMin;
int (*resource_argCountMax)() = dummy_resource_argCountMax;
int (*resource_overloadCount)() = dummy_resource_overloadCount;
const char* (*resource_parameters)(int i) = dummy_resource_parameters;
int (*resource_isTypeName)() = dummy_resource_isTypeName;
int (*resource_isGlobal)() = dummy_resource_isGlobal;
bool (*resources_atEnd)() = dummy_resources_atEnd;
const char* (*libInit)(EnigmaCallbacks* ecs) = dummy_libInit;
void (*libFree)() = dummy_libFree;
syntax_error *(*definitionsModified)(const char* wscode, const char* targetYaml) = dummy_definitionsModified;
syntax_error *(*syntaxCheck)(int script_count, const char* *script_names, const char* code) = dummy_syntaxCheck;
