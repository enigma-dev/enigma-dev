#include "c_stats_and_achievements.h"

namespace steamworks {

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

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

  c_stats_and_achievements::request_current_stats();
}

bool c_stats_and_achievements::stats_valid() { return c_stats_and_achievements::stats_valid_; }

void c_stats_and_achievements::set_achievement(const std::string& achievement_name) {
  if (!c_stats_and_achievements::steam_user_stats_->SetAchievement(achievement_name.c_str())) {
    DEBUG_MESSAGE("Calling SetAchievement failed for '" + achievement_name +
                      "'. Make sure that RequestCurrentStats has completed and successfully returned its callback and "
                      "the API Name of the specified achievement exists in App Admin on the Steamworks website, and "
                      "the changes are published.",
                  M_ERROR);
    return;
  }

  c_stats_and_achievements::store_stats();
}

bool c_stats_and_achievements::get_achievement(const std::string& achievement_name) {
  bool achieved{false};
  if (!c_stats_and_achievements::steam_user_stats_->GetAchievement(achievement_name.c_str(), &achieved)) {
    DEBUG_MESSAGE("Calling GetAchievement failed for '" + achievement_name +
                      "'. Make sure that RequestCurrentStats has completed and successfully returned its callback and "
                      "the API Name of the specified achievement exists in App Admin on the Steamworks website, and "
                      "the changes are published.",
                  M_ERROR);
    return false;
  }

  return achieved;
}

void c_stats_and_achievements::clear_achievement(const std::string& achievement_name) {
  if (!c_stats_and_achievements::steam_user_stats_->ClearAchievement(achievement_name.c_str())) {
    DEBUG_MESSAGE("Calling ClearAchievement failed for '" + achievement_name +
                      "'. Make sure that RequestCurrentStats has completed and successfully returned its callback and "
                      "the API Name of the specified achievement exists in App Admin on the Steamworks website, and "
                      "the changes are published.",
                  M_ERROR);
    return;
  }

  c_stats_and_achievements::store_stats();
}

void c_stats_and_achievements::set_stat_int(const std::string& stat_name, const int value) {
  if (!c_stats_and_achievements::steam_user_stats_->SetStat(stat_name.c_str(), value)) {
    DEBUG_MESSAGE(
        "Calling SetStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type passed to "
            "this function must match the type listed in the App Admin panel of the Steamworks website.",
        M_ERROR);
    return;
  }

  c_stats_and_achievements::store_stats();
}

int c_stats_and_achievements::get_stat_int(const std::string& stat_name) {
  int value{-1};

  if (!c_stats_and_achievements::steam_user_stats_->GetStat(stat_name.c_str(), &value)) {
    DEBUG_MESSAGE(
        "Calling GetStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type passed to "
            "this function must match the type listed in the App Admin panel of the Steamworks website.",
        M_ERROR);
    return value;
  }

  return value;
}

void c_stats_and_achievements::set_stat_float(const std::string& stat_name, const float value) {
  if (!c_stats_and_achievements::steam_user_stats_->SetStat(stat_name.c_str(), value)) {
    DEBUG_MESSAGE(
        "Calling SetStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type passed to "
            "this function must match the type listed in the App Admin panel of the Steamworks website.",
        M_ERROR);
    return;
  }

  c_stats_and_achievements::store_stats();
}

float c_stats_and_achievements::get_stat_float(const std::string& stat_name) {
  float value{-1.0f};

  if (!c_stats_and_achievements::steam_user_stats_->GetStat(stat_name.c_str(), &value)) {
    DEBUG_MESSAGE(
        "Calling GetStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type passed to "
            "this function must match the type listed in the App Admin panel of the Steamworks website.",
        M_ERROR);
    return value;
  }

  return value;
}

void c_stats_and_achievements::set_stat_average_rate(const std::string& stat_name, const float count_this_session,
                                                     const double session_length) {
  if (!c_stats_and_achievements::steam_user_stats_->UpdateAvgRateStat(stat_name.c_str(), count_this_session,
                                                                      session_length)) {
    DEBUG_MESSAGE(
        "Calling UpdateAvgRateStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type must be "
            "AVGRATE in the Steamworks Partner backend.",
        M_ERROR);
    return;
  }

  c_stats_and_achievements::store_stats();
}

float c_stats_and_achievements::get_stat_average_rate(const std::string& stat_name) {
  return c_stats_and_achievements::get_stat_float(stat_name);
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
    DEBUG_MESSAGE("Calling StoreStats with some stats that are failed to validate. Retrying ...", M_INFO);
    UserStatsReceived_t callback;
    callback.m_eResult = k_EResultOK;
    callback.m_nGameID = c_stats_and_achievements::c_game_id_.ToUint64();
    on_user_stats_received(&callback);
    //c_stats_and_achievements::store_stats();
  } else {
    DEBUG_MESSAGE("Calling StoreStats failed. Retrying ..." + std::to_string(pCallback->m_eResult), M_INFO);
    //c_stats_and_achievements::store_stats();
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

void c_stats_and_achievements::reset_all_stats() {
  if (!c_stats_and_achievements::steam_user_stats_->ResetAllStats(false)) {
    DEBUG_MESSAGE(
        "Calling ResetAllStats failed. Make sure that RequestCurrentStats has completed and successfully returned its "
        "callback.",
        M_ERROR);
    return;
  }

  c_stats_and_achievements::request_current_stats();
}

void c_stats_and_achievements::reset_all_stats_achievements() {
  if (!c_stats_and_achievements::steam_user_stats_->ResetAllStats(true)) {
    DEBUG_MESSAGE(
        "Calling ResetAllStats failed. Make sure that RequestCurrentStats has completed and successfully returned its "
        "callback.",
        M_ERROR);
    return;
  }
  
  c_stats_and_achievements::request_current_stats();
}

////////////////////////////////////////////////////////
// Private fields & functions
////////////////////////////////////////////////////////

void c_stats_and_achievements::request_current_stats() {
  if (!c_stats_and_achievements::steam_user_stats_->RequestCurrentStats()) {
    DEBUG_MESSAGE(
        "Calling RequestCurrentStats failed. Only returns false if there is no user logged in; otherwise, true.",
        M_ERROR);
  }
}

void c_stats_and_achievements::store_stats() {
  if (!c_stats_and_achievements::steam_user_stats_->StoreStats()) {
    DEBUG_MESSAGE(
        "Calling StoreStats failed. Make sure that RequestCurrentStats has completed and successfully returned its "
        "callback and the current game has stats associated with it in the Steamworks Partner backend, and those stats "
        "are published.",
        M_ERROR);
  }
}

}  // namespace steamworks
