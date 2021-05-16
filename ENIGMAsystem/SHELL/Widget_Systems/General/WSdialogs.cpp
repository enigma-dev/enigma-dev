#include "Widget_Systems/General/WSdialogs.h"
#include "Widget_Systems/widgets_mandatory.h"

using std::string;

namespace enigma_user {

void show_debug_message(string errortext, MESSAGE_TYPE type) {
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

}
