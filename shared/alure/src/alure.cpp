/*
 * ALURE  OpenAL utility library
 * Copyright (c) 2009 by Chris Robinson.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/* Title: Main and Miscellanious */

#include "config.h"

#include "main.h"

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#include <vector>
#include <string>
#include <map>

std::map<ALint,UserCallbacks> InstalledCallbacks;
CRITICAL_SECTION cs_StreamPlay;
alureStream::ListType alureStream::StreamList;

PFNALCSETTHREADCONTEXTPROC palcSetThreadContext;
PFNALCGETTHREADCONTEXTPROC palcGetThreadContext;


template<typename T>
static inline void LoadALCProc(ALCdevice *dev, const char *name, T **ptr)
{ *ptr = reinterpret_cast<T*>(alcGetProcAddress(dev, name)); }


#ifdef HAVE_GCC_CONSTRUCTOR
static void init_alure(void) __attribute__((constructor));
static void deinit_alure(void) __attribute__((destructor));
static struct MyConstructorClass {
    ~MyConstructorClass()
    { alureStream::Clear(); };
} MyConstructor;
#elif defined(_WIN32) && !defined(ALURE_STATIC_LIBRARY)
static void init_alure(void);
static void deinit_alure(void);
static struct MyConstructorClass {
    ~MyConstructorClass()
    { alureStream::Clear(); };
} MyConstructor;

extern "C" BOOL APIENTRY DllMain(HINSTANCE module, DWORD reason, LPVOID /*reserved*/)
{
    // Perform actions based on the reason for calling.
    switch(reason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(module);
            init_alure();
            break;

        case DLL_PROCESS_DETACH:
            deinit_alure();
            break;
    }
    return TRUE;
}
#else
static void init_alure(void);
static void deinit_alure(void);

static struct MyConstructorClass {
    MyConstructorClass()
    { init_alure(); };
    ~MyConstructorClass()
    { alureStream::Clear();
      deinit_alure(); };
} MyConstructor;
#endif

static void init_alure(void)
{
    InitializeCriticalSection(&cs_StreamPlay);

    if(alcIsExtensionPresent(NULL, "ALC_EXT_thread_local_context"))
    {
        LoadALCProc(NULL, "alcSetThreadContext", &palcSetThreadContext);
        LoadALCProc(NULL, "alcGetThreadContext", &palcGetThreadContext);
        if(!palcSetThreadContext || !palcGetThreadContext)
        {
            fprintf(stderr, "Alure lib: ALC_EXT_thread_local_context advertised, but missing function:\n"
                            "    alcSetThreadContext=%p\n"
                            "    alcGetThreadContext=%p\n",
                            palcSetThreadContext, palcGetThreadContext);
            palcSetThreadContext = NULL;
            palcGetThreadContext = NULL;
        }
    }
}

static void deinit_alure(void)
{
    alureUpdateInterval(0.0f);
    DeleteCriticalSection(&cs_StreamPlay);
}


#ifndef HAVE_WINDOWS_H

void EnterCriticalSection(CRITICAL_SECTION *cs)
{
    int ret;
    ret = pthread_mutex_lock(cs);
    assert(ret == 0);
}
void LeaveCriticalSection(CRITICAL_SECTION *cs)
{
    int ret;
    ret = pthread_mutex_unlock(cs);
    assert(ret == 0);
}
void InitializeCriticalSection(CRITICAL_SECTION *cs)
{
    pthread_mutexattr_t attrib;
    int ret;

    ret = pthread_mutexattr_init(&attrib);
    assert(ret == 0);

    ret = pthread_mutexattr_settype(&attrib, PTHREAD_MUTEX_RECURSIVE);
#ifdef HAVE_PTHREAD_NP_H
    if(ret != 0)
        ret = pthread_mutexattr_setkind_np(&attrib, PTHREAD_MUTEX_RECURSIVE);
#endif
    assert(ret == 0);
    ret = pthread_mutex_init(cs, &attrib);
    assert(ret == 0);

    pthread_mutexattr_destroy(&attrib);
}
void DeleteCriticalSection(CRITICAL_SECTION *cs)
{
    int ret;
    ret = pthread_mutex_destroy(cs);
    assert(ret == 0);
}

#endif


