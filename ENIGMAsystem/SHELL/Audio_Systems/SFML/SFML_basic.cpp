/** Copyright (C) 2008-2013 Cheeseboy, Robert B. Colton 
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

#include <SFML/Audio.hpp>
#include <vector>
#include "SFML_basic.h"

struct PlayableSound 
{ 
	sf::Sound *sound; 
	sf::SoundBuffer *buffer; 
	PlayableSound(sf::Sound *s, sf::SoundBuffer *b): sound(s), buffer(b) {};
};

std::vector<PlayableSound> sounds;

int sound_add(std::string fname, int kind, bool preload)
{
  int i = (int)sounds.size();
  sf::SoundBuffer *buffer = new sf::SoundBuffer();
  sf::Sound *snd;
		
  if (!buffer->loadFromFile(fname))
  {
	delete buffer;
    return -1;
  }
  else
  {
    snd = new sf::Sound();
    snd->setBuffer(*buffer);
    sounds.push_back(PlayableSound(snd, buffer));
    return i;
  }
}
bool sound_exists(int sound)
{
	return unsigned(sound) < sounds.size() && sounds[sound].sound;
}

void sound_delete(int sound)
{
	delete sounds[sound].buffer;
	sounds[sound].buffer = NULL;
}

bool sound_replace(int sound, std::string fname, int kind, bool preload)
{
  sf::SoundBuffer *buffer = new sf::SoundBuffer();
		
  if (!buffer->loadFromFile(fname))
  {
    delete buffer;
    return -1;
  }
  else
  {
    delete sounds[sound].buffer;
    sounds[sound].buffer = buffer;
    sounds[sound].sound->setBuffer(*buffer);
    return true;
  }
}

bool sound_play(int sound)
{
	sounds[sound].sound->play();
	return sounds[sound].sound->getStatus() == sf::Sound::Playing;
}

bool sound_loop(int sound)
{
	sounds[sound].sound->setLoop(true);
	sounds[sound].sound->play();
	return sounds[sound].sound->getStatus() == sf::Sound::Playing;
}

void sound_stop(int sound)
{
	sounds[sound].sound->setLoop(false);
	sounds[sound].sound->stop();
}
void sound_stop_all()
{
	for(unsigned i=0; i < sounds.size(); i++)
	{
		sounds[i].sound->stop();
	}
}

bool sound_pause(int sound)
{
	sounds[sound].sound->pause();
	return sounds[sound].sound->getStatus() == sf::Sound::Paused;
}
void sound_pause_all()
{
	for(unsigned i=0; i < sounds.size(); i++)
	{
		sounds[i].sound->pause();
	}
}
void sound_resume_all()
{
	for(unsigned i=0; i < sounds.size(); i++)
	{
		if (sounds[i].sound->getStatus() == sf::Sound::Paused)
			sounds[i].sound->play();
	}
}

void sound_global_volume(float volume)
{
	sf::Listener::setGlobalVolume(volume);
}

void sound_set_volume(int sound, float volume)
{
	sounds[sound].sound->setVolume(volume);
}

void sound_seek(int sound, float position)
{
	sounds[sound].sound->setPlayingOffset(sf::seconds(position));
}
void sound_seek_all(float position)
{
	for(unsigned i=0; i < sounds.size(); i++)
	{
		sounds[i].sound->setPlayingOffset(sf::seconds(position));
	}
}

void sound_set_pitch(int sound, float pitch)
{
	sounds[sound].sound->setPitch(pitch);
}

void sound_3d_set_sound_position(int sound,int x,int y,int z)
{
	sounds[sound].sound->setPosition(x,y,z);
}

void sound_3d_set_sound_distance(int sound, float mindist, float maxdist)
{
	sounds[sound].sound->setMinDistance(mindist);
	sounds[sound].sound->setAttenuation(1/(maxdist-mindist));
}

bool sound_isplaying(int sound)
{
	return sounds[sound].sound->getStatus() == sf::Sound::Playing;
}

bool sound_ispaused(int sound)
{
	return sounds[sound].sound->getStatus() == sf::Sound::Paused;
}

float sound_get_position(int sound)
{
	return sounds[sound].sound->getPlayingOffset().asSeconds();
}

float sound_get_length(int sound)
{
	return sounds[sound].buffer->getDuration().asSeconds();
}

int sound_get_samplerate(int sound)
{
	return sounds[sound].buffer->getSampleRate();
}

int sound_get_channels(int sound)
{
	return sounds[sound].buffer->getChannelCount();
}

int sound_get_x(int sound)
{
	return sounds[sound].sound->getPosition().x;
}
int sound_get_y(int sound)
{
	return sounds[sound].sound->getPosition().y;
}
int sound_get_z(int sound)
{
	return sounds[sound].sound->getPosition().z;
}

float sound_get_volume(int sound)
{
	return sounds[sound].sound->getVolume();
}

float sound_get_pitch(int sound)
{
	return sounds[sound].sound->getPitch();
}
