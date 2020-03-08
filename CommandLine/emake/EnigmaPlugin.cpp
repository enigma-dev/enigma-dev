#include "EnigmaPlugin.hpp"
#include "Main.hpp"

#include "OS_Switchboard.h"

#if CURRENT_PLATFORM_ID == OS_WINDOWS
# define byte __windows_byte_workaround
# include <windows.h>
# undef byte
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
#include <map>
#include <vector>
#include <algorithm>

EnigmaPlugin::EnigmaPlugin()
{
}

int EnigmaPlugin::Load()
{
  // Load Plugin
#if CURRENT_PLATFORM_ID == OS_WINDOWS
#	define dlopen(x, y) LoadLibrary(x)
#define dlerror() "Can't be arsed to implement windows errors" //FIXME
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
    errorStream  << "Error Loading Plugin '" << pluginName << "'" << std::endl;
    errorStream << dlerror() << std::endl;
    return PLUGIN_ERROR;
  }

  // Bind Functions
#if CURRENT_PLATFORM_ID == OS_WINDOWS
                        // LOL fucking Winblows warnings
#	define BindFunc(x, y) reinterpret_cast<void(*)()>(GetProcAddress(static_cast<HMODULE>(x), y))
#else
#	define BindFunc(x, y) dlsym(x, y)
#endif

  plugin_Init = reinterpret_cast<const char*(*)(EnigmaCallbacks*)>(BindFunc(_handle, "libInit"));
  plugin_CompileEGM = reinterpret_cast<int (*)(deprecated::JavaStruct::EnigmaStruct *es, const char* exe_filename, int mode)>(BindFunc(_handle, "compileEGMf"));
  plugin_CompileProto = reinterpret_cast<int (*)(const buffers::Project *proj, const char* exe_filename, int mode)>(BindFunc(_handle, "compileProto"));
  plugin_NextResource = reinterpret_cast<const char* (*)()>(BindFunc(_handle, "next_available_resource"));
  plugin_FirstResource = reinterpret_cast<const char* (*)()>(BindFunc(_handle, "first_available_resource"));
  plugin_ResourceIsFunction = reinterpret_cast<bool (*)()>(BindFunc(_handle, "resource_isFunction"));
  plugin_ResourceArgCountMin = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_argCountMin"));
  plugin_ResourceArgCountMax = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_argCountMax"));
  plugin_ResourceOverloadCount = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_overloadCount"));
  plugin_ResourceParameters = reinterpret_cast<const char* (*)(int i)>(BindFunc(_handle, "resource_parameters"));
  plugin_ResourceIsTypeName = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_isTypeName"));
  plugin_ResourceIsGlobal = reinterpret_cast<int (*)()>(BindFunc(_handle, "resource_isGlobal"));
  plugin_ResourcesAtEnd = reinterpret_cast<bool (*)()>(BindFunc(_handle, "resources_atEnd"));
  plugin_Free = reinterpret_cast<void (*)()>(BindFunc(_handle, "libFree"));
  plugin_DefinitionsModified = reinterpret_cast<syntax_error* (*)(const char*, const char*)>(BindFunc(_handle, "definitionsModified"));
  plugin_SyntaxCheck = reinterpret_cast<syntax_error* (*)(int, const char**, const char*)>(BindFunc(_handle, "syntaxCheck"));
  plugin_HandleGameLaunch = reinterpret_cast<void (*)()>(BindFunc(_handle, "ide_handles_game_launch"));
  plugin_LogMakeToConsole = reinterpret_cast<void (*)()>(BindFunc(_handle, "log_make_to_console"));

  return PLUGIN_SUCCESS;
}

const char* EnigmaPlugin::Init(CallBack *ecb)
{
  return plugin_Init(ecb);
}

syntax_error* EnigmaPlugin::SetDefinitions(const char* def, const char* yaml)
{
  return plugin_DefinitionsModified(def, yaml);
}

syntax_error* EnigmaPlugin::SetDefinitions(const char* yaml)
{
  return plugin_DefinitionsModified("", yaml);
}

syntax_error* EnigmaPlugin::SyntaxCheck(int count, const char** names, const char* code)
{
  return plugin_SyntaxCheck(count, names, code);
}

void EnigmaPlugin::HandleGameLaunch()
{
  plugin_HandleGameLaunch();
}

void EnigmaPlugin::LogMakeToConsole()
{
  plugin_LogMakeToConsole();
}

int EnigmaPlugin::BuildGame(deprecated::JavaStruct::EnigmaStruct* data,
                            GameMode mode, const char* fpath) {
  return plugin_CompileEGM(data, fpath, mode);
}

int EnigmaPlugin::BuildGame(buffers::Game* data, GameMode mode, const char* fpath)
{
  buffers::Project proj;
  proj.set_allocated_game(data);
  int ret = plugin_CompileProto(&proj, fpath, mode);
  proj.release_game();
  return ret;
}

const char* EnigmaPlugin::NextResource() {
  return plugin_NextResource();
}

const char* EnigmaPlugin::FirstResource() {
  return plugin_FirstResource();
}

bool EnigmaPlugin::ResourceIsFunction() {
  return plugin_ResourceIsFunction();
}

int EnigmaPlugin::ResourceArgCountMin() {
  return plugin_ResourceArgCountMin();
}

int EnigmaPlugin::ResourceArgCountMax() {
  return plugin_ResourceArgCountMax();
}

int EnigmaPlugin::ResourceOverloadCount() {
  return plugin_ResourceOverloadCount();
}

const char* EnigmaPlugin::ResourceParameters(int i) {
  return plugin_ResourceParameters(i);
}

int EnigmaPlugin::ResourceIsTypeName() {
  return plugin_ResourceIsTypeName();
}

int EnigmaPlugin::ResourceIsGlobal() {
  return plugin_ResourceIsGlobal();
}

bool EnigmaPlugin::ResourcesAtEnd() {
  return plugin_ResourcesAtEnd();
}

void EnigmaPlugin::PrintBuiltins(std::string& fName)
{
  std::vector<std::string> types;
  std::vector<std::string> globals;
  std::map<std::string, std::string> functions;

  const char* currentResource = plugin_FirstResource();
  while (!plugin_ResourcesAtEnd()) {

    if (plugin_ResourceIsFunction()) {
      //for (int i = 0; i < plugin_ResourceOverloadCount(); i++) // FIXME: JDI can't print overloads
        functions[currentResource] = plugin_ResourceParameters(0);
    }

    if (plugin_ResourceIsGlobal())
      globals.push_back(currentResource);

    if (plugin_ResourceIsTypeName())
      types.push_back(currentResource);

    currentResource = plugin_NextResource();
  }

  std::sort(types.begin(), types.end());

  std::ostream out(std::cout.rdbuf());
  std::filebuf fb;

  if (!fName.empty()) {
    std::cout << "Writing builtins..." << std::endl;
    fb.open(fName.c_str(), std::ios::out);
    out.rdbuf(&fb);
  }

  out << "[Types]" << std::endl;
  for (const std::string& t : types)
    out << t << std::endl;

  std::sort(globals.begin(), globals.end());

  out << "[Globals]" << std::endl;
  for (const std::string& g : globals)
    out << g << std::endl;

  out << "[Functions]" << std::endl;
  for (const auto& f : functions)
    out << f.second << std::endl;

  if (!fName.empty()) {
    fb.close();
    std::cout << "Done writing builtins" << std::endl;
  }
}
