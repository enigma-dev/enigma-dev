#include <cstdlib>

#include "include.h"

namespace enigma
{
  int audiosystem_initialize() { return 0; }
  void audiosystem_update() {}
  int sound_add_from_buffer(int id, void* buffer, size_t size) { return -1; }
  void audiosystem_cleanup() {}
}

namespace enigma_user
{
  int sound_add(string fname, int kind, bool preload) {
    return -1;
  }

  void sound_pitch(int sound, float value) {}

  bool sound_exists(int) { return false; }
  bool sound_play(int) { return false; }
  bool sound_loop(int) { return false; }
  void sound_stop(int) {}
  void sound_stop_all() {}
  void sound_delete(int) {}
  bool sound_pause(int) { return false; }
  void sound_pause_all() {}
  bool sound_resume(int) { return false; }
  void sound_resume_all() {}
  float sound_get_pan(int) { return 0; }
  float sound_get_volume(int) { return 0; }
  float sound_get_length(int) { return 0; }
  float sound_get_position(int) { return 0; }
  void sound_seek(int, float) {}
  void sound_seek_all(float) {}
  bool sound_isplaying(int) { return false; }
  bool sound_ispaused(int) { return false; }
  bool sound_replace(int, string, int, bool) { return false; }
  const char* sound_get_audio_error() { return ""; }
  void sound_pan(int, float) {}
  void sound_volume(int, float) {}
  void sound_global_volume(float) {}
  void sound_3d_set_sound_cone(int, float, float, float, double, double, long) {}
  void sound_3d_set_sound_distance(int, float, float) {}
  void sound_3d_set_sound_position(int, float, float, float) {}
  void sound_3d_set_sound_velocity(int, float, float, float) {}
  void sound_effect_chorus(int, float, float, float, float, long, float, long) {}
  void sound_effect_echo(int, float, float, float, float, long) {}
  void sound_effect_flanger(int, float, float, float, float, long, float, long) {}
  void sound_effect_gargle(int, unsigned, unsigned) {}
  void sound_effect_reverb(int, float, float, float, float) {}
  void sound_effect_compressor(int, float, float, float, float, float, float) {}
  void sound_effect_equalizer(int, float, float, float) {}
  void sound_effect_set(int, int) {}

  bool audio_exists(int) { return false; }
  bool audio_is_playing(int) { return false; }
  bool audio_is_paused(int) { return false; }
  int audio_play_sound(int, double, bool) { return -1; }
  int audio_play_sound_at(int, as_scalar, as_scalar, as_scalar, as_scalar, as_scalar, as_scalar, bool, double) { return -1; }
  int audio_play_sound_on(int, int, bool, double) { return -1; }
  void audio_pause_sound(int) {}
  void audio_resume_sound(int) {}
  void audio_stop_sound(int) {}
  void audio_pause_all() {}
  void audio_resume_all() {}
  void audio_stop_all() {}
  void audio_sound_seek(int, double) {}
  double audio_sound_offset(int) { return 0; }
  void audio_listener_orientation(as_scalar, as_scalar, as_scalar, as_scalar, as_scalar, as_scalar) {}
  void audio_listener_position(as_scalar, as_scalar, as_scalar) {}
  void audio_listener_velocity(as_scalar, as_scalar, as_scalar) {}
  int audio_sound_length(int) { return 0; }
  void audio_sound_pitch(int, float) {}
  void audio_sound_gain(int, float, double) {}
  void audio_master_gain(float) {}
  void audio_channel_num(int) {}
  int audio_system() { return 0; }
  int audio_add(string) { return -1; }
  void audio_delete(int) {}
  void audio_falloff_set_model(int) {}
  int audio_emitter_create() { return -1; }
  bool audio_emitter_exists(int) { return false; }
  void audio_emitter_falloff(int, as_scalar, as_scalar, as_scalar) {}
  void audio_emitter_free(int) {}
  void audio_emitter_gain(int, double) {}
  void audio_emitter_pitch(int, double) {}
  void audio_emitter_position(int, as_scalar, as_scalar, as_scalar) {}
  void audio_emitter_velocity(int, as_scalar, as_scalar, as_scalar) {}
}
