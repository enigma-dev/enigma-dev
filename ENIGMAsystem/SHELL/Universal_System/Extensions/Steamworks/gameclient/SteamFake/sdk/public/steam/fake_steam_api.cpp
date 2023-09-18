#include "steam/steam_api.h"

bool SteamAPI_Init() { return true; }

void SteamAPI_Shutdown() {}

/**
 * @brief 
 * 
 * @param unOwnAppID 
 * @return false when success
 * @return true when failure
 */
bool SteamAPI_RestartAppIfNecessary( uint32 unOwnAppID ) { return false; }
