/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2011 Josh Ventura                                        **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <string>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
using std::string;
#include "as_basic.h"
#include "../audio_mandatory.h"
#include "alure/include/AL/alure.h"

namespace enigma
{
  struct sound
  {
    ALuint src, buf[3]; // The source-id and buffer-id of the sound data
	alureStream *stream; // optional stream
	void (*cleanup)(void *userdata); // optional cleanup callback for streams
	void *userdata; // optional userdata for streams

    int loaded;   // Degree to which this sound has been loaded successfully
    bool idle;    // True if this sound is not being used, false if playing or paused.
    bool playing; // True if this sound is playing; not paused or idle.

    sound(): src(0), stream(0), cleanup(0), userdata(0), loaded(0), idle(1), playing(0) {
	  buf[0] = 0; buf[1] = 0; buf[2] = 0;
	}
  };
  
  sound *sounds;
  size_t numSounds;
  extern size_t sound_idmax;
  
  static void eos_callback(void *soundID, ALuint src)
  {
    sound &snd = sounds[(ptrdiff_t)soundID];
    /*if (snd.looping)
      snd.idle = !(snd.looping = snd.playing = (alurePlaySource(snd.src, eos_callback, soundID) != AL_FALSE));
    else {*/
      snd.playing = false;
      snd.idle = true;
    //}
  }

  int audiosystem_initialize()
  {
    numSounds = sound_idmax > 0 ? sound_idmax : 1;
    sounds = new sound[numSounds];
    
    #ifdef _WIN32
    load_al_dll();
    #endif
    
    if(!alureInitDevice(NULL, NULL)) {
      fprintf(stderr, "Failed to open OpenAL device: %s\n", alureGetErrorString());
      return 1;
    }
    
    for (size_t i = 0; i < numSounds; i++)
    {
      alGenSources(1, &sounds[i].src);
      if(alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Failed to create OpenAL source!\n");
        return 1;
      }
      sounds[i].loaded = 1;
    }
    
    return 0;
  }
  
  int sound_add_from_buffer(int id, void* buffer, size_t bufsize)
  {
    if (sounds[id].loaded != 1)
      return 1;
    
    ALuint& buf = sounds[id].buf[0];
    buf = alureCreateBufferFromMemory((ALubyte*)buffer, bufsize);
    
    if(!buf) {
      fprintf(stderr, "Could not load sound %d: %s\n", id, alureGetErrorString());
      return 1;
    }
    
    alSourcei(sounds[id].src, AL_BUFFER, buf);
    alSourcei(sounds[id].src, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(sounds[id].src, AL_REFERENCE_DISTANCE, 1);
    sounds[id].loaded = 2;
    return 0;
  }

  int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size), void (*cleanup)(void *userdata), void *userdata) {
    if (sounds[id].loaded != 1)
      return 1;

    sounds[id].stream = alureCreateStreamFromCallback((ALuint (*)(void*, ALubyte*, ALuint))callback, userdata, AL_FORMAT_STEREO16, 44100, 4096, 3, sounds[id].buf);
    if (!sounds[id].stream) {
      fprintf(stderr, "Could not create stream %d: %s\n", id, alureGetErrorString());
      return 1;
    }
	sounds[id].cleanup = cleanup;
	sounds[id].userdata = userdata;

    sounds[id].loaded = 2;
    return 0;
  }

  int sound_allocate() {
    // Make room for sound
    const size_t osc = enigma::numSounds;
    if (osc <= enigma::sound_idmax)
    {
  	enigma::sound *nsounds = new enigma::sound[enigma::numSounds = enigma::sound_idmax+1];
  	for (size_t i = 0; i < osc; i++)
  	  nsounds[i] = enigma::sounds[i];
  	for (size_t i = osc; i < enigma::numSounds; i++)
  	{
  	  alGenSources(1, &nsounds[i].src);
  	  if(alGetError() != AL_NO_ERROR) {
  		fprintf(stderr, "Failed to create OpenAL source!\n");
  		return 1;
  	  }
  	  nsounds[i].loaded = 1;
  	}
  	delete[] enigma::sounds;
  	enigma::sounds = nsounds;
    }

	return enigma::sound_idmax++;
  }
  
  void audiosystem_update(void) { alureUpdate(); }
  
  void audiosystem_cleanup()
  {
    for (size_t i = 0; i < numSounds; i++) 
    {
      switch (sounds[i].loaded)
      {
        case 2:
          alDeleteBuffers(sounds[i].stream ? 3 : 1, sounds[i].buf);
		  if (sounds[i].stream) {
			alureStopSource(sounds[i].src, true);
			alureDestroyStream(sounds[i].stream, 0, 0);
			if (sounds[i].cleanup) sounds[i].cleanup(sounds[i].userdata);
		  }
		  // fallthrough
        case 1:
          alDeleteSources(1, &sounds[i].src);
      }
    }
    
    alureShutdownDevice();
  }
};

bool sound_play(int sound) // Returns nonzero if an error occurred
{
  enigma::sound &snd = enigma::sounds[sound]; //snd.looping = false;
  alSourcei(snd.src, AL_LOOPING, AL_FALSE); //Just playing
  return snd.idle = !(snd.playing = !snd.stream ?
	alurePlaySource(snd.src, enigma::eos_callback, (void*)sound) != AL_FALSE :
	alurePlaySourceStream(snd.src, snd.stream, 3, -1, enigma::eos_callback, (void*)sound) != AL_FALSE);
}
bool sound_loop(int sound)
{
  enigma::sound &snd = enigma::sounds[sound];
  alSourcei(snd.src, AL_LOOPING, AL_TRUE); //Looping now
  return snd.idle = !(/*snd.looping =*/ snd.playing = (alurePlaySource(snd.src, enigma::eos_callback, (void*)sound) != AL_FALSE));
}
bool sound_pause(int sound)
{
  enigma::sound &snd = enigma::sounds[sound];
  return snd.playing = !alurePauseSource(snd.src);
}
void sound_stop(int sound) {
    enigma::sound &snd = enigma::sounds[sound];
    alureStopSource(snd.src,AL_FALSE);
}
bool sound_resume(int sound)
{
  enigma::sound &snd = enigma::sounds[sound];
  return snd.playing = alureResumeSource(snd.src);
}

bool sound_isplaying(int sound) {
  return enigma::sounds[sound].playing;
}
bool sound_ispaused(int sound) {
  return !enigma::sounds[sound].idle and !enigma::sounds[sound].playing;
}

void sound_pan(int sound, float value)
{
  enigma::sound &snd = enigma::sounds[sound];
  const float pan = value*2-1;
  alSource3f(snd.src,AL_POSITION,pan,sqrt(1-pan*pan),0);
}


const char* sound_get_audio_error() {
  return alureGetErrorString();
}

#include <string>
using namespace std;
extern void show_message(string);
int sound_add(string fname, int kind, bool preload) //At the moment, the latter two arguments do nothing! =D
{
  // Open sound
  FILE *afile = fopen(fname.c_str(),"rb");
  if (!afile)
    return -1;
  
  // Buffer sound
  fseek(afile,0,SEEK_END);
  const size_t flen = ftell(afile);
  char fdata[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading sound data");
  
  // Decode sound
  int rid = enigma::sound_allocate();
  if (enigma::sound_add_from_buffer(rid,fdata,flen))
    return (--enigma::sound_idmax, -1);
  
  return rid;
}
