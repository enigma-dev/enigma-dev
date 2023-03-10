#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/SDL/Window.h"

using std::string;

namespace enigma_user {

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  if (type != M_INFO && type != M_WARNING) {
    SDL_ShowSimpleMessageBox(0,
    enigma_user::window_get_caption().c_str(), errortext.c_str(), enigma::windowHandle);
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
