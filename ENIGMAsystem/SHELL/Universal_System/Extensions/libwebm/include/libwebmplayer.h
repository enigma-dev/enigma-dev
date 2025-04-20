#include <string>

namespace enigma_user {

const int WEBM_INVALID  = 0;
const int WEBM_WIDTH    = 1;
const int WEBM_HEIGHT   = 2;
const int WEBM_PLAYTIME = 3;
const int WEBM_DURATION = 4;

bool video_exists(int ind);
int video_add(std::string fname);
bool video_delete(int ind);
bool video_is_playing(int ind);
bool video_play(int ind);
bool video_is_paused(int ind);
bool video_pause(int ind);
bool video_is_stopped(int ind);
bool video_stop(int ind);
double video_get_property(int ind, int prop);
int video_get_width(int ind);
int video_get_height(int ind);
double video_get_playtime(int ind);
double video_get_duration(int ind);
bool video_grab_frame_image(int ind, std::string fname);
bool video_grab_frame_buffer(int ind, void *buffer);

} // namespace enigma_user
