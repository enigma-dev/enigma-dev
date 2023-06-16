#if !defined(WRAPPER_STEAM_UTILS_H)
#define WRAPPER_STEAM_UTILS_H

#include "Widget_Systems/widgets_mandatory.h"
#include "wrapper/classes/steam_main.h"

#include <stdexcept>
// #include <exception>
#include <string>

namespace steamworks {

class steam_utils {
 private:

 public:
    static uint32 get_app_id();

};

}  // namespace steamworks

#endif  // !WRAPPER_STEAM_UTILS_H