static const ALchar *last_error = "No error";
void SetError(const char *err)
{
    last_error = err;
}


ALuint DetectBlockAlignment(ALenum format)
{
    switch(format)
    {
#define CHECK_RET(f,s) case (f): return (s)
        CHECK_RET(AL_FORMAT_MONO8, sizeof(ALubyte));
        CHECK_RET(AL_FORMAT_MONO16, sizeof(ALshort));
        CHECK_RET(AL_FORMAT_MONO_FLOAT32, sizeof(ALfloat));
        CHECK_RET(AL_FORMAT_MONO_DOUBLE_EXT, sizeof(ALdouble));
        CHECK_RET(AL_FORMAT_MONO_MULAW, sizeof(ALubyte)*1);

        CHECK_RET(AL_FORMAT_STEREO8, sizeof(ALubyte)*2);
        CHECK_RET(AL_FORMAT_STEREO16, sizeof(ALshort)*2);
        CHECK_RET(AL_FORMAT_STEREO_FLOAT32, sizeof(ALfloat)*2);
        CHECK_RET(AL_FORMAT_STEREO_DOUBLE_EXT, sizeof(ALdouble)*2);
        CHECK_RET(AL_FORMAT_STEREO_MULAW, sizeof(ALubyte)*2);

        CHECK_RET(AL_FORMAT_QUAD8, sizeof(ALubyte)*4);
        CHECK_RET(AL_FORMAT_QUAD16, sizeof(ALshort)*4);
        CHECK_RET(AL_FORMAT_QUAD32, sizeof(ALfloat)*4);
        CHECK_RET(AL_FORMAT_QUAD_MULAW, sizeof(ALubyte)*4);

        CHECK_RET(AL_FORMAT_REAR8, sizeof(ALubyte)*2);
        CHECK_RET(AL_FORMAT_REAR16, sizeof(ALshort)*2);
        CHECK_RET(AL_FORMAT_REAR32, sizeof(ALfloat)*2);
        CHECK_RET(AL_FORMAT_REAR_MULAW, sizeof(ALubyte)*2);

        CHECK_RET(AL_FORMAT_51CHN8, sizeof(ALubyte)*6);
        CHECK_RET(AL_FORMAT_51CHN16, sizeof(ALshort)*6);
        CHECK_RET(AL_FORMAT_51CHN32, sizeof(ALfloat)*6);
        CHECK_RET(AL_FORMAT_51CHN_MULAW, sizeof(ALubyte)*6);

        CHECK_RET(AL_FORMAT_61CHN8, sizeof(ALubyte)*7);
        CHECK_RET(AL_FORMAT_61CHN16, sizeof(ALshort)*7);
        CHECK_RET(AL_FORMAT_61CHN32, sizeof(ALfloat)*7);
        CHECK_RET(AL_FORMAT_61CHN_MULAW, sizeof(ALubyte)*7);

        CHECK_RET(AL_FORMAT_71CHN8, sizeof(ALubyte)*8);
        CHECK_RET(AL_FORMAT_71CHN16, sizeof(ALshort)*8);
        CHECK_RET(AL_FORMAT_71CHN32, sizeof(ALfloat)*8);
        CHECK_RET(AL_FORMAT_71CHN_MULAW, sizeof(ALubyte)*8);

        CHECK_RET(AL_FORMAT_MONO_IMA4, 36);
        CHECK_RET(AL_FORMAT_STEREO_IMA4, 36*2);
#undef CHECK_RET
    }
    return 0;
}

