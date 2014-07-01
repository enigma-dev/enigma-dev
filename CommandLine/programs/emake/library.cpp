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

#if CURRENT_PLATFORM_ID == OS_WINDOWS
#include <windows.h>
#include <process.h>
#else
#include <pthread.h> // use POSIX threads
#include <unistd.h>
#endif

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

bool isoutputting;
FILE* outputfile;
static void* OutputThread() {
  while (isoutputting || !feof(outputfile)) {
    if (!feof(outputfile)) {
      long int pos = ftell(outputfile);
      fpos_t realpos;
      fgetpos(outputfile, &realpos);
   
      // seek to the end
      fseek(outputfile, 0, SEEK_END);

      // get current position
      long int size = ftell(outputfile) - pos;
      
      fsetpos(outputfile, &realpos);
      
      char* buffer = (char*) malloc (sizeof(char)*size);
      fread(buffer, 1, size, outputfile);
      cout << buffer;
      delete buffer;
    } else {
      #if CURRENT_PLATFORM_ID == OS_WINDOWS
      Sleep(1000);
      #else
      usleep(1000);
      #endif
    }
  }

  fclose(outputfile);
  return 0;
}

//Sets the file from which data is redirected to frame log
static void ede_output_redirect_file(const char* filepath)
{
  outputfile = fopen(filepath,"r");

}

//Indicates file completion, dumps remainder to frame log
static void ede_output_redirect_reset()
{
  isoutputting = true;
  #if CURRENT_PLATFORM_ID == OS_WINDOWS // move this shit back to the function above once we've worked out the mutex
  uintptr_t ret = _beginthread((void (*)(void*))OutputThread, 0, 0); 
  #else
  pthread_t me;
  pthread_create(&me, NULL, OutputThread, NULL);
  #endif
  isoutputting = false;
}

//Executes a given command, returns errors or ret val
//static int ede_ide_execute(const char*, const char**, bool) {  }
 //Compresses data. Note image width/height unused
//static Image* ede_ide_compress_data(char *, int) {  }
#if CURRENT_PLATFORM_ID == OS_WINDOWS
GenericFunction LoadPluginFnc(HMODULE library, string funcname)
{
    return (GenericFunction)GetProcAddress(library, funcname.c_str());
}
#else
GenericFunction LoadPluginFnc(void *library, string funcname)
{
    return (GenericFunction)dlsym(library, funcname.c_str());
}
#endif

void* LoadPluginLib()
{
	#if CURRENT_PLATFORM_ID == OS_WINDOWS
    HMODULE handle = LoadLibrary("../../compileEGMf.dll");
  #else
    #if CURRENT_PLATFORM_ID == OS_MAC
    string extension = ".dylib";
    #else
    string extension = ".so";
    #endif
    void *handle = dlopen(("enigma/libcompileEGMf" + extension).c_str(), RTLD_NOW | RTLD_GLOBAL);
	#endif
	
    if (handle == NULL)
    {
      return 0;
    }

    libInit = (const char* (*)(EnigmaCallbacks*)) LoadPluginFnc(handle, "libInit");
    libSetMakeDirectory = (void (*)(const char* dir)) LoadPluginFnc(handle, "libSetMakeDirectory");
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

    return handle;
}

void InitializePluginLib() {
    EnigmaCallbacks ecb;

    ecb.dia_open = ede_dia_open;
    ecb.dia_add = ede_dia_add;
    ecb.dia_clear = ede_dia_clear;
    ecb.dia_progress = ede_dia_progress;
    ecb.dia_progress_text = ede_dia_progress_text;
    ecb.output_redirect_file = ede_output_redirect_file;
    ecb.output_redirect_reset = ede_output_redirect_reset;
    libInit(&ecb);
}
