/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Kitty Cat <http://kcat.strangesoft.net/>                 **
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

#ifndef _WRAP_OAL__H
#define _WRAP_OAL__H

extern LPALCCREATECONTEXT palcCreateContext;
extern LPALCMAKECONTEXTCURRENT palcMakeContextCurrent;
extern LPALCPROCESSCONTEXT palcProcessContext;
extern LPALCSUSPENDCONTEXT palcSuspendContext;
extern LPALCDESTROYCONTEXT palcDestroyContext;
extern LPALCGETCURRENTCONTEXT palcGetCurrentContext;
extern LPALCGETCONTEXTSDEVICE palcGetContextsDevice;
extern LPALCOPENDEVICE palcOpenDevice;
extern LPALCCLOSEDEVICE palcCloseDevice;
extern LPALCGETERROR palcGetError;
extern LPALCISEXTENSIONPRESENT palcIsExtensionPresent;
extern LPALCGETPROCADDRESS palcGetProcAddress;
extern LPALCGETENUMVALUE palcGetEnumValue;
extern LPALCGETSTRING palcGetString;
extern LPALCGETINTEGERV palcGetIntegerv;
extern LPALCCAPTUREOPENDEVICE palcCaptureOpenDevice;
extern LPALCCAPTURECLOSEDEVICE palcCaptureCloseDevice;
extern LPALCCAPTURESTART palcCaptureStart;
extern LPALCCAPTURESTOP palcCaptureStop;
extern LPALCCAPTURESAMPLES palcCaptureSamples;

extern LPALENABLE palEnable;
extern LPALDISABLE palDisable;
extern LPALISENABLED palIsEnabled;
extern LPALGETSTRING palGetString;
extern LPALGETBOOLEANV palGetBooleanv;
extern LPALGETINTEGERV palGetIntegerv;
extern LPALGETFLOATV palGetFloatv;
extern LPALGETDOUBLEV palGetDoublev;
extern LPALGETBOOLEAN palGetBoolean;
extern LPALGETINTEGER palGetInteger;
extern LPALGETFLOAT palGetFloat;
extern LPALGETDOUBLE palGetDouble;
extern LPALGETERROR palGetError;
extern LPALISEXTENSIONPRESENT palIsExtensionPresent;
extern LPALGETPROCADDRESS palGetProcAddress;
extern LPALGETENUMVALUE palGetEnumValue;
extern LPALLISTENERF palListenerf;
extern LPALLISTENER3F palListener3f;
extern LPALLISTENERFV palListenerfv;
extern LPALLISTENERI palListeneri;
extern LPALLISTENER3I palListener3i;
extern LPALLISTENERIV palListeneriv;
extern LPALGETLISTENERF palGetListenerf;
extern LPALGETLISTENER3F palGetListener3f;
extern LPALGETLISTENERFV palGetListenerfv;
extern LPALGETLISTENERI palGetListeneri;
extern LPALGETLISTENER3I palGetListener3i;
extern LPALGETLISTENERIV palGetListeneriv;
extern LPALGENSOURCES palGenSources;
extern LPALDELETESOURCES palDeleteSources;
extern LPALISSOURCE palIsSource;
extern LPALSOURCEF palSourcef;
extern LPALSOURCE3F palSource3f;
extern LPALSOURCEFV palSourcefv;
extern LPALSOURCEI palSourcei;
extern LPALSOURCE3I palSource3i;
extern LPALSOURCEIV palSourceiv;
extern LPALGETSOURCEF palGetSourcef;
extern LPALGETSOURCE3F palGetSource3f;
extern LPALGETSOURCEFV palGetSourcefv;
extern LPALGETSOURCEI palGetSourcei;
extern LPALGETSOURCE3I palGetSource3i;
extern LPALGETSOURCEIV palGetSourceiv;
extern LPALSOURCEPLAYV palSourcePlayv;
extern LPALSOURCESTOPV palSourceStopv;
extern LPALSOURCEREWINDV palSourceRewindv;
extern LPALSOURCEPAUSEV palSourcePausev;
extern LPALSOURCEPLAY palSourcePlay;
extern LPALSOURCESTOP palSourceStop;
extern LPALSOURCEREWIND palSourceRewind;
extern LPALSOURCEPAUSE palSourcePause;
extern LPALSOURCEQUEUEBUFFERS palSourceQueueBuffers;
extern LPALSOURCEUNQUEUEBUFFERS palSourceUnqueueBuffers;
extern LPALGENBUFFERS palGenBuffers;
extern LPALDELETEBUFFERS palDeleteBuffers;
extern LPALISBUFFER palIsBuffer;
extern LPALBUFFERF palBufferf;
extern LPALBUFFER3F palBuffer3f;
extern LPALBUFFERFV palBufferfv;
extern LPALBUFFERI palBufferi;
extern LPALBUFFER3I palBuffer3i;
extern LPALBUFFERIV palBufferiv;
extern LPALGETBUFFERF palGetBufferf;
extern LPALGETBUFFER3F palGetBuffer3f;
extern LPALGETBUFFERFV palGetBufferfv;
extern LPALGETBUFFERI palGetBufferi;
extern LPALGETBUFFER3I palGetBuffer3i;
extern LPALGETBUFFERIV palGetBufferiv;
extern LPALBUFFERDATA palBufferData;
extern LPALDOPPLERFACTOR palDopplerFactor;
extern LPALDOPPLERVELOCITY palDopplerVelocity;
extern LPALDISTANCEMODEL palDistanceModel;
extern LPALSPEEDOFSOUND palSpeedOfSound;