ALuint DetectCompressionRate(ALenum format)
{
    switch(format)
    {
    case AL_FORMAT_MONO8:
    case AL_FORMAT_MONO16:
    case AL_FORMAT_MONO_FLOAT32:
    case AL_FORMAT_MONO_DOUBLE_EXT:
    case AL_FORMAT_STEREO8:
    case AL_FORMAT_STEREO16:
    case AL_FORMAT_STEREO_FLOAT32:
    case AL_FORMAT_STEREO_DOUBLE_EXT:
    case AL_FORMAT_QUAD8:
    case AL_FORMAT_QUAD16:
    case AL_FORMAT_QUAD32:
    case AL_FORMAT_REAR8:
    case AL_FORMAT_REAR16:
    case AL_FORMAT_REAR32:
    case AL_FORMAT_51CHN8:
    case AL_FORMAT_51CHN16:
    case AL_FORMAT_51CHN32:
    case AL_FORMAT_61CHN8:
    case AL_FORMAT_61CHN16:
    case AL_FORMAT_61CHN32:
    case AL_FORMAT_71CHN8:
    case AL_FORMAT_71CHN16:
    case AL_FORMAT_71CHN32:
        return 1;

    case AL_FORMAT_MONO_MULAW:
    case AL_FORMAT_STEREO_MULAW:
    case AL_FORMAT_QUAD_MULAW:
    case AL_FORMAT_REAR_MULAW:
    case AL_FORMAT_51CHN_MULAW:
    case AL_FORMAT_61CHN_MULAW:
    case AL_FORMAT_71CHN_MULAW:
        return 1;

    case AL_FORMAT_MONO_IMA4:
    case AL_FORMAT_STEREO_IMA4:
        return 65;
    }
    fprintf(stderr, "Alure lib: Unhandled format: %#x\n", format);
    return 0;
}

ALenum GetSampleFormat(ALuint channels, ALuint bits, bool isFloat)
{
#define CHECK_FMT_RET(f) do {                                                 \
    ALenum fmt = alGetEnumValue(#f);                                          \
    if(alGetError() == AL_NO_ERROR && fmt != 0 && fmt != -1)                  \
        return fmt;                                                           \
} while(0)
    if(!isFloat)
    {
        if(bits == 8)
        {
            if(channels == 1) CHECK_FMT_RET(AL_FORMAT_MONO8);
            if(channels == 2) CHECK_FMT_RET(AL_FORMAT_STEREO8);
            if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                if(channels == 4) CHECK_FMT_RET(AL_FORMAT_QUAD8);
                if(channels == 6) CHECK_FMT_RET(AL_FORMAT_51CHN8);
                if(channels == 7) CHECK_FMT_RET(AL_FORMAT_61CHN8);
                if(channels == 8) CHECK_FMT_RET(AL_FORMAT_71CHN8);
            }
            if(alIsExtensionPresent("AL_LOKI_quadriphonic"))
            {
                if(channels == 4) CHECK_FMT_RET(AL_FORMAT_QUAD8_LOKI);
            }
            SetError("Unsupported 8-bit channel count\n");
            return AL_NONE;
        }
        if(bits == 16)
        {
            if(channels == 1) CHECK_FMT_RET(AL_FORMAT_MONO16);
            if(channels == 2) CHECK_FMT_RET(AL_FORMAT_STEREO16);
            if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                if(channels == 4) CHECK_FMT_RET(AL_FORMAT_QUAD16);
                if(channels == 6) CHECK_FMT_RET(AL_FORMAT_51CHN16);
                if(channels == 7) CHECK_FMT_RET(AL_FORMAT_61CHN16);
                if(channels == 8) CHECK_FMT_RET(AL_FORMAT_71CHN16);
            }
            if(alIsExtensionPresent("AL_LOKI_quadriphonic"))
            {
                if(channels == 4) CHECK_FMT_RET(AL_FORMAT_QUAD16_LOKI);
            }
            SetError("Unsupported 16-bit channel count\n");
            return AL_NONE;
        }
        SetError("Unsupported PCM bit depth\n");
        return AL_NONE;
    }

    if(bits == 32 && alIsExtensionPresent("AL_EXT_FLOAT32"))
    {
        if(channels == 1) CHECK_FMT_RET(AL_FORMAT_MONO_FLOAT32);
        if(channels == 2) CHECK_FMT_RET(AL_FORMAT_STEREO_FLOAT32);
        if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
        {
            if(channels == 4) CHECK_FMT_RET(AL_FORMAT_QUAD32);
            if(channels == 6) CHECK_FMT_RET(AL_FORMAT_51CHN32);
            if(channels == 7) CHECK_FMT_RET(AL_FORMAT_61CHN32);
            if(channels == 8) CHECK_FMT_RET(AL_FORMAT_71CHN32);
        }
        SetError("Unsupported float32 channel count\n");
        return AL_NONE;
    }
    if(bits == 64 && alIsExtensionPresent("AL_EXT_DOUBLE"))
    {
        if(channels == 1) CHECK_FMT_RET(AL_FORMAT_MONO_DOUBLE_EXT);
        if(channels == 2) CHECK_FMT_RET(AL_FORMAT_STEREO_DOUBLE_EXT);
        SetError("Unsupported double channel count\n");
        return AL_NONE;
    }
