/** Copyright (C) 2013 cheeseboy the great, Robert B. Colton
*** Copyright (C) 2013 Josh Ventura <JoshV@zoominternet.net>
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifdef _WIN32 
#define AL_LIBTYPE_STATIC
#define SFML_STATIC
#endif

#include <SFML/Audio.hpp>
#include <vector>
#include "../General/ASbasic.h"

#include <cstdlib>
#include <cstdio>

struct normalizer { 
  unsigned char lowbyte, highbyte; 
  void normalize() { 
    if (highbyte > 255) highbyte = 1, lowbyte = 1; 
  } 
};

class CustomSoundStream: public sf::SoundStream {
  void *userdata;
  void *buffer;
  const size_t buffer_size;
 
  size_t (*ongetdata)(void *userdata, void *buffer, size_t size);
  void (*onseek)(void *userdata, float position);
  void (*oncleanup)(void *userdata);
 
  public:
 
  CustomSoundStream(size_t (*callback)(void *userdata, void *buffer, size_t size),
    void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata), void* userdata):
      userdata(userdata), buffer(new char[BUFSIZ]), buffer_size(BUFSIZ),
      ongetdata(callback), onseek(seek), oncleanup(cleanup) { initialize(2, 44100); }

  ~CustomSoundStream() {
    oncleanup(userdata);
  }
 
  private:

  void clipBuffer() {
    for (size_t i = 0; i < BUFSIZ / sizeof(normalizer); ++i) 
      ((normalizer*)buffer)[i].normalize();
  }

  bool onGetData(Chunk& data) {
    data.samples = (short*) buffer;
    data.sampleCount = ongetdata(userdata, buffer, buffer_size)/sizeof(short);
    //clipBuffer();
    return true;
  }
 
  void onSeek(sf::Time timeOffset) {
    onseek(userdata, timeOffset.asSeconds());
  }
};

struct PlayableSound {
    virtual ~PlayableSound() {

    }
	virtual bool play() = 0;
	virtual bool pause() = 0;
    virtual void stop() = 0;
	virtual sf::SoundSource::Status getStatus() = 0;
	virtual sf::Time getPlayingOffset() = 0;
	virtual sf::Time getDuration() = 0;
	virtual unsigned int getSampleRate() = 0;
	virtual unsigned int getChannelCount() = 0;
	virtual float getVolume() = 0;
	virtual float getPitch() = 0;
	virtual float getMinDistance() = 0;
	virtual float getAttenuation() = 0;
	virtual sf::Vector3f getPosition() = 0;
	virtual float getX() = 0;
	virtual float getY() = 0;
	virtual float getZ() = 0;
	virtual bool setLoop(bool loop) = 0;
    virtual void setVolume(float volume) = 0;
	virtual void setPlayingOffset(sf::Time offset) = 0;
	virtual void setPosition(float x, float y, float z) = 0;
	virtual void setPitch(float pitch) = 0;
	virtual void setMinDistance(float distance) = 0;
	virtual void setAttenuation(float attenuation) = 0;
};

struct PlayableSoundInstance : public PlayableSound { 
	sf::Sound *sound; 
	sf::SoundBuffer *buffer; 

	PlayableSoundInstance(sf::Sound *s, sf::SoundBuffer *b): sound(s), buffer(b) {};
        
        ~PlayableSoundInstance() {
          delete sound;
          delete buffer;
        }

        bool play() {
          sound->play();
        }

        bool pause() {
          sound->pause();
        }

        void stop() {
          sound->stop();
        }

        sf::SoundSource::Status getStatus() {
          return sound->getStatus();
        }

	sf::Time getPlayingOffset() {
	  return sound->getPlayingOffset();
	} 

	sf::Time getDuration() {
	  return buffer->getDuration();
	}

	unsigned int getSampleRate() {
	  return buffer->getSampleRate();
	}

	unsigned int getChannelCount() {
	  return buffer->getChannelCount();
	}

	float getVolume() {
	  return sound->getVolume();
	}

	float getPitch() {
	  return sound->getPitch();
	}

	float getMinDistance() {
	  return sound->getMinDistance();
	}

	float getAttenuation() {
	  return sound->getAttenuation();
	}

	sf::Vector3f getPosition() {
	  return sound->getPosition();
	}

	float getX() {
	  return sound->getPosition().x;
	}

	float getY() {
	  return sound->getPosition().y;
	}

	float getZ() {
	  return sound->getPosition().z;
	}

        bool setLoop(bool loop) {
          sound->setLoop(loop);
        }

        void setVolume(float volume) {
	  sound->setVolume(volume);
        }

        void setPlayingOffset(sf::Time offset) {
	  sound->setPlayingOffset(offset);
	}

	void setPosition(float x, float y, float z) {
	  sound->setPosition(x, y, z);
	}

	void setPitch(float pitch) {
	  sound->setPitch(pitch);
	}

	void setMinDistance(float distance) {
	  sound->setMinDistance(distance);
	}

	void setAttenuation(float attenuation) {
	  sound->setAttenuation(attenuation);
	}
};

struct PlayableSoundStream : public PlayableSound {
	sf::SoundStream *stream;

        PlayableSoundStream(sf::SoundStream *s): stream(s) {};

        ~PlayableSoundStream() {
          delete stream;
        }

        bool play() {
          stream->play();
        }

        bool pause() {
          stream->pause();
        }

        void stop() {
          stream->stop();
        }

        sf::SoundSource::Status getStatus() {
          return stream->getStatus();
        }

	sf::Time getPlayingOffset() {
	  return stream->getPlayingOffset();
	} 

	sf::Time getDuration() {
	  // sfml soundstreams do not have a way of getting their duration
	  //return stream->getDuration();
	}

	unsigned int getSampleRate() {
	  return stream->getSampleRate();
	}

	unsigned int getChannelCount() {
	  return stream->getChannelCount();
	}

	float getVolume() {
	  return stream->getVolume();
	}

	float getPitch() {
	  return stream->getPitch();
	}

	float getMinDistance() {
	  return stream->getMinDistance();
	}

	float getAttenuation() {
	  return stream->getAttenuation();
	}

	sf::Vector3f getPosition() {
	  return stream->getPosition();
	}

	float getX() {
	  return stream->getPosition().x;
	}

	float getY() {
	  return stream->getPosition().y;
	}

	float getZ() {
	  return stream->getPosition().z;
	}

        bool setLoop(bool loop) {
          stream->setLoop(loop);
        }

        void setVolume(float volume) {
	  stream->setVolume(volume);
        }

        void setPlayingOffset(sf::Time offset) {
	  stream->setPlayingOffset(offset);
	}

	void setPosition(float x, float y, float z) {
	  stream->setPosition(x, y, z);
	}

	void setPitch(float pitch) {
	  stream->setPitch(pitch);
	}

	void setMinDistance(float distance) {
	  stream->setMinDistance(distance);
	}

	void setAttenuation(float attenuation) {
	  stream->setAttenuation(attenuation);
	}
};

std::vector<PlayableSound*> sounds;

namespace enigma
{
  int audiosystem_initialize() { 
    //TODO: Initialize audio system
    return 0; 
  }

  void audiosystem_update() {
    //TODO: Perform audio fall off and emitter updating
  }

  int sound_add_from_buffer(int id, void* buffer, size_t size) { 
    int i = (int)sounds.size();
    sf::SoundBuffer *buf = new sf::SoundBuffer();
    sf::Sound *snd;

    buf->loadFromMemory(buffer, size);
		
    snd = new sf::Sound();
    snd->setBuffer(*buf);
    sounds.push_back(new PlayableSoundInstance(snd, buf));
    return i;
  }

  void audiosystem_cleanup() {
    //TODO: Cleanup and delete/free sounds/existing buffers from memory
  }

  int sound_allocate() {
    int i = sounds.size();
    sounds.push_back(NULL);
    return i;
  }

  int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size), void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata), void *userdata)
  {
      CustomSoundStream *snd;

      snd = new CustomSoundStream(callback, seek, cleanup, userdata);
      sounds[id] = new PlayableSoundStream(snd);	
      return 0;
  }

}

namespace enigma_user {

int sound_add(std::string fname, int kind, bool preload)
{
  int i = (int)sounds.size();
  sf::SoundBuffer *buffer = new sf::SoundBuffer();
  sf::Sound *snd;
  
  snd = new sf::Sound();
  snd->setBuffer(*buffer);
  sounds.push_back(new PlayableSoundInstance(snd, buffer));
  return i;
}

bool sound_exists(int sound)
{
	return unsigned(sound) < sounds.size();
}

void sound_delete(int sound)
{
	delete sounds[sound];
}

bool sound_replace(int sound, std::string fname, int kind, bool preload)
{
  sf::SoundBuffer *buffer = new sf::SoundBuffer();
  sf::Sound *snd;
  
  delete sounds[sound];
  snd = new sf::Sound();
  snd->setBuffer(*buffer);
  sounds.push_back(new PlayableSoundInstance(snd, buffer));
  return true;
}

bool sound_play(int sound)
{
	sounds[sound]->play();
	return sounds[sound]->getStatus() == sf::Sound::Playing;
}

void action_sound(int snd, bool loop)
{
	(loop ? sound_loop:sound_play)(snd);
}

bool sound_loop(int sound)
{
	sounds[sound]->setLoop(true);
	sounds[sound]->play();
	return sounds[sound]->getStatus() == sf::Sound::Playing;
}

void sound_stop(int sound)
{
	sounds[sound]->setLoop(false);
	sounds[sound]->stop();
}

void sound_stop_all()
{
	for(unsigned i=0; i < sounds.size(); i++)
	{
	  sounds[i]->stop();
	}
}

bool sound_pause(int sound)
{
	sounds[sound]->pause();
	return sounds[sound]->getStatus() == sf::Sound::Paused;
}

void sound_pause_all()
{
	for(unsigned i=0; i < sounds.size(); i++)
	{
	  sounds[i]->pause();
	}
}
void sound_resume_all()
{
	for(unsigned i=0; i < sounds.size(); i++)
	{
	  if (sounds[i]->getStatus() == sf::Sound::Paused)
		sounds[i]->play();
	}
}

void sound_global_volume(float volume)
{
	sf::Listener::setGlobalVolume(volume);
}

void sound_set_volume(int sound, float volume)
{
	sounds[sound]->setVolume(volume);
}

void sound_seek(int sound, float position)
{
	sounds[sound]->setPlayingOffset(sf::seconds(position));
}

void sound_seek_all(float position)
{
	for(unsigned i=0; i < sounds.size(); i++)
	{
	  sounds[i]->setPlayingOffset(sf::seconds(position));
	}
}

void sound_set_pitch(int sound, float pitch)
{
	sounds[sound]->setPitch(pitch);
}

void sound_3d_set_sound_position(int sound, float x, float y, float z)
{
	sounds[sound]->setPosition(x, y, z);
}

void sound_3d_set_sound_distance(int sound, float mindist, float maxdist)
{
	sounds[sound]->setMinDistance(mindist);
	sounds[sound]->setAttenuation(1/(maxdist-mindist));
}

bool sound_isplaying(int sound)
{
	return sounds[sound]->getStatus() == sf::Sound::Playing;
}

bool sound_ispaused(int sound)
{
	return sounds[sound]->getStatus() == sf::Sound::Paused;
}

float sound_get_position(int sound)
{
	return sounds[sound]->getPlayingOffset().asSeconds();
}

float sound_get_length(int sound)
{
	return sounds[sound]->getDuration().asSeconds();
}

unsigned int sound_get_samplerate(int sound)
{
	return sounds[sound]->getSampleRate();
}

unsigned int sound_get_channels(int sound)
{
	return sounds[sound]->getChannelCount();
}

int sound_get_x(int sound)
{
	return sounds[sound]->getX();
}
int sound_get_y(int sound)
{
	return sounds[sound]->getY();
}
int sound_get_z(int sound)
{
	return sounds[sound]->getZ();
}

float sound_get_volume(int sound)
{
	return sounds[sound]->getVolume();
}

float sound_get_pitch(int sound)
{
	return sounds[sound]->getPitch();
}

}
