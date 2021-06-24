#include <string>

namespace enigma_user {

enum {
  video_width,
  video_height,
  video_duration
};

bool video_exists(int ind);

int video_add(std::string fname);

bool video_delete(int ind);

bool video_is_playing(int ind);

bool video_play(int ind);

bool video_is_paused(int ind);

bool video_pause(int ind);

bool video_is_stopped(int ind);

bool video_stop(int ind);

double video_get_property(int ind, int property);

int video_get_width(int ind);

int video_get_height(int ind);

float video_get_duration(int ind);

bool video_grab_frame_image(int ind, std::string fname);

bool video_grab_frame_buffer(int ind, void *buffer);

} // namespace enigma_user
