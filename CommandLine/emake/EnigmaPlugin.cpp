#include "EnigmaPlugin.hpp"

#include "OS_Switchboard.h"

#if CURRENT_PLATFORM_ID == OS_WINDOWS
#	include <windows.h>
#	include <process.h>
#else
#	include <pthread.h>
#	include <unistd.h>
#	include <dlfcn.h>
#endif

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

EnigmaPlugin::EnigmaPlugin()
{
}

int EnigmaPlugin::Init()
{
  // Load Plugin
#if CURRENT_PLATFORM_ID == OS_WINDOWS
#	define dlopen(x, y) LoadLibrary(x)
  std::string extension = ".dll";
  std::string prefix = "";
#elif CURRENT_PLATFORM_ID ==  OS_MACOSX
  std::string extension = ".dylib";
  std::string prefix = "lib";
#else
  std::string extension = ".so";
  std::string prefix = "lib";
#endif

  std::string pluginName = "./" + prefix + "compileEGMf" + extension;
  
  _handle = dlopen(pluginName.c_str(), RTLD_LAZY);

  if (!_handle)
  {
    std::cerr << "Error Loading Plugin " << pluginName << std::endl;
    return PLUGIN_ERROR;
  }

  // Bind Functions
#if CURRENT_PLATFORM_ID == OS_WINDOWS
#	define BindFunc(x, y) GetProcAddress(static_cast<HMODULE>(x), y)
#else
#	define BindFunc(x, y) dlsym(x, y)
#endif

  plugin_Init = reinterpret_cast<const char*(*)(EnigmaCallbacks*)>(BindFunc(_handle, "libInit"));
  plugin_SetMakeDirectory = reinterpret_cast<void (*)(const char* dir)>(BindFunc(_handle, "libSetMakeDirectory"));
  plugin_CompileEGM = reinterpret_cast<int (*)(EnigmaStruct *es, const char* exe_filename, int mode)>(BindFunc(_handle, "compileEGMf"));
  plugin_NextResource = reinterpret_cast<const char* (*)()>(BindFunc(_handle, "next_available_resource"));
  plugin_FirstResource = reinterpret_cast<const char* (*)()>(BindFunc(_handle, "first_available_resource"));
  plugin_ResourceIsFunction = reinterpret_cast<bool (*)()>(BindFunc(_handle, "resource_isFunction"));
  plugin_ResourceArgCountMin = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_argCountMin"));
  plugin_ResourceArgCountMax = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_argCountMax"));
  plugin_ResourceOverloadCount = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_overloadCount"));
  plugin_ResourceParameters = reinterpret_cast<const char* (*)(int i)>(BindFunc(_handle, "resource_paramters"));
  plugin_ResourceIsTypeName = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_isTypeName"));
  plugin_ResourceIsGlobal = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_isGlobal"));
  plugin_ResourcesAtEnd = reinterpret_cast<bool (*)()>(BindFunc(_handle, "resources_atEnd"));
  plugin_Free = reinterpret_cast<void (*)()>(BindFunc(_handle, "libFree"));
  plugin_DefinitionsModified = reinterpret_cast<syntax_error* (*)(const char*, const char*)>(BindFunc(_handle, "definitionsModified"));
  plugin_SyntaxCheck = reinterpret_cast<syntax_error* (*)(int, const char**, const char*)>(BindFunc(_handle, "syntaxCheck"));


  CallBack ecb;
  CallBack::SetOutFile("emake_out.log");
  plugin_Init(&ecb);

  // Who Added this garbage and why?
  //plugin_SetMakeDirectory("");

  return PLUGIN_SUCCESS;
}

void EnigmaPlugin::SetDefinitions(const char* def)
{
  plugin_DefinitionsModified("", def);
}

int EnigmaPlugin::BuildGame(EnigmaStruct* data, GameMode mode, const char* fpath)
{
  const char* currentResource = plugin_FirstResource();
  while (!plugin_ResourcesAtEnd())
  {
    currentResource = plugin_NextResource();
  }

  return plugin_CompileEGM(data, fpath, mode);
}