#undef CHECK_FMT_RET

    SetError("Unsupported float bit depth\n");
    return AL_NONE;
}

extern "C" {

/* Function: alureGetVersion
 *
 * Stores the major and minor version of the library. If either major or minor
 * are NULL, that value is not provided.
 */
ALURE_API void ALURE_APIENTRY alureGetVersion(ALuint *major, ALuint *minor)
{
    if(major) *major = ALURE_VER_MAJOR;
    if(minor) *minor = ALURE_VER_MINOR;
}

/* Function: alureGetErrorString
 *
 * Returns a string describing the last error encountered.
 */
ALURE_API const ALchar* ALURE_APIENTRY alureGetErrorString(void)
{
    const ALchar *ret = last_error;
    last_error = "No error";
    return ret;
}


/* Function: alureGetDeviceNames
 *
 * Gets an array of device name strings from OpenAL. This encapsulates
 * AL_ENUMERATE_ALL_EXT (if supported and 'all' is true) and standard
 * enumeration, with 'count' being set to the number of returned device
 * names.
 *
 * Returns:
 * An array of device name strings, or NULL on error.
 *
 * See Also:
 * <alureFreeDeviceNames>
 */
ALURE_API const ALCchar** ALURE_APIENTRY alureGetDeviceNames(ALCboolean all, ALCsizei *count)
{
    const ALCchar *list = NULL;
    if(all && alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
        list = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
    else
        list = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    if(!list)
    {
        alcGetError(NULL);
        SetError("No device names found");
        return NULL;
    }

    const ALCchar *cur = list;
    ALuint retlistLen = 0;
    while(*cur)
    {
        cur += strlen(cur)+1;
        retlistLen++;
    }

    const ALCchar **retlist = new const ALCchar*[retlistLen+1];
    retlistLen = 0;
    cur = list;
    while(*cur)
    {
        ALCuint len = strlen(cur)+1;
        ALCchar *newstr = new ALCchar[len];

        memcpy(newstr, cur, len);
        cur += len;

        retlist[retlistLen] = newstr;
        retlistLen++;
    }
    retlist[retlistLen] = NULL;

    *count = retlistLen;
    return retlist;
}

/* Function: alureFreeDeviceNames
 *
 * Frees the device name array returned from alureGetDeviceNames.
 *
 * See Also:
 * <alureGetDeviceNames>
 */
ALURE_API ALvoid ALURE_APIENTRY alureFreeDeviceNames(const ALCchar **names)
{
    if(names)
    {
        for(ALCuint i = 0;names[i];i++)
            delete[] const_cast<ALCchar*>(names[i]);
        delete[] names;
    }
}


/* Function: alureInitDevice
 *
 * Opens the named device, creates a context with the given attributes, and
 * sets that context as current. The name and attribute list would be the same
 * as what's passed to alcOpenDevice and alcCreateContext respectively.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureShutdownDevice>
 */
ALURE_API ALboolean ALURE_APIENTRY alureInitDevice(const ALCchar *name, const ALCint *attribs)
{
    ALCdevice *device = alcOpenDevice(name);
    if(!device)
    {
        alcGetError(NULL);

        SetError("Device open failed");
        return AL_FALSE;
    }

    ALCcontext *context = alcCreateContext(device, attribs);
    if(!context || alcMakeContextCurrent(context) == ALC_FALSE)
    {
        if(context)
            alcDestroyContext(context);
        alcCloseDevice(device);

        SetError("Context setup failed");
        return AL_FALSE;
    }
    alcGetError(device);

    return AL_TRUE;
}

/* Function: alureShutdownDevice
 *
 * Destroys the current context and closes its associated device.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureInitDevice>
 */
ALURE_API ALboolean ALURE_APIENTRY alureShutdownDevice(void)
{
    ALCcontext *context = alcGetCurrentContext();
    ALCdevice *device = alcGetContextsDevice(context);
    if(!context || !device)
    {
        alcGetError(device);
        SetError("Failed to get current device");
        return AL_FALSE;
    }

    if(alcMakeContextCurrent(NULL) == ALC_FALSE)
    {
        alcGetError(NULL);
        SetError("Failed to unset current context");
        return AL_FALSE;
    }

    alcDestroyContext(context);
    alcCloseDevice(device);
    alcGetError(NULL);

    return AL_TRUE;
}


/* Function: alureGetSampleFormat
 *
 * Retrieves an OpenAL format for the given sample format. If bits is non-0,
 * floatbits must be 0, and if floatbits is non-0, bits must be 0. The
 * application should not rely on any particular format enum being returned as
 * it is dependant on the available extensions. The returned format will be
 * valid for the current context. Requires an active context.
 *
 * Returns:
 * An OpenAL format enum for the given sample format, or AL_NONE if one can't
 * be found.
 */
ALURE_API ALenum ALURE_APIENTRY alureGetSampleFormat(ALuint channels, ALuint bits, ALuint floatbits)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_NONE;
    }

    if(bits && floatbits)
    {
        SetError("Both bit-types specified");
        return AL_NONE;
    }

    if(bits)
        return GetSampleFormat(channels, bits, false);
    return GetSampleFormat(channels, floatbits, true);
}


