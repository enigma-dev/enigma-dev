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

#include <jni.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string>
using std::string;

extern JNIEnv *_my_jnienv;
void set_jnienv(JNIEnv *env);
JNIEnv * /**env*/ get_jnienv();

namespace enigma {

struct sound {
  int src, buf;  // The source-id and buffer-id of the sound data
  int loaded;    // Degree to which this sound has been loaded successfully
  bool idle;     // True if this sound is not being used, false if playing or paused.
  bool playing;  // True if this sound is playing; not paused or idle.
  sound() : src(0), buf(0), loaded(0), idle(1), playing(0) {}
};

sound *sounds;
size_t numSounds;
extern size_t sound_idmax;

int audiosystem_initialize() {
  numSounds = sound_idmax > 0 ? sound_idmax : 1;
  sounds = new sound[numSounds];

  //call to java

  for (size_t i = 0; i < numSounds; i++) {
    //call to java
    sounds[i].loaded = 1;
  }

  return 0;
}

int sound_add_from_buffer(int id, void *buffer, size_t bufsize) {
  //call to java
  sounds[id].loaded = 2;
  return 0;
}

void audiosystem_update(void) {
  //call to java
}

void audiosystem_cleanup() {
  //call to java
}

/* functions for C to call java */

int jni_load_sound(const char *s) {
  JNIEnv *env = get_jnienv();
  jclass cls = env->FindClass("org/enigmadev/EnigmaActivity");
  jmethodID mid = env->GetStaticMethodID(cls, "sound_load", "(Ljava/lang/String;)I");
  // there could be some exception handling happening here, but there isn't
  jint ret;
  jstring mystr = env->NewStringUTF(s);
  ret = env->CallStaticIntMethod(cls, mid, mystr);
  return ret;
}

int jni_play_sound(int i) {
  JNIEnv *env = get_jnienv();
  jclass cls = env->FindClass("org/enigmadev/EnigmaActivity");
  jmethodID mid = env->GetStaticMethodID(cls, "sound_play", "(I)I");
  // there could be some exception handling happening here, but there isn't
  jint ret;
  //jstring mystr = env->NewStringUTF(s);
  jint ii = i;
  ret = env->CallStaticIntMethod(cls, mid, ii);
  return ret;
}
};  // namespace enigma

int id = 0;

namespace enigma_user {

bool sound_play(int sound)  // Returns nonzero if an error occurred
{
  enigma::jni_play_sound(sound);
  enigma::sound &snd = enigma::sounds[sound];  //snd.looping = false;
  //alSourcei(snd.src, AL_LOOPING, AL_FALSE); //Just playing
  //alSourcePlay(snd.src);
  return 1;
  //return snd.idle = !(snd.playing = (alurePlaySource(snd.src, enigma::eos_callback, (void*)sound) != AL_FALSE));
}

int sound_add(string fname, int kind, bool preload)  //last two arguments do nothing
{
  return enigma::jni_load_sound(fname.c_str());
}

}  // namespace enigma_user
