#include <stdio.h>
#include <iostream>
#include <string>
#include "../General/ASadvanced.h"
#include "Audio_Systems/audio_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "../General/ASutil.h"
#include "Universal_System/estring.h"
#include "SoundResource.h"
#include "SoundChannel.h"
#include "SDLsystem.h"
#include <SDL_mixer.h>

const int AUDIO_CHANNEL_OFFSET = 400000;
const int AUDIO_CHANNEL_COUNT = 128;
vector<SoundChannel *> sound_channels;

namespace enigma_user {
	
bool audio_exists(int index) { return sounds.exists(index); }

int audio_add(string fname) {
 // Open sound
  size_t flen = 0;
  char* fdata = enigma::read_all_bytes(fname, flen); 
  if (!fdata) {
    DEBUG_MESSAGE(std::string("The Sound file failed - ") + std::string(fname) + std::string(" failed to open"), MESSAGE_TYPE::M_ERROR);
	return -1;
  }
  // Decode sound
  int rid = sounds.size();
  bool fail = enigma::sound_add_from_buffer(rid, fdata, flen);
  delete[] fdata;
  
  if (fail) {
    DEBUG_MESSAGE(std::string("The Sound file failed - ") + std::string(fname) + std::string(" failed to open"), MESSAGE_TYPE::M_ERROR);
	return -1;
  }
  return rid;	
}

int audio_play_sound(int index, double priority, bool loop) {
  const Sound& snd = sounds.get(index);
  if(sound_channels.empty() || sound_channels.size() <= AUDIO_CHANNEL_COUNT - 1) {
    SoundChannel* sc = new SoundChannel();
    sc->mchunk = snd.mc;
    sc->priority = priority;
    sc->soundIndex = index;
    sound_channels.push_back(sc);
    if(Mix_PlayChannel(sound_channels.size() -1,snd.mc, loop)==-1) {
      DEBUG_MESSAGE( std::string("Mix_PlayChannel : ")+std::string(Mix_GetError()), MESSAGE_TYPE::M_ERROR);
    }
    std::cout<<"channel id returned : "<<(sound_channels.size() -1 + AUDIO_CHANNEL_OFFSET)<<"\n";
	return (sound_channels.size() -1 + AUDIO_CHANNEL_OFFSET);
  }
  else {
    for(size_t i =0; i <sound_channels.size();i++) {
      if(Mix_Paused(i) == 1 || sound_channels[i]->priority < priority) {
        if(Mix_PlayChannel(i,snd.mc, loop)==-1) {
          DEBUG_MESSAGE( std::string("Mix_PlayChannel : ")+std::string(Mix_GetError()), MESSAGE_TYPE::M_ERROR);
        }
        sound_channels[i]->mchunk = snd.mc;
        sound_channels[i]->priority = priority;
        sound_channels[i]->soundIndex = index;
        std::cout<<"channel id returned : "<<(i + AUDIO_CHANNEL_OFFSET)<<"\n";
	    return (i + AUDIO_CHANNEL_OFFSET);
      }
    }
  }
  return -1;  
}
  
bool audio_is_playing(int index) {
  if(index >= AUDIO_CHANNEL_OFFSET) {
    const Sound& snd = sounds.get(index - AUDIO_CHANNEL_OFFSET);
    if(sound_channels[index - AUDIO_CHANNEL_OFFSET]->mchunk == snd.mc && Mix_Playing(index - AUDIO_CHANNEL_OFFSET) == 1) { return true; }
  }
  const Sound& snd = sounds.get(index);
  for(size_t i =0; i <sound_channels.size();i++) {
    if(sound_channels[i]->mchunk == snd.mc && Mix_Playing(i) == 1) { return true; }
  }
  return false;  
}

bool audio_is_paused(int index) {
  if(index >= AUDIO_CHANNEL_OFFSET) {
    const Sound& snd = sounds.get(index - AUDIO_CHANNEL_OFFSET);
    if(sound_channels[index - AUDIO_CHANNEL_OFFSET]->mchunk == snd.mc && Mix_Paused(index - AUDIO_CHANNEL_OFFSET) == 1) { return true; }
  }
  const Sound& snd = sounds.get(index);
  for(size_t i =0; i <sound_channels.size();i++) {
    if(sound_channels[i]->mchunk == snd.mc && Mix_Paused(i) == 1) { return true; }
  }
  return false;  
}

void audio_stop_sound(int index) {
  if(index >= AUDIO_CHANNEL_OFFSET) {
    const Sound& snd = sounds.get(index - AUDIO_CHANNEL_OFFSET);
    if(sound_channels[index - AUDIO_CHANNEL_OFFSET]->mchunk == snd.mc) {
      Mix_HaltChannel(index - AUDIO_CHANNEL_OFFSET);
    }
  }
  const Sound& snd = sounds.get(index);
  for(size_t i =0; i <sound_channels.size();i++) {
    if(sound_channels[i]->mchunk == snd.mc) {
      Mix_HaltChannel(i);
    }
  }
}

void audio_pause_sound(int index) {
  if(index >= AUDIO_CHANNEL_OFFSET) {
    const Sound& snd = sounds.get(index - AUDIO_CHANNEL_OFFSET);
    if(sound_channels[index - AUDIO_CHANNEL_OFFSET]->mchunk == snd.mc && Mix_Playing(index - AUDIO_CHANNEL_OFFSET) == 1) {
      Mix_Pause(index - AUDIO_CHANNEL_OFFSET);
    }
  }	
  const Sound& snd = sounds.get(index);
  for(size_t i =0; i <sound_channels.size();i++) {
    if(sound_channels[i]->mchunk == snd.mc && Mix_Playing(i) == 1) {
      Mix_Pause(i);
    }
  }
}

void audio_resume_sound(int index) {
  if(index >= AUDIO_CHANNEL_OFFSET) {
    const Sound& snd = sounds.get(index - AUDIO_CHANNEL_OFFSET);
    if(sound_channels[index - AUDIO_CHANNEL_OFFSET]->mchunk == snd.mc && Mix_Paused(index - AUDIO_CHANNEL_OFFSET) == 1) {
      Mix_Resume(index - AUDIO_CHANNEL_OFFSET);
    }
  }
  const Sound& snd = sounds.get(index);
  for(size_t i =0; i <sound_channels.size();i++) {
    if(sound_channels[i]->mchunk == snd.mc && Mix_Paused(i) == 1) {
      Mix_Resume(i);
    }
  }
}

void audio_stop_all() {
  Mix_HaltChannel(-1);  
}

void audio_pause_all() {
  Mix_Pause(-1);
}

}
