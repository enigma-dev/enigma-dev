#include "general.h"

bool general::steam_initialised() {
    return SteamAPI_Init();
}
