#include "Widget_Systems/widgets_mandatory.h"

using std::string;

void show_debug_message_helper(string errortext, MESSAGE_TYPE type);

namespace enigma_user {

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  if (!debug_output_get_enabled(type)) return;
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
  
void debug_output_set_enabled(bool enabled, message_type) {
  enigma::printErrs[message_type] = enabled;
}

bool debug_output_get_enabled(message_type) {
  return enigma::printErrs[message_type];
}

}
