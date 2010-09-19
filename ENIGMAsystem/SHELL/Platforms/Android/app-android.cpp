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
#include <sys/time.h>
#include <time.h>

#include <stdint.h>
#include "org_enigmadev_EnigmaRenderer.h"
#include <stdio.h>
#include "AndroidFunctions.h"

#include <android/log.h>
#define  LOG_TAG    "libenigma"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


int   gAppAlive   = 1;

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

static long
_getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}


JNIEXPORT void JNICALL Java_org_enigmadev_EnigmaRenderer_nativeInit
(JNIEnv *, jclass)
{
	LOGE("Starting enigma Game!",1,1);
	init();
}


JNIEXPORT void JNICALL Java_org_enigmadev_EnigmaRenderer_nativeResize
(JNIEnv *, jclass, jint, jint)
{
  //call to resize
}


JNIEXPORT void JNICALL Java_org_enigmadev_EnigmaRenderer_nativeDone
(JNIEnv *, jclass)
{
   //cleanup
}


JNIEXPORT void
Java_org_enigmadev_SanAngeles_EnigmaGLSurfaceView_nativePause( JNIEnv*  env )
{
 //pause
}


JNIEXPORT void JNICALL Java_org_enigmadev_EnigmaRenderer_nativeRender
(JNIEnv *, jclass)
{
	//run the main loop
	loopy();
}
