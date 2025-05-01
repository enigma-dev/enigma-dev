#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "AL/alure.h"

#ifdef __linux__
/* Linux implementation for reading CD digital audio */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>


typedef struct {
    int fd;
    int start;
    int current;
    int end;
} CDDAData;

static const char *cd_device = "/dev/cdrom1";

static void *cdda_open_file(const char *fname)
{
    struct cdrom_tochdr cdtochdr;
    struct cdrom_tocentry cdtocentry;
    int startaddr=-1, endaddr=-1, idx;
    CDDAData *dat;
    int fd, ret;

    /* Make sure the filename has the appropriate URI and extract the track
     * number */
    if(strncmp(fname, "cdda://", 7) != 0)
        return NULL;
    idx = atoi(fname+7);

    /* Open the block device and get the TOC header */
    fd = open(cd_device, O_RDONLY | O_NONBLOCK);
    if(fd == -1) return NULL;

    ret = ioctl(fd, CDROMREADTOCHDR, &cdtochdr);
    if(ret != -1)
    {
        if(idx < cdtochdr.cdth_trk0 || idx >= cdtochdr.cdth_trk1)
            ret = -1;
    }
    if(ret != -1)
    {
        /* Get the start address for the requested track, and the start address
         * of the next track as the end point */
        cdtocentry.cdte_format = CDROM_LBA;
        cdtocentry.cdte_track = idx;
        ret = ioctl(fd, CDROMREADTOCENTRY, &cdtocentry);
        if(ret != -1 && !(cdtocentry.cdte_ctrl&CDROM_DATA_TRACK))
            startaddr = cdtocentry.cdte_addr.lba;
    }
    if(ret != -1)
    {
        cdtocentry.cdte_format = CDROM_LBA;
        cdtocentry.cdte_track = ++idx;
        ret = ioctl(fd, CDROMREADTOCENTRY, &cdtocentry);
        if(ret != -1)
            endaddr = cdtocentry.cdte_addr.lba;
    }

    if(ret == -1 || startaddr == -1 || endaddr == -1)
    {
        close(fd);
        return NULL;
    }

    dat = malloc(sizeof(*dat));
    dat->fd = fd;
    dat->start = startaddr;
    dat->current = startaddr;
    dat->end = endaddr;

    return dat;
}

static ALboolean cdda_get_format(void *instance, ALenum *format, ALuint *samplerate, ALuint *blocksize)
{
    /* These values are specified by the red-book audio standard and do not
     * change */
    *format = AL_FORMAT_STEREO16;
    *samplerate = 44100;
    *blocksize = CD_FRAMESIZE_RAW;

    return AL_TRUE;
    (void)instance;
}

static ALuint cdda_decode(void *instance, ALubyte *data, ALuint bytes)
{
    CDDAData *self = instance;

    ALuint got = 0;
    while(bytes-got >= CD_FRAMESIZE_RAW && self->current < self->end)
    {
        struct cdrom_read_audio cdra;

        /* Read as many frames that are left that will fit */
        cdra.addr.lba = self->current;
        cdra.addr_format = CDROM_LBA;
        cdra.nframes = (bytes-got) / CD_FRAMESIZE_RAW;
        cdra.buf = data;

        if(cdra.nframes > self->end-self->current)
            cdra.nframes = self->end-self->current;

        if(ioctl(self->fd, CDROMREADAUDIO, &cdra) == -1)
        {
            cdra.nframes = 1;
            memset(cdra.buf, 0, CD_FRAMESIZE_RAW);
        }

        self->current += cdra.nframes;
        data += CD_FRAMESIZE_RAW*cdra.nframes;
        got += CD_FRAMESIZE_RAW*cdra.nframes;
    }

    return got;
}

static ALboolean cdda_rewind(void *instance)
{
    CDDAData *self = instance;

    self->current = self->start;
    return AL_TRUE;
}

static void cdda_close(void *instance)
{
    CDDAData *self = instance;

    close(self->fd);
    free(self);
}

#elif defined(HAVE_DDK_NTDDCDRM_H)
/* Windows implementation for reading CD digital audio */
#include <windows.h>
#include <ddk/ntddcdrm.h>

/* Defined by red-book standard; do not change! */
#define CD_FRAMESIZE_RAW  (2352)

#define CDFRAMES_PERSEC  (75)
#define CDFRAMES_PERMIN  (CDFRAMES_PERSEC * 60)
#define FRAME_OF_ADDR(a) ((a)[1] * CDFRAMES_PERMIN + (a)[2] * CDFRAMES_PERSEC + (a)[3])
#define FRAME_OF_TOC(toc, idx)  FRAME_OF_ADDR((toc).TrackData[idx - (toc).FirstTrack].Address)

/* All of this is pretty similar to the Linux version, except using the WinAPI
 * device functions instead of POSIX */
typedef struct {
    HANDLE fd;
    DWORD start;
    DWORD current;
    DWORD end;
} CDDAData;

static const char *cd_device = "D";

