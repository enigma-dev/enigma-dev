#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "game_client/main.h"
#include "steamworks.h"

namespace enigma_user {

void steam_init();
void steam_update();
void steam_shutdown();

}  // namespace enigma_user

#endif  // MANAGEMENT_H
