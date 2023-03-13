#include "Widget_Systems/widgets_mandatory.h"
#include "GameSettings.h"

#include <android/log.h>
#include <string>

namespace {

android_LogPriority log_priority(MESSAGE_TYPE type) {
  switch(type) {
  case MESSAGE_TYPE::M_INFO: return ANDROID_LOG_INFO;
  case MESSAGE_TYPE::M_WARNING: return ANDROID_LOG_WARN;
  case MESSAGE_TYPE::M_ERROR: case MESSAGE_TYPE::M_USER_ERROR: return ANDROID_LOG_ERROR;
  case MESSAGE_TYPE::M_FATAL_ERROR: case MESSAGE_TYPE::M_FATAL_USER_ERROR: return ANDROID_LOG_ERROR;
  default: return ANDROID_LOG_UNKNOWN;
  //ANDROID_LOG_VERBOSE,
  //ANDROID_LOG_DEBUG,
  //ANDROID_LOG_SILENT
  //ANDROID_LOG_DEFAULT,
  }
}
}


namespace enigma_user {

void show_debug_message(std::string err, MESSAGE_TYPE type) {
  __android_log_print(log_priority(type), enigma::error_type(type).c_str(), "%s\n", err.c_str());
  if (type == MESSAGE_TYPE::M_FATAL_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR) exit(133);
  ABORT_ON_ALL_ERRORS();
}

} //namespace  enigma_user
