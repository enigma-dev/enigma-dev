#ifndef EMAKE_ENIGMAPLUGIN_HPP
#define EMAKE_ENIGMAPLUGIN_HPP

#include "EnigmaCallbacks.hpp"
#include "backend/EnigmaStruct.h"
#include "frontend.h"
#include "project.pb.h"
#include "game.pb.h"

#include <functional>
#include <string>

typedef enum
{
  PLUGIN_ERROR = 1,
  PLUGIN_SUCCESS = 0
} PluginResult;

typedef enum
{
  emode_run,
  emode_debug,
  emode_design,
  emode_compile,
  emode_rebuild,
  emode_invalid
} GameMode;

class EnigmaPlugin
{
public:
  EnigmaPlugin();
  int Load();
  const char* Init(CallBack *cb);
  syntax_error* SetDefinitions(const char* def, const char* yaml);
  syntax_error* SetDefinitions(const char* yaml);
  syntax_error* SyntaxCheck(int count, const char** names, const char* code);
  void HandleGameLaunch();
  void LogMakeToConsole();
  int BuildGame(deprecated::JavaStruct::EnigmaStruct* data, GameMode mode, const char* fpath);
  int BuildGame(buffers::Game* data, GameMode mode, const char* fpath);
  const char* NextResource();
  const char* FirstResource();
  bool ResourceIsFunction();
  int ResourceArgCountMin();
  int ResourceArgCountMax();
  int ResourceOverloadCount();
  const char* ResourceParameters(int i);
  int ResourceIsTypeName();
  int ResourceIsGlobal();
  bool ResourcesAtEnd();
  void PrintBuiltins(std::string& fName);

private:
  std::function<const char*(EnigmaCallbacks*)> plugin_Init = nullptr;
  std::function<int(deprecated::JavaStruct::EnigmaStruct*, const char*, int)> plugin_CompileEGM = nullptr;
  std::function<int(const buffers::Project *proj, const char*, int)> plugin_CompileProto = nullptr;
  std::function<const char*()> plugin_NextResource = nullptr;
  std::function<const char*()> plugin_FirstResource = nullptr;
  std::function<bool()> plugin_ResourceIsFunction = nullptr;
  std::function<int()> plugin_ResourceArgCountMin = nullptr;
  std::function<int()> plugin_ResourceArgCountMax = nullptr;
  std::function<int()> plugin_ResourceOverloadCount = nullptr;
  std::function<const char*(int i)> plugin_ResourceParameters = nullptr;
  std::function<int()> plugin_ResourceIsTypeName = nullptr;
  std::function<int()> plugin_ResourceIsGlobal = nullptr;
  std::function<bool()> plugin_ResourcesAtEnd = nullptr;
  std::function<void()> plugin_Free = nullptr;
  std::function<syntax_error*(const char*, const char*)> plugin_DefinitionsModified = nullptr;
  std::function<syntax_error*(int, const char**, const char*)> plugin_SyntaxCheck = nullptr;
  std::function<void()> plugin_HandleGameLaunch = nullptr;
  std::function<void()> plugin_LogMakeToConsole = nullptr;

  void* _handle;
};

#endif