/* Function: alureInstallDecodeCallbacks
 *
 * Installs callbacks to enable ALURE to handle more file types. The index is
 * the order that each given set of callbacks will be tried, starting at the
 * most negative number (INT_MIN) and going up. Negative indices will be tried
 * before the built-in decoders, and positive indices will be tried after.
 * Installing callbacks onto the same index multiple times will remove the
 * previous callbacks, and removing old callbacks won't affect any opened files
 * using them (they'll continue to use the old functions until properly closed,
 * although newly opened files will use the new ones). Passing NULL for all
 * callbacks is a valid way to remove an installed set, otherwise certain
 * callbacks must be specified. Callbacks that are not specified will assume
 * failure.
 *
 * Parameters:
 * open_file - This callback is expected to open the named file and prepare it
 *             for decoding. If the callbacks cannot decode the file, NULL
 *             should be returned to indicate failure. Upon success, a non-NULL
 *             handle must be returned, which will be used as a unique
 *             identifier for the decoder instance. This callback is required
 *             if open_memory is not specified.
 * open_memory - This callback behaves the same as open_file, except it takes a
 *               memory segment for input instead of a filename. The given
 *               memory will remain valid while the instance is open. This
 *               callback is required if open_file is not specified.
 * get_format - This callback is used to retrieve the format of the decoded
 *              data for the given instance. It is the responsibility of the
 *              function to make sure the returned format is valid for the
 *              current AL context (eg. don't return AL_FORMAT_QUAD16 if the
 *              AL_EXT_MCFORMATS extension isn't available). Returning 0 for
 *              samplerate or blocksize, or returning AL_NONE for format, will
 *              cause a failure. Returning AL_FALSE indicates failure. This
 *              callback is required.
 * decode - This callback is called to get more decoded data. Up to the
 *          specified amount of bytes should be written to the data pointer.
 *          The number of bytes written should be a multiple of the block size,
 *          otherwise an OpenAL error may occur during buffering. The function
 *          should return the number of bytes written. This callback is
 *          required.
 * rewind - This callback is for rewinding the instance so that the next decode
 *          calls for it will get audio data from the start of the sound file.
 *          If the stream fails to rewind, AL_FALSE should be returned.
 * close - This callback is called at the end of processing for a particular
 *         instance. The handle will not be used further and any associated
 *         data may be deleted.
 *
 * Returns:
 * AL_FALSE on error.
 */
