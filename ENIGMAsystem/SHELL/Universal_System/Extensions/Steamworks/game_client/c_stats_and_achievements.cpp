#include "c_stats_and_achievements.h"

namespace steamworks {

c_stats_and_achievements::c_stats_and_achievements()
    : stats_valid_(false),
      steam_user_(NULL),
      steam_user_stats_(NULL),
      c_game_id_(SteamUtils()->GetAppID()),
      m_CallbackUserStatsReceived(this, &c_stats_and_achievements::on_user_stats_received),
      m_CallbackUserStatsStored(this, &c_stats_and_achievements::on_user_stats_stored),
      m_CallbackAchievementStored(this, &c_stats_and_achievements::on_achievement_stored) {
  c_stats_and_achievements::steam_user_ = SteamUser();
  c_stats_and_achievements::steam_user_stats_ = SteamUserStats();

  request_current_stats();
}

void c_stats_and_achievements::on_user_stats_received(UserStatsReceived_t* pCallback) {
  if (c_stats_and_achievements::c_game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (c_stats_and_achievements::stats_valid_) return;

  if (k_EResultOK == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling RequestCurrentStats succeeded.", M_INFO);

    c_stats_and_achievements::stats_valid_ = true;

  } else {
    DEBUG_MESSAGE(
        "Calling RequestCurrentStats failed. Only returns false if there is no user logged in; otherwise, true." +
            std::to_string(pCallback->m_eResult),
        M_INFO);
  }
}

void c_stats_and_achievements::on_user_stats_stored(UserStatsStored_t* pCallback) {
  if (c_stats_and_achievements::c_game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (k_EResultOK == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling StoreStats succeeded.", M_INFO);
  } else if (k_EResultInvalidParam == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling StoreStats nearly failed to validate. Retrying ...", M_INFO);
    UserStatsReceived_t callback;
    callback.m_eResult = k_EResultOK;
    callback.m_nGameID = c_stats_and_achievements::c_game_id_.ToUint64();
    on_user_stats_received(&callback);
    store_stats();
  } else {
    DEBUG_MESSAGE("Calling StoreStats failed. Retrying ..." + std::to_string(pCallback->m_eResult), M_INFO);
    store_stats();
  }
}

void c_stats_and_achievements::on_achievement_stored(UserAchievementStored_t* pCallback) {
  if (c_stats_and_achievements::c_game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (0 == pCallback->m_nMaxProgress) {
    DEBUG_MESSAGE("Achievement '" + std::string(pCallback->m_rgchAchievementName) + "' unlocked!.", M_INFO);
  } else {
    DEBUG_MESSAGE("Achievement '" + std::string(pCallback->m_rgchAchievementName) + "' progress callback, (" +
                      std::to_string(pCallback->m_nCurProgress) + "," + std::to_string(pCallback->m_nMaxProgress) +
                      ").",
                  M_INFO);
  }
}

void c_stats_and_achievements::request_current_stats() {
  if (!c_stats_and_achievements::steam_user_stats_->RequestCurrentStats()) {
    DEBUG_MESSAGE(
        "Calling RequestCurrentStats failed. Only returns false if there is no user logged in; otherwise, true.",
        M_ERROR);
  }
}

bool c_stats_and_achievements::stats_valid() { return c_stats_and_achievements::stats_valid_; }

void c_stats_and_achievements::set_achievement(const std::string& achievement_name) {
  c_stats_and_achievements::steam_user_stats_->SetAchievement(achievement_name.c_str());
  store_stats();
}

bool c_stats_and_achievements::get_achievement(const std::string& achievement_name) {
  bool achieved{false};
  if (!c_stats_and_achievements::steam_user_stats_->GetAchievement(achievement_name.c_str(), &achieved)) {
    DEBUG_MESSAGE("Calling GetAchievement failed for '" + achievement_name +
                      "'. RequestCurrentStats has completed and successfully returned its callback.RequestCurrentStats "
                      "has completed and successfully returned its callback. and the 'API Name' of the specified "
                      "achievement exists in App Admin on the Steamworks website, and the changes are published.",
                  M_ERROR);
    return false;
  }
  return achieved;
}

void c_stats_and_achievements::clear_achievement(const std::string& achievement_name) {
  c_stats_and_achievements::steam_user_stats_->ClearAchievement(achievement_name.c_str());
}

void c_stats_and_achievements::reset_all_stats() { c_stats_and_achievements::steam_user_stats_->ResetAllStats(false); }

void c_stats_and_achievements::reset_all_stats_achievements() {
  c_stats_and_achievements::steam_user_stats_->ResetAllStats(true);
}

void c_stats_and_achievements::store_stats() { c_stats_and_achievements::steam_user_stats_->StoreStats(); }

}  // namespace steamworks