static void *cdda_open_file(const char *fname)
{
    /* Device filename is of the format "\\.\D:", where "D" is the letter of
     * the CD drive */
    const char cd_drv[] = { '\\','\\','.','\\',*cd_device,':', 0 };
    DWORD br, idx;
    CDROM_TOC toc;
    CDDAData *dat;
    HANDLE fd;
    int ret;

    if(strncmp(fname, "cdda://", 7) != 0)
        return NULL;
    idx = atoi(fname+7);

    fd = CreateFileA(cd_drv, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    if(fd == (HANDLE)-1) return NULL;

    ret = DeviceIoControl(fd, IOCTL_CDROM_READ_TOC, NULL, 0,
                          &toc, sizeof(toc), &br, NULL);
    if(ret == 0 || idx < toc.FirstTrack || idx > toc.LastTrack ||
       (toc.TrackData[idx-toc.FirstTrack].Control&4))
    {
        CloseHandle(fd);
        return NULL;
    }

    dat = malloc(sizeof(*dat));
    dat->fd = fd;
    dat->start = FRAME_OF_TOC(toc, idx) - FRAME_OF_TOC(toc, toc.FirstTrack);
    dat->current = dat->start;
    dat->end = FRAME_OF_TOC(toc, idx+1) - FRAME_OF_TOC(toc, toc.FirstTrack);

    return dat;
}

static ALboolean cdda_get_format(void *instance, ALenum *format, ALuint *samplerate, ALuint *blocksize)
{
    *format = AL_FORMAT_STEREO16;
    *samplerate = 44100;
    *blocksize = CD_FRAMESIZE_RAW;

    return AL_TRUE;
    (void)instance;
}

static ALuint cdda_decode(void *instance, ALubyte *data, ALuint bytes)
{
    CDDAData *self = instance;

    ALuint got = 0;
    while(bytes-got >= CD_FRAMESIZE_RAW && self->current < self->end)
    {
        RAW_READ_INFO rdInfo;
        DWORD br;

        rdInfo.DiskOffset.QuadPart = (LONGLONG)self->current<<11;
        rdInfo.SectorCount = (bytes-got) / CD_FRAMESIZE_RAW;
        rdInfo.TrackMode = CDDA;

        if(rdInfo.SectorCount > self->end-self->current)
            rdInfo.SectorCount = self->end-self->current;

        if(!DeviceIoControl(self->fd, IOCTL_CDROM_RAW_READ,
                            &rdInfo, sizeof(rdInfo), data, bytes-got,
                            &br, NULL))
        {
            rdInfo.SectorCount = 1;
            memset(data, 0, CD_FRAMESIZE_RAW);
        }

        self->current += rdInfo.SectorCount;
        data += CD_FRAMESIZE_RAW*rdInfo.SectorCount;
        got += CD_FRAMESIZE_RAW*rdInfo.SectorCount;
    }

    return got;
}

static ALboolean cdda_rewind(void *instance)
{
    CDDAData *self = instance;

    self->current = self->start;
    return AL_TRUE;
}

static void cdda_close(void *instance)
{
    CDDAData *self = instance;

    CloseHandle(self->fd);
    free(self);
}

#else

static const char *cd_device = "(unknown)";

static void *cdda_open_file(const char *fname)
{
    if(strncmp(fname, "cdda://", 7) == 0)
        fprintf(stderr, "CD Digital Audio was not compiled for this system\n");
    return NULL;
}

static ALboolean cdda_get_format(void *instance, ALenum *format, ALuint *samplerate, ALuint *blocksize)
{
    return AL_FALSE;
    (void)instance;
}

static ALuint cdda_decode(void *instance, ALubyte *data, ALuint bytes)
{
    return 0;
    (void)instance;
    (void)data;
    (void)bytes;
}

static ALboolean cdda_rewind(void *instance)
{
    return AL_FALSE;
    (void)instance;
}

static void cdda_close(void *instance)
{
    (void)instance;
}

#endif


static void eos_callback(void *param, ALuint unused)
{
    *(volatile int*)param = 1;
    (void)unused;
}

#define NUM_BUFS 3


int main(int argc, char **argv)
{
    volatile int isdone;
    alureStream *stream;
    const char *fname;
    ALuint src;

    if(argc < 2 || (strcmp(argv[1], "-cd-device") == 0 && argc < 4))
    {
        fprintf(stderr, "Usage %s [-cd-device <device>] cdda://<tracknum>\n", argv[0]);
        fprintf(stderr, "Default CD device is %s\n", cd_device);
        return 1;
    }

    if(strcmp(argv[1], "-cd-device") != 0)
        fname = argv[1];
    else
    {
        cd_device = argv[2];
        fname = argv[3];
    }

    alureInstallDecodeCallbacks(-1, cdda_open_file, NULL, cdda_get_format,
                                cdda_decode, cdda_rewind, cdda_close);

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

    alureStreamSizeIsMicroSec(AL_TRUE);

    stream = alureCreateStreamFromFile(fname, 250000, 0, NULL);
    if(!stream)
    {
        fprintf(stderr, "Could not load %s: %s\n", fname, alureGetErrorString());
        alDeleteSources(1, &src);

        alureShutdownDevice();
        return 1;
    }

    isdone = 0;
    if(!alurePlaySourceStream(src, stream, NUM_BUFS, 0, eos_callback, (void*)&isdone))
    {
        fprintf(stderr, "Failed to play stream: %s\n", alureGetErrorString());
        isdone = 1;
    }

    while(!isdone)
    {
        alureSleep(0.125);
        alureUpdate();
    }
    alureStopSource(src, AL_FALSE);

    alDeleteSources(1, &src);
    alureDestroyStream(stream, 0, NULL);

    alureShutdownDevice();
    return 0;
}