ALURE_API ALboolean ALURE_APIENTRY alureInstallDecodeCallbacks(ALint index,
      void*     (*open_file)(const ALchar *filename),
      void*     (*open_memory)(const ALubyte *data, ALuint length),
      ALboolean (*get_format)(void *instance, ALenum *format, ALuint *samplerate, ALuint *blocksize),
      ALuint    (*decode)(void *instance, ALubyte *data, ALuint bytes),
      ALboolean (*rewind)(void *instance),
      void      (*close)(void *instance))
{
    if(!open_file && !open_memory && !get_format && !decode && !rewind && !close)
    {
        std::map<ALint,UserCallbacks>::iterator i = InstalledCallbacks.find(index);
        if(i != InstalledCallbacks.end())
            InstalledCallbacks.erase(i);
        return AL_TRUE;
    }

    if((!open_file && !open_memory) || !get_format || !decode)
    {
        SetError("Missing callback functions");
        return AL_FALSE;
    }

    UserCallbacks newcb;
    newcb.open_file = open_file;
    newcb.open_mem  = open_memory;
    newcb.get_fmt   = get_format;
    newcb.decode    = decode;
    newcb.rewind    = rewind;
    newcb.close     = close;

    InstalledCallbacks[index] = newcb;

    return AL_TRUE;
}


/* Function: alureSleep
 *
 * Rests the calling thread for the given number of seconds.
 *
 * Returns:
 * AL_FALSE on error.
 */
ALURE_API ALboolean ALURE_APIENTRY alureSleep(ALfloat duration)
{
    if(duration < 0.0f)
    {
        SetError("Invalid duration");
        return AL_FALSE;
    }

    ALuint seconds = (ALuint)duration;
    ALdouble rest = duration - (ALdouble)seconds;

#ifdef HAVE_NANOSLEEP

    struct timespec t, remainingTime;
    t.tv_sec = (time_t)seconds;
    t.tv_nsec = (long)(rest*1000000000);

    while(nanosleep(&t, &remainingTime) < 0 && errno == EINTR)
        t = remainingTime;

#elif defined(HAVE_WINDOWS_H)

    while(seconds > 0)
    {
        Sleep(1000);
        seconds--;
    }
    Sleep((DWORD)(rest * 1000));

#endif

    return AL_TRUE;
}


/* Function: alureGetProcAddress
 *
 * Returns a pointer for the named ALURE function.
 *
 * Returns:
 * NULL on error.
 *
 * *Version Added*: 1.1
 */
ALURE_API void* ALURE_APIENTRY alureGetProcAddress(const ALchar *funcname)
{
    static const struct {
        const char *name;
        void *func;
    } FunctionList[] = {
#define ADD_FUNCTION(x) { #x, (void*)x },
        ADD_FUNCTION(alureGetVersion)
        ADD_FUNCTION(alureGetErrorString)
        ADD_FUNCTION(alureGetDeviceNames)
        ADD_FUNCTION(alureFreeDeviceNames)
        ADD_FUNCTION(alureInitDevice)
        ADD_FUNCTION(alureShutdownDevice)
        ADD_FUNCTION(alureGetSampleFormat)
        ADD_FUNCTION(alureSleep)
        ADD_FUNCTION(alureCreateBufferFromFile)
        ADD_FUNCTION(alureCreateBufferFromMemory)
        ADD_FUNCTION(alureBufferDataFromFile)
        ADD_FUNCTION(alureBufferDataFromMemory)
        ADD_FUNCTION(alureCreateStreamFromFile)
        ADD_FUNCTION(alureCreateStreamFromMemory)
        ADD_FUNCTION(alureCreateStreamFromStaticMemory)
        ADD_FUNCTION(alureCreateStreamFromCallback)
        ADD_FUNCTION(alureGetStreamLength)
        ADD_FUNCTION(alureRewindStream)
        ADD_FUNCTION(alureDestroyStream)
        ADD_FUNCTION(alureSetStreamOrder)
        ADD_FUNCTION(alureSetStreamPatchset)
        ADD_FUNCTION(alureInstallDecodeCallbacks)
        ADD_FUNCTION(alureSetIOCallbacks)
        ADD_FUNCTION(alureGetProcAddress)
        ADD_FUNCTION(alurePlaySourceStream)
        ADD_FUNCTION(alurePlaySource)
        ADD_FUNCTION(alureStopSource)
#undef ADD_FUNCTION
        { NULL, NULL }
    };

    size_t i;
    for(i = 0;FunctionList[i].name;i++)
    {
        if(strcmp(FunctionList[i].name, funcname) == 0)
            break;
    }

    if(!FunctionList[i].name)
        SetError("Function not found");
    return FunctionList[i].func;
}

} // extern "C"
