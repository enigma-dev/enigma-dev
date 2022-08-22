#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/Instances/instance_system.h"
#include "GameSettings.h"

#include <string>

namespace enigma_user {

void show_debug_message(std::string err, MESSAGE_TYPE type) {
if (type == MESSAGE_TYPE::M_USER_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR) {
    printf((enigma::error_type(type) + ": in some action of some event for object %d, instance id %d: %s\n").c_str(),
           (enigma::instance_event_iterator == NULL? enigma_user::global :
              enigma::instance_event_iterator->inst == NULL? enigma_user::noone :
                enigma::instance_event_iterator->inst->object_index),
           (enigma::instance_event_iterator == NULL? enigma_user::global :
              enigma::instance_event_iterator->inst == NULL? enigma_user::noone :
                enigma::instance_event_iterator->inst->id),
           err.c_str()
      );
  } 
  
  else printf((enigma::error_type(type) + ": %s\n").c_str(), err.c_str());
  fflush(stdout);
  
  if (type == MESSAGE_TYPE::M_FATAL_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR) exit(133);
  ABORT_ON_ALL_ERRORS();
}

} //namespace enigma_user
