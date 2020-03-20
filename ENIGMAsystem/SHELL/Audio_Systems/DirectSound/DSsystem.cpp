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

#include "Widget_Systems/widgets_mandatory.h"  // DEBUG_MESSAGE

#include "Bridges/Win32/WINDOWShandle.h" // enigma::hWnd/hInstance

#include <cstdio>

#include "DSsystem.h"
struct WaveHeaderType {
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
  unsigned short parSize;
  unsigned short headerSize;
  unsigned long dataOffset;
  char dataChunkId[4];
  unsigned long dataSize;
};

IDirectSound8* dsound;

const GUID GUID_NULL = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};

#include <time.h>
clock_t starttime;
clock_t elapsedtime;
clock_t lasttime;

#include <sstream>
#include <string>
using std::string;
using std::stringstream;

IDirectSoundBuffer* primaryBuffer;

AssetArray<Sound> sounds;

namespace enigma {

extern HWND hWnd;

void eos_callback(void* soundID, unsigned src) {
  //auto snd = sounds.get((ptrdiff_t)soundID);
  //snd.playing = false;
  //snd.idle = true;
}

int audiosystem_initialize() {
  if (enigma_user::window_handle() == NULL) {
    DEBUG_MESSAGE("Window handle is NULL.", MESSAGE_TYPE::M_ERROR);
    return false;
  }

  starttime = clock();
  elapsedtime = starttime;
  lasttime = elapsedtime;
  DEBUG_MESSAGE("Initializing audio system...", MESSAGE_TYPE::M_INFO);

  HRESULT result;
  DSBUFFERDESC bufferDesc;

  // Initialize the direct sound interface pointer for the default sound device.
  result = DirectSoundCreate8(NULL, &dsound, NULL);
  if (FAILED(result)) {
    DEBUG_MESSAGE("Failed to create DirectSound8 object.", MESSAGE_TYPE::M_ERROR);
    return false;
  }

  // Set the cooperative level to priority so the format of the primary sound buffer can be modified.
  result = dsound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
  if (FAILED(result)) {
    DEBUG_MESSAGE("Failed to set the cooperative level of the Window handle.", MESSAGE_TYPE::M_ERROR);
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

WaveHeaderType* buffer_get_wave_header(char* buffer, size_t bufsize) {
  WaveHeaderType* waveHeader = new WaveHeaderType();

  memcpy(waveHeader, buffer, 36);

  unsigned long remaining = waveHeader->subChunkSize - 16;
  if (remaining) {
    memcpy(&waveHeader->parSize, buffer + 36, 2);
  }

  if (strncmp(waveHeader->chunkId, "RIFF", 4) != 0 || strncmp(waveHeader->format, "WAVE", 4) != 0) {
    return NULL;  // Not a WAVE file or the format is just messed up
  }

  // Read Subchunks
  char chunkId[5] = {'H', 'U', 'N', 'G'};
  unsigned long chunkSize = 0;
  for (unsigned i = 36 + remaining; i < bufsize; i += 8 + chunkSize) {
    memcpy(&chunkId, buffer + i, 4);
    memcpy(&chunkSize, buffer + i + 4, 4);
    if (strncmp(chunkId, "data", 4) == 0) {
      waveHeader->dataSize = chunkSize;
      waveHeader->dataOffset = i + 8;
      break;
    } else {
      continue;
    }
  }

  return waveHeader;
}

int sound_add_from_buffer(int id, void* buffer, size_t bufsize) {
  Sound snd;

  WaveHeaderType* waveHeader = buffer_get_wave_header((char*)buffer, bufsize);
  WAVEFORMATEX waveFormat = {};
  waveFormat.wFormatTag = waveHeader->audioFormat;
  waveFormat.nSamplesPerSec = waveHeader->sampleRate;
  waveFormat.wBitsPerSample = waveHeader->bitsPerSample;
  waveFormat.nChannels = waveHeader->numChannels;
  waveFormat.nBlockAlign = waveHeader->blockAlign;
  waveFormat.nAvgBytesPerSec = waveHeader->bytesPerSecond;
  waveFormat.cbSize = waveHeader->parSize;

  DSBUFFERDESC bufferDesc = {};
  bufferDesc.dwSize = sizeof(DSBUFFERDESC);
  // DSBCAPS_CTRLFX causes all kinds of weird nasty shit, please read #1508 on GitHub
  // before considering whether to readd it to the dwFlags below
  bufferDesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
  bufferDesc.dwBufferBytes = waveHeader->dataSize;
  bufferDesc.dwReserved = 0;
  bufferDesc.lpwfxFormat = &waveFormat;
  bufferDesc.guid3DAlgorithm = GUID_NULL;
  dsound->CreateSoundBuffer(&bufferDesc, &snd.soundBuffer, NULL);

  auto sndBuf = snd.soundBuffer;
  LPVOID lpvWrite;
  DWORD dwLength;

  if (DS_OK == sndBuf->Lock(0,                     // Offset at which to start lock.
                            waveHeader->dataSize,  // Size of lock; ignored because of flag.
                            &lpvWrite,             // Gets address of first part of lock.
                            &dwLength,             // Gets size of first part of lock.
                            NULL,                  // Address of wraparound not needed.
                            NULL,                  // Size of wraparound not needed.
                            0))                    // Flag.
  {
    memcpy(lpvWrite, (char*)buffer + waveHeader->dataOffset, waveHeader->dataSize);
    sndBuf->Unlock(lpvWrite,  // Address of lock start.
                   dwLength,  // Size of lock.
                   NULL,      // No wraparound portion.
                   0);        // No wraparound size.
  } else {
    //ErrorHandler();  // Add error-handling here.
  }

  float channels = waveHeader->numChannels,
        freq = waveHeader->sampleRate,
        bits = waveHeader->bitsPerSample,
        size = waveHeader->dataSize;

  snd.length = size / channels / (bits / 8) / freq;

  delete waveHeader;

  snd.soundBuffer->SetCurrentPosition(0);
  // Set volume of the buffer to 100%.
  snd.soundBuffer->SetVolume(0);
  snd.loaded = LOADSTATE_COMPLETE;

  sounds.assign(id, std::move(snd));
  return 0;
}

int sound_add_from_stream(int id, size_t (*callback)(void* userdata, void* buffer, size_t size),
                          void (*seek)(void* userdata, float position), void (*cleanup)(void* userdata),
                          void* userdata) {
  return -1;
}

void audiosystem_update(void) {}

void audiosystem_cleanup() {
  // Release the direct sound interface pointer.
  if (dsound) {
    dsound->Release();
    dsound = 0;
  }

  return;
}

}  // namespace enigma