#define alcCreateContext palcCreateContext
#define alcMakeContextCurrent palcMakeContextCurrent
#define alcProcessContext palcProcessContext
#define alcSuspendContext palcSuspendContext
#define alcDestroyContext palcDestroyContext
#define alcGetCurrentContext palcGetCurrentContext
#define alcGetContextsDevice palcGetContextsDevice
#define alcOpenDevice palcOpenDevice
#define alcCloseDevice palcCloseDevice
#define alcGetError palcGetError
#define alcIsExtensionPresent palcIsExtensionPresent
#define alcGetProcAddress palcGetProcAddress
#define alcGetEnumValue palcGetEnumValue
#define alcGetString palcGetString
#define alcGetIntegerv palcGetIntegerv
#define alcCaptureOpenDevice palcCaptureOpenDevice
#define alcCaptureCloseDevice palcCaptureCloseDevice
#define alcCaptureStart palcCaptureStart
#define alcCaptureStop palcCaptureStop
#define alcCaptureSamples palcCaptureSamples
#define alEnable palEnable
#define alDisable palDisable
#define alIsEnabled palIsEnabled
#define alGetString palGetString
#define alGetBooleanv palGetBooleanv
#define alGetIntegerv palGetIntegerv
#define alGetFloatv palGetFloatv
#define alGetDoublev palGetDoublev
#define alGetBoolean palGetBoolean
#define alGetInteger palGetInteger
#define alGetFloat palGetFloat
#define alGetDouble palGetDouble
#define alGetError palGetError
#define alIsExtensionPresent palIsExtensionPresent
#define alGetProcAddress palGetProcAddress
#define alGetEnumValue palGetEnumValue
#define alListenerf palListenerf
#define alListener3f palListener3f
#define alListenerfv palListenerfv
#define alListeneri palListeneri
#define alListener3i palListener3i
#define alListeneriv palListeneriv
#define alGetListenerf palGetListenerf
#define alGetListener3f palGetListener3f
#define alGetListenerfv palGetListenerfv
#define alGetListeneri palGetListeneri
#define alGetListener3i palGetListener3i
#define alGetListeneriv palGetListeneriv
#define alGenSources palGenSources
#define alDeleteSources palDeleteSources
#define alIsSource palIsSource
#define alSourcef palSourcef
#define alSource3f palSource3f
#define alSourcefv palSourcefv
#define alSourcei palSourcei
#define alSource3i palSource3i
#define alSourceiv palSourceiv
#define alGetSourcef palGetSourcef
#define alGetSource3f palGetSource3f
#define alGetSourcefv palGetSourcefv
#define alGetSourcei palGetSourcei
#define alGetSource3i palGetSource3i
#define alGetSourceiv palGetSourceiv
#define alSourcePlayv palSourcePlayv
#define alSourceStopv palSourceStopv
#define alSourceRewindv palSourceRewindv
#define alSourcePausev palSourcePausev
#define alSourcePlay palSourcePlay
#define alSourceStop palSourceStop
#define alSourceRewind palSourceRewind
#define alSourcePause palSourcePause
#define alSourceQueueBuffers palSourceQueueBuffers
#define alSourceUnqueueBuffers palSourceUnqueueBuffers
#define alGenBuffers palGenBuffers
#define alDeleteBuffers palDeleteBuffers
#define alIsBuffer palIsBuffer
#define alBufferf palBufferf
#define alBuffer3f palBuffer3f
#define alBufferfv palBufferfv
#define alBufferi palBufferi
#define alBuffer3i palBuffer3i
#define alBufferiv palBufferiv
#define alGetBufferf palGetBufferf
#define alGetBuffer3f palGetBuffer3f
#define alGetBufferfv palGetBufferfv
#define alGetBufferi palGetBufferi
#define alGetBuffer3i palGetBuffer3i
#define alGetBufferiv palGetBufferiv
#define alBufferData palBufferData
#define alDopplerFactor palDopplerFactor
#define alDopplerVelocity palDopplerVelocity
#define alDistanceModel palDistanceModel
#define alSpeedOfSound palSpeedOfSound

bool load_al_dll();

#endif

