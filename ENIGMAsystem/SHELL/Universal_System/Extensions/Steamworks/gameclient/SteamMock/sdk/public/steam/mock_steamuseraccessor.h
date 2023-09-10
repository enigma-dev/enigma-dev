#ifndef MOCK_STEAMUSER_H
#define MOCK_STEAMUSER_H

#include "mock_isteamuser.h"

class SteamUserAccessor : public ISteamUser {
 public:
 static SteamUserAccessor &getInstance() {
    static SteamUserAccessor instance;
    return instance;
  }

  // SteamUserAccessor(SteamUserAccessor const &) = delete;
  // void operator=(SteamUserAccessor const &) = delete;

  bool BLoggedOn() override;

  CSteamID GetSteamID() override;

  private:
  SteamUserAccessor(){}
  ~SteamUserAccessor() = default;

  SteamUserAccessor(SteamUserAccessor const &);
  void operator=(SteamUserAccessor const &);
};

inline ISteamUser *SteamUser() {
  return &SteamUserAccessor::getInstance();
}

#endif  // MOCK_STEAMUSER_H
