/** Copyright (C) 2013 Robert B. Colton
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

#include <stdio.h>

#include "DSsystem.h"
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
	
IDirectSound8* dsound;

const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } }; 

#include <time.h>
clock_t starttime;
clock_t elapsedtime;
clock_t lasttime;

int falloff_model = 0;
size_t channel_num = 128;

float listenerPos[] = {0.0f,0.0f,0.0f};
float listenerVel[] = {0.0f,0.0f,0.0f};
float listenerOri[] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};

#include <string>
using std::string;

#include "Platforms/Win32/WINDOWSmain.h"
IDirectSoundBuffer* primaryBuffer;

vector<SoundResource*> sound_resources(0);
	
namespace enigma {

  int get_free_channel(double priority)
  {

  }

  void eos_callback(void *soundID, unsigned src)
  {
    get_sound(snd, (ptrdiff_t)soundID, );
    snd->playing = false;
    snd->idle = true;
  }

  int audiosystem_initialize()
  {
  	starttime = clock();
    elapsedtime = starttime;
    lasttime = elapsedtime;
    printf("Initializing audio system...\n");
	
	HRESULT result;
	DSBUFFERDESC bufferDesc;
 
	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &dsound, NULL);
	if(FAILED(result))
	{
		MessageBox(NULL, "Failed to create DirectSound8 object.", "Error", MB_OK);
		return false;
	}
 
	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = dsound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(FAILED(result))
	{
		MessageBox(NULL, "Failed to set the cooperative level of the Window handle.", "Error", MB_OK);
		return false;
	}
	
	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
	dsound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, NULL);
	
	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	WAVEFORMATEX primaryFormat;
	primaryFormat.wFormatTag = WAVE_FORMAT_PCM;
	primaryFormat.nSamplesPerSec = 44100;
	primaryFormat.wBitsPerSample = 16;
	primaryFormat.nChannels = 2;
	primaryFormat.nBlockAlign = (primaryFormat.wBitsPerSample / 8) * primaryFormat.nChannels;
	primaryFormat.nAvgBytesPerSec = primaryFormat.nSamplesPerSec * primaryFormat.nBlockAlign;
	primaryFormat.cbSize = 0;
	
	primaryBuffer->SetFormat(&primaryFormat);
	
	return true;
  }

  SoundResource* sound_new_with_source() {
    SoundResource *res = new SoundResource();
    res->loaded = LOADSTATE_SOURCED;
    return res;
  }


  int sound_add_from_buffer(int id, void* buffer, size_t bufsize)
  {
    SoundResource *snd = sound_new_with_source();
	if (id > sound_resources.size()) {
		sound_resources.resize(id + 1);
	}
    sound_resources.insert(sound_resources.begin() + id, snd);
    if (snd->loaded != LOADSTATE_SOURCED) {
      //fprintf(stderr, "Could not load sound %d: %s\n", id, alureGetErrorString());
      return 1;
    }
	  
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 16000;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
	
    unsigned int& buf = snd->buf[0];
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = bufsize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
	dsound->CreateSoundBuffer(&bufferDesc, &snd->soundBuffer, NULL);
	
	LPVOID lpvWrite;
	DWORD  dwLength;

	IDirectSoundBuffer* sndBuf = snd->soundBuffer;
	//sndBuf->SetFormat(&waveFormat);
	if (DS_OK == sndBuf->Lock(
      0,          // Offset at which to start lock.
      0,          // Size of lock; ignored because of flag.
      &lpvWrite,  // Gets address of first part of lock.
      &dwLength,  // Gets size of first part of lock.
      NULL,       // Address of wraparound not needed. 
      NULL,       // Size of wraparound not needed.
      DSBLOCK_ENTIREBUFFER))  // Flag.
	{
		memcpy(lpvWrite, buffer, dwLength);
		sndBuf->Unlock(
		lpvWrite,   // Address of lock start.
		dwLength,   // Size of lock.
		NULL,       // No wraparound portion.
		0);         // No wraparound size.
	} else {
		//ErrorHandler();  // Add error-handling here.
	}
	
	snd->soundBuffer->SetCurrentPosition(0);
	// Set volume of the buffer to 100%.
	snd->soundBuffer->SetVolume(DSBVOLUME_MAX);
	
    if (!buf) {
      //fprintf(stderr, "Could not load sound %d: %s\n", id, alureGetErrorString());
      return 2;
    }

    snd->loaded = LOADSTATE_COMPLETE;
    return 0;
  }


  int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size), void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata), void *userdata)
  {

  }

  int sound_allocate()
  {
	int id = -1;
	for (int i = 0; i < sound_resources.size(); i++) {
		if (sound_resources[id] == NULL) {
			id = i;
		}
	}
	if (id < 0) {
	  id = sound_resources.size();
	}
	if (id > sound_resources.size()) {
		sound_resources.resize(id + 1);
	}
	sound_resources.insert(sound_resources.begin() + id, new SoundResource());
	return id;
  }

  void audiosystem_update(void)
  {

  }

  void audiosystem_cleanup()
  {
	// Release the direct sound interface pointer.
	if(dsound)
	{
		dsound->Release();
		dsound = 0;
	}
 
	return;
  }

}
