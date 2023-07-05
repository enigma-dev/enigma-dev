#include "c_stats_and_achievements.h"

namespace steamworks {

c_stats_and_achievements::c_stats_and_achievements()
    : steam_user_(NULL),
      steam_user_stats_(NULL),
      c_game_id_(SteamUtils()->GetAppID()),
      m_CallbackUserStatsReceived(this, &c_stats_and_achievements::on_user_stats_received),
      m_CallbackUserStatsStored(this, &c_stats_and_achievements::on_user_stats_stored),
      m_CallbackAchievementStored(this, &c_stats_and_achievements::on_achievement_stored) {
  steam_user_ = SteamUser();
  steam_user_stats_ = SteamUserStats();

  stats_valid_ = false;

  request_current_stats();
}

c_stats_and_achievements::~c_stats_and_achievements() {
  if (NULL != steam_user_) delete steam_user_;
  if (NULL != steam_user_stats_) delete steam_user_stats_;
}

void c_stats_and_achievements::on_user_stats_received(UserStatsReceived_t* pCallback) {
  if (c_game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (k_EResultOK == pCallback->m_eResult) {
    DEBUG_MESSAGE("Received stats and achievements from Steam.", M_INFO);

    stats_valid_ = true;

  } else {
    DEBUG_MESSAGE("RequestStats - failed." + std::to_string(pCallback->m_eResult), M_INFO);
  }
}

void c_stats_and_achievements::on_user_stats_stored(UserStatsStored_t* pCallback) {
  if (c_game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (k_EResultOK == pCallback->m_eResult) {
    DEBUG_MESSAGE("StoreStats - success.", M_INFO);
  } else if (k_EResultInvalidParam == pCallback->m_eResult) {
    DEBUG_MESSAGE("StoreStats - some failed to validate.", M_INFO);
    UserStatsReceived_t callback;
    callback.m_eResult = k_EResultOK;
    callback.m_nGameID = c_game_id_.ToUint64();
    on_user_stats_received(&callback);
  } else {
    DEBUG_MESSAGE("StoreStats - failed." + std::to_string(pCallback->m_eResult), M_INFO);
  }
}

void c_stats_and_achievements::on_achievement_stored(UserAchievementStored_t* pCallback) {
  if (c_game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (0 == pCallback->m_nMaxProgress) {
    DEBUG_MESSAGE("Achievement '" + std::string(pCallback->m_rgchAchievementName) + "' unlocked!.", M_INFO);
  } else {
    DEBUG_MESSAGE("Achievement '" + std::string(pCallback->m_rgchAchievementName) + "' progress callback, (" +
                      std::to_string(pCallback->m_nCurProgress) + "," + std::to_string(pCallback->m_nMaxProgress) +
                      ").",
                  M_INFO);
  }
}

void c_stats_and_achievements::request_current_stats() { steam_user_stats_->RequestCurrentStats(); }

bool c_stats_and_achievements::stats_valid() { return stats_valid_; }

void c_stats_and_achievements::set_achievement(const std::string& achievement_name) {
  steam_user_stats_->SetAchievement(achievement_name.c_str());
}

// Review this function
bool c_stats_and_achievements::get_achievement(const std::string& achievement_name) {
  bool achieved = false;
  return steam_user_stats_->GetAchievement(achievement_name.c_str(), &achieved);
}

void c_stats_and_achievements::clear_achievement(const std::string& achievement_name) {
  steam_user_stats_->ClearAchievement(achievement_name.c_str());
}

}  // namespace steamworks
