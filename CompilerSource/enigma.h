#ifndef ENIGMA_H
#define ENIGMA_H

#include "backend/EnigmaStruct.h"
#include "backend/GameData.h"
#include "frontend.h"

extern "C" {
  
const char* libInit(EnigmaCallbacks* ecs);
int compileEGMf(deprecated::JavaStruct::EnigmaStruct *es, const char* exe_filename, int mode);
int compileProto(const buffers::Project *proj, const char* exe_filename, int mode);
const char* next_available_resource();
const char* first_available_resource();
bool resource_isFunction();
int resource_argCountMin();
int resource_argCountMax();
int resource_overloadCount();
const char* resource_parameters(int i);
int resource_isTypeName();
int resource_isGlobal();
bool resources_atEnd();
void libFree();
syntax_error *definitionsModified(const char* wscode, const char* targetYaml);
syntax_error *syntaxCheck(int script_count, const char* *script_names, const char* code);
void ide_handles_game_launch();
void log_make_to_console();

}

#endif //ENIGMA_H
