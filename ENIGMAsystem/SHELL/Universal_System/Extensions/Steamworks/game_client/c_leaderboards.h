#ifndef GAMECLIENT_LEADERBOARDS_H
#define GAMECLIENT_LEADERBOARDS_H

#include "game_client.h"

namespace steamworks {

class c_game_client;

class c_leaderboards {
 public:
  c_leaderboards();
  ~c_leaderboards() = default;

  void find_leaderboard(const std::string& leaderboard_name, const ELeaderboardSortMethod leaderboard_sort_method,
                        const ELeaderboardDisplayType leaderboard_display_type);
  bool upload_score(const int score, const ELeaderboardUploadScoreMethod leaderboard_upload_score_method =
                                         k_ELeaderboardUploadScoreMethodNone);

  bool download_scores(const ELeaderboardDataRequest leaderboard_data_request, const int range_start = -1,
                       const int range_end = -1);

 private:
  SteamLeaderboard_t current_leaderboard_;
  bool loading_;
  unsigned number_of_leaderboard_entries_;

  // Called when SteamUserStats()->FindOrCreateLeaderboard() returns asynchronously
  void on_find_leaderboard(LeaderboardFindResult_t* pFindLearderboardResult, bool bIOFailure);
  CCallResult<c_leaderboards, LeaderboardFindResult_t> m_callResultFindLeaderboard;

  // Called when SteamUserStats()->UploadLeaderboardScore() returns asynchronously
  void on_upload_score(LeaderboardScoreUploaded_t* pFindLearderboardResult, bool bIOFailure);
  CCallResult<c_leaderboards, LeaderboardScoreUploaded_t> m_SteamCallResultUploadScore;

  // Called when SteamUserStats()->DownloadLeaderboardEntries() returns asynchronously
  void on_download_scores(LeaderboardScoresDownloaded_t* pFindLearderboardResult, bool bIOFailure);
  CCallResult<c_leaderboards, LeaderboardScoresDownloaded_t> m_SteamCallResultDownloadScores;
};
}  // namespace steamworks

#endif  // GAMECLIENT_LEADERBOARDS_H
