#include "general_user.h"
#include "wrapper/general_api/general.h"

namespace enigma_user {

    bool steam_initialised() {
        return general::steam_initialised();
    }
    
}
