/**
* @file library.cpp
* @brief Source file of the library interface for linking to the compiler.
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

#include "library.h"

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

//Opens the EnigmaFrame
static void ede_dia_open()
{

}
//Appends a given text to the frame log
static void ede_dia_add(const char* text)
{
    cout << text;
}
//Clears the frame log
static void ede_dia_clear()
{

}
//Sets the progress bar (0-100)
static void ede_dia_progress(int progress)
{

}
//Applies a given text to the progress bar
static void ede_dia_progress_text(const char* caption)
{

}
//Sets the file from which data is redirected to frame log
static void ede_output_redirect_file(const char* filepath)
{

}
//Indicates file completion, dumps remainder to frame log
static void ede_output_redirect_reset()
{

}
//Executes a given command, returns errors or ret val
//static int ede_ide_execute(const char*, const char**, bool) {  }
 //Compresses data. Note image width/height unused
//static Image* ede_ide_compress_data(char *, int) {  }

#if CURRENT_PLATFORM_ID == OS_WINDOWS
void* LoadPluginLib()
{

}
#else

void* LoadPluginLib()
{
    enigmaFrame = frame;
    #if CURRENT_PLATFORM_ID == OS_MAC
    string extension = ".dylib";
    #else
    string extension = ".so";
    #endif

    void *handle = dlopen(("enigma/libcompileEGMf" + extension).c_str(), RTLD_NOW | RTLD_GLOBAL);

    if (handle == NULL)
    {
      return 0;
    }

    libInit = (const char* (*)(EnigmaCallbacks*)) LoadPluginFnc(handle, "libInit");
    compileEGMf = (int (*)(EnigmaStruct *es, const char* exe_filename, int mode)) LoadPluginFnc(handle, "compileEGMf");
    next_available_resource = (const char* (*)()) LoadPluginFnc(handle, "next_available_resource");
    first_available_resource = (const char* (*)()) LoadPluginFnc(handle, "first_available_resource");
    resource_isFunction = (bool (*)()) LoadPluginFnc(handle, "resource_isFunction");
    resource_argCountMin = (int (*)()) LoadPluginFnc(handle, "resource_argCountMin");
    resource_argCountMax = (int (*)()) LoadPluginFnc(handle, "resource_argCountMax");
    resource_overloadCount = (int (*)()) LoadPluginFnc(handle, "resource_overloadCount");
    resource_parameters = (const char* (*)(int i)) LoadPluginFnc(handle, "resource_paramters");
    resource_isTypeName = (int (*)()) LoadPluginFnc(handle, "resource_isTypeName");
    resource_isGlobal = (int (*)()) LoadPluginFnc(handle, "resource_isGlobal");
    resources_atEnd = (bool (*)()) LoadPluginFnc(handle, "resources_atEnd");
    libFree = (void (*)()) LoadPluginFnc(handle, "libFree");
    definitionsModified = (syntax_error* (*)(const char* wscode, const char* targetYaml)) LoadPluginFnc(handle, "definitionsModified");
    syntaxCheck = (syntax_error* (*)(int script_count, const char* *script_names, const char* code)) LoadPluginFnc(handle, "syntaxCheck");


    EnigmaCallbacks ecb;

    ecb.dia_open = ede_dia_open;
    ecb.dia_add = ede_dia_add;
    ecb.dia_clear = ede_dia_clear;
    ecb.dia_progress = ede_dia_progress;
    ecb.dia_progress_text = ede_dia_progress_text;
    ecb.output_redirect_file = ede_output_redirect_file;
    ecb.output_redirect_reset = ede_output_redirect_reset;
    libInit(&ecb);

    return handle;
}

GenericFunction LoadPluginFnc(void *library, string funcname)
{
    return (GenericFunction)dlsym(library, funcname.c_str());
}
#endif
