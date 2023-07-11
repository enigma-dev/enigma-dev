#include "c_leaderboards.h"

namespace steamworks {

c_leaderboards::c_leaderboards() : loading_(false) {}

void c_leaderboards::create_leaderboard(const std::string& leaderboard_name,
                                        const ELeaderboardSortMethod leaderboard_sort_method,
                                        const ELeaderboardDisplayType leaderboard_display_type) {
  if (loading_) return;

  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindOrCreateLeaderboard(leaderboard_name.c_str(), leaderboard_sort_method,
                                                             leaderboard_display_type);

  if (steam_api_call != 0) {
    c_leaderboards::m_SteamCallResultCreateLeaderboard.Set(steam_api_call, this, &c_leaderboards::on_find_leaderboard);
    loading_ = true;
  }
}

void c_leaderboards::on_find_leaderboard(LeaderboardFindResult_t* pFindLeaderboardResult, bool bIOFailure) {
  loading_ = false;

  if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure) return;

  DEBUG_MESSAGE("Calling FindOrCreateLeaderboard succeeded.", M_INFO);
}

void c_leaderboards::upload_score(const std::string& leaderboard_name, const int score,
                                  const ELeaderboardUploadScoreMethod leaderboard_upload_score_method) {
  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindLeaderboard(leaderboard_name.c_str());

  if (steam_api_call != 0) {
    c_leaderboards::m_SteamCallResultCreateLeaderboard.Set(steam_api_call, this, &c_leaderboards::on_find_leaderboard);
    loading_ = true;
  }

  steam_api_call =
      SteamUserStats()->UploadLeaderboardScore(steam_api_call, leaderboard_upload_score_method, score, NULL, 0);
  c_leaderboards::m_SteamCallResultUploadScore.Set(steam_api_call, this, &c_leaderboards::on_upload_score);
}

void c_leaderboards::on_upload_score(LeaderboardScoreUploaded_t* pScoreUploadedResult, bool bIOFailure) {
  if (!pScoreUploadedResult->m_bSuccess) {
    DEBUG_MESSAGE("Failed to upload score to leaderboard.", M_ERROR);
  }

  if (pScoreUploadedResult->m_bScoreChanged) {
    DEBUG_MESSAGE("Score changed.", M_INFO);
  }
}

void c_leaderboards::download_scores(const std::string& leaderboard_name,
                                     const ELeaderboardDataRequest leaderboard_data_request, const int range_start,
                                     const int range_end) {
  SteamAPICall_t steam_api_call{0};

  steam_api_call = SteamUserStats()->FindLeaderboard(leaderboard_name.c_str());

  if (steam_api_call != 0) {
    c_leaderboards::m_SteamCallResultCreateLeaderboard.Set(steam_api_call, this, &c_leaderboards::on_find_leaderboard);
    loading_ = true;
  }

  steam_api_call =
      SteamUserStats()->DownloadLeaderboardEntries(steam_api_call, leaderboard_data_request, range_start, range_end);
  c_leaderboards::m_SteamCallResultDownloadScores.Set(steam_api_call, this, &c_leaderboards::on_download_scores);
}

void c_leaderboards::on_download_scores(LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure) {
  if (bIOFailure) {
    DEBUG_MESSAGE("Failed to download scores from leaderboard.", M_ERROR);
    return;
  }

  DEBUG_MESSAGE("Downloaded scores from leaderboard.", M_INFO);
}

}  // namespace steamworks
