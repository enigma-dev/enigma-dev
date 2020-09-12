#include "Widget_Systems/widgets_mandatory.h"

using std::string;

void show_debug_message_helper(string errortext, MESSAGE_TYPE type);

namespace enigma_user {

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  #ifndef DO_NOT_ABORT_ERRORS
  if (severity == MESSAGE_TYPE::M_ERROR || severity == MESSAGE_TYPE::M_USER_ERROR ||
    severity == MESSAGE_TYPE::M_FATAL_ERROR || severity == MESSAGE_TYPE::M_FATAL_USER_ERROR) {
    abort();
  }
  #endif
  if (!debug_output_get_enabled(type)) {
    if (type == MESSAGE_TYPE::M_FATAL_ERROR || 
      type == MESSAGE_TYPE::M_FATAL_USER_ERROR)
      abort();
    return;
  }
  if (type != M_INFO && type != M_WARNING) {
    show_debug_message_helper(errortext, type);
  } else {
    #ifndef DEBUG_MODE
    errortext += "\n";
    fputs(errortext.c_str(), stderr);
    fflush(stderr);
    #endif
    if (type == MESSAGE_TYPE::M_FATAL_ERROR || 
      type == MESSAGE_TYPE::M_FATAL_USER_ERROR)
      abort();
  }
}
  
void debug_output_set_enabled(bool enabled, MESSAGE_TYPE type) {
  enigma::printErrs[type] = enabled;
}

bool debug_output_get_enabled(MESSAGE_TYPE type) {
  return enigma::printErrs[type];
}

}
