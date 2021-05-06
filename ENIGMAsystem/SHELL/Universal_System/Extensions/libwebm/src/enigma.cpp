#include <unordered_map>

#include "player.hpp"
#include "webm_player.hpp"
#include "yuv_rgb.h"

#include "libpng-util/libpng-util.h"
#include "enigma.h"

static int id = -1;
static std::unordered_map<int, uvpx::Player *> videos;
static std::unordered_map<int, wp::WebmPlayer *> audios;

static void convert_rgb_to_rgba(const uint8_t *RGB, uint32_t width, uint32_t height, uint8_t **RGBA, uint32_t dispWidth, uint32_t dispHeight) {
  if ((*RGBA) == nullptr) {
    *RGBA = (uint8_t *)malloc(4 * dispWidth * dispHeight);
  }
  for (uint32_t y = 0; y < height; ++y) {
    if (y > dispHeight) break;
    for (uint32_t x = 0; x < width; ++x) {
      if (x > dispWidth) break;
      (*RGBA)[(y * dispWidth + x) * 4] = RGB[(y * width + x) * 3];
      (*RGBA)[(y * dispWidth + x) * 4 + 1] = RGB[(y * width + x) * 3 + 1];
      (*RGBA)[(y * dispWidth + x) * 4 + 2] = RGB[(y * width + x) * 3 + 2];
      (*RGBA)[(y * dispWidth + x) * 4 + 3] = 255;
    }
  }
}

namespace enigma_user {

bool video_exists(int ind) {
  return (videos.find(ind) != videos.end());
}

int video_add(std::string fname) {
  uvpx::Player *video = new uvpx::Player(uvpx::Player::defaultConfig());
  uvpx::Player::LoadResult res = video->load(fname.c_str(), 1, true);
  wp::WebmPlayer *audio = new wp::WebmPlayer; audio->load(fname.c_str());
  id++; videos.insert(std::make_pair(id, video));
  audios.insert(std::make_pair(id, audio));
  return id;
}

bool video_delete(int ind) {
  if (video_exists(ind)) {
    audios[ind]->destroy();
    delete videos[ind];
    delete audios[ind];
    videos[ind] = nullptr;
    audios[ind] = nullptr;
    videos.erase(ind);
    audios.erase(ind);
    return true;
  }
  return false;
}

bool video_is_playing(int ind) {
  if (video_exists(ind)) {
    return videos[ind]->isPlaying();
  }
  return false;
}

bool video_play(int ind) {
  if (video_exists(ind)) {
    videos[ind]->play();
    audios[ind]->playback(wp::WebmPlayer::PlaybackCommand::Play);
  }
  return video_is_playing(ind);
}

bool video_is_paused(int ind) {
  if (video_exists(ind)) {
    return videos[ind]->isPaused();
  }
  return false;
}

bool video_pause(int ind) {
  if (video_exists(ind)) {
    videos[ind]->pause();
    audios[ind]->playback(wp::WebmPlayer::PlaybackCommand::Pause);
  }
  return video_is_paused(ind);
}

bool video_is_stopped(int ind) {
  if (video_exists(ind)) {
    return videos[ind]->isStopped();
  }
  return false;
}

bool video_stop(int ind) {
  if (video_exists(ind)) {
    videos[ind]->stop();
    audios[ind]->playback(wp::WebmPlayer::PlaybackCommand::Stop);
  }
  return video_is_stopped(ind);
}

double video_get_property(int ind, int property) {
  if (video_exists(ind)) {
    uvpx::Frame *yuv = nullptr;
    yuv = videos[ind]->lockRead();
    if (yuv) {
      double ret;
      switch (property) {
        case video_width:    ret = yuv->displayWidth();          break;
        case video_height:   ret = yuv->displayHeight();         break;
        case video_duration: ret = videos[ind]->info().duration; break;
        default:             ret = -1.0;                         break;
      }
      videos[ind]->unlockRead();
      return ret;
    }
  }
  return -1.0;
}

int video_get_width(int ind) {
  return video_get_property(ind, video_width);
}

int video_get_height(int ind) {
  return video_get_property(ind, video_height);
}

float video_get_duration(int ind) {
  return video_get_property(ind, video_duration);
}

bool video_grab_frame_image(int ind, std::string fname) {
  if (video_exists(ind)) {
    videos[ind]->update(0);
    uvpx::Frame *yuv = nullptr;
    yuv = videos[ind]->lockRead();
    if (yuv) {
      unsigned char *rgb = (unsigned char *)malloc(3 * yuv->width() * yuv->height());
      yuv420_rgb24_std(yuv->width(), yuv->height(), yuv->y(), yuv->u(), yuv->v(), 
      yuv->yPitch(), yuv->uvPitch(), rgb, yuv->width() * 3, YCBCR_JPEG);
      if (rgb) {
        unsigned char *rgba = nullptr;
        convert_rgb_to_rgba(rgb, yuv->width(), yuv->height(), &rgba, yuv->displayWidth(), yuv->displayHeight());
        free(rgb);
        videos[ind]->unlockRead();
        if (rgba) {
          libpng_encode32_file(rgba, yuv->displayWidth(), yuv->displayHeight(), fname.c_str());
          free(rgba);
          return true;
        }
      }
    }
  }
  return false;
}

bool video_grab_frame_buffer(int ind, void *buffer) {
  if (video_exists(ind)) {
    videos[ind]->update(0);
    uvpx::Frame *yuv = nullptr;
    yuv = videos[ind]->lockRead();
    if (yuv) {
      unsigned char *rgb = (unsigned char *)malloc(3 * yuv->width() * yuv->height());
      yuv420_rgb24_std(yuv->width(), yuv->height(), yuv->y(), yuv->u(), yuv->v(), 
      yuv->yPitch(), yuv->uvPitch(), rgb, yuv->width() * 3, YCBCR_JPEG);
      if (rgb) {
        unsigned char *rgba = (unsigned char *)buffer;
        convert_rgb_to_rgba(rgb, yuv->width(), yuv->height(), &rgba, yuv->displayWidth(), yuv->displayHeight());
        free(rgb);
        videos[ind]->unlockRead();
        return true;
      }
    }
  }
  return false;
}

} // namespace enigma_user
