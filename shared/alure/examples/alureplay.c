#include <stdio.h>

#include "AL/alure.h"

volatile int isdone = 0;
static void eos_callback(void *unused, ALuint unused2)
{
    isdone = 1;
    (void)unused;
    (void)unused2;
}

int main( int argc, char **argv)
{
    ALuint src, buf;

    if(argc < 2)
    {
        fprintf(stderr, "Usage %s <soundfile>\n", argv[0]);
        return 1;
    }

    if(!alureInitDevice(NULL, NULL))
    {
        fprintf(stderr, "Failed to open OpenAL device: %s\n", alureGetErrorString());
        return 1;
    }

    alGenSources(1, &src);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Failed to create OpenAL source!\n");
        alureShutdownDevice();
        return 1;
    }

    buf = alureCreateBufferFromFile(argv[1]);
    if(!buf)
    {
        fprintf(stderr, "Could not load %s: %s\n", argv[1], alureGetErrorString());
        alDeleteSources(1, &src);

        alureShutdownDevice();
        return 1;
    }

    alSourcei(src, AL_BUFFER, buf);
    if(alurePlaySource(src, eos_callback, NULL) == AL_FALSE)
    {
        fprintf(stderr, "Failed to start source!\n");
        alDeleteSources(1, &src);
        alDeleteBuffers(1, &buf);

        alureShutdownDevice();
        return 1;
    }

    while(!isdone)
    {
        alureSleep(0.125);
        alureUpdate();
    }

    alDeleteSources(1, &src);
    alDeleteBuffers(1, &buf);

    alureShutdownDevice();
    return 0;
}
