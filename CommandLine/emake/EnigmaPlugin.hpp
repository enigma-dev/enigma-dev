#ifndef EMAKE_ENIGMAPLUGIN_HPP
#define EMAKE_ENIGMAPLUGIN_HPP

#include "EnigmaCallbacks.hpp"
#include "backend/EnigmaStruct.h"
#include "frontend.h"
#include "project.pb.h"
#include "game.pb.h"

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
  int BuildGame(const buffers::Game& data, GameMode mode, const char* fpath);
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
};

#endif
