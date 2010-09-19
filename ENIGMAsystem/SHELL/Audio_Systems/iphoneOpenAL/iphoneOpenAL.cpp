/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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
#import "OpenAl/alc.h"
#import "OpenAl/al.h"
#import <AudioToolbox/AudioToolbox.h>
#include "iphoneOpenAL.h"
#include <string>

namespace enigma
{
	
ALCcontext* mContext;
ALCdevice* mDevice;

struct sound
{
    ALuint src, buf; // The source-id and buffer-id of the sound data
    int loaded;   // Degree to which this sound has been loaded successfully
    bool idle;    // True if this sound is not being used, false if playing or paused.
    bool playing; // True if this sound is playing; not paused or idle.
    sound(): src(0), buf(0), loaded(0), idle(1), playing(0) {}
};

sound *sounds;
size_t numSounds;
extern size_t sound_idmax;

int audiosystem_initialize()
{
    numSounds = sound_idmax > 0 ? sound_idmax : 1;
    sounds = new sound[numSounds];
    
	
	// Initialization
	mDevice = alcOpenDevice(NULL); // select the "preferred device"
	if (mDevice) {
		// use the device to make a context
		mContext=alcCreateContext(mDevice,NULL);
		// set my context to the currently active one
		alcMakeContextCurrent(mContext);
	}
	else
	{printf("Failed to open OpenAL device");
		return 1;}
    
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
		
		ALuint& buf = sounds[id].buf;
		//buf = alureCreateBufferFromMemory((ALubyte*)buffer, bufsize);
		alBufferData(buf,AL_FORMAT_STEREO16,(ALubyte*)buffer,bufsize,44100); 
		
		if(!buf) {
			//fprintf(stderr, "Could not load sound %d: %s\n", id, alureGetErrorString());
			printf("Could not load Sound id: %d ",id);
			return 1;
		}
		
		alSourcei(sounds[id].src, AL_BUFFER, buf);
		alSourcei(sounds[id].src, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcei(sounds[id].src, AL_REFERENCE_DISTANCE, 1);
		sounds[id].loaded = 2;
		return 0;
	}
	
	void audiosystem_update(void) { //alureUpdate();
	}
	
	void audiosystem_cleanup()
	{
		for (size_t i = 0; i < numSounds; i++) 
		{
			switch (sounds[i].loaded)
			{
				case 2:
					alDeleteBuffers(1, &sounds[i].buf);
				case 1:
					alDeleteSources(1, &sounds[i].src);
			}
		}
		
		//alureShutdownDevice();
		
		// destroy the context
		alcDestroyContext(mContext);
		// close the device
		alcCloseDevice(mDevice);
	}
};

bool sound_play(int sound) // Returns nonzero if an error occurred
{
	enigma::sound &snd = enigma::sounds[sound]; //snd.looping = false;
	alSourcei(snd.src, AL_LOOPING, AL_FALSE); //Just playing
	alSourcePlay(snd.src);
	return 1;
	//return snd.idle = !(snd.playing = (alurePlaySource(snd.src, enigma::eos_callback, (void*)sound) != AL_FALSE));
}

int sound_add(std::string fname, int kind, bool preload) //At the moment, the latter two arguments do nothing! =D
{
	loadstaticSound(fname.c_str());
	return 0;
}
