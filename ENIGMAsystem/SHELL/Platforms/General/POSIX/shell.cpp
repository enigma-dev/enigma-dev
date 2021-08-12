#include "Platforms/General/PFmain.h"
#include "Widget_Systems/widgets_mandatory.h"

using std::string;

namespace enigma_user {

void url_open(string url) {
  enigma_user::execute_program("xdg-open", url, false);
}

void action_webpage(const string& url) {
  url_open(url);
}

} // namespace enigma_user
