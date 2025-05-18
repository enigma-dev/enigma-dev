/*
 * ALURE  OpenAL utility library
 * Copyright (c) 2009-2010 by Chris Robinson.
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

#include "config.h"

#include "main.h"

#include <string.h>
#include <assert.h>

#include <istream>

#include <dumb.h>


struct dumbStream : public alureStream {
private:
    DUMBFILE_SYSTEM vfs;
    DUMBFILE *dumbFile;
    DUH *duh;
    DUH_SIGRENDERER *renderer;
    std::vector<sample_t> sampleBuf;
    ALuint lastOrder;
    ALenum format;
    ALCint samplerate;

public:
    static void Init() { }
    static void Deinit() { }

    virtual bool IsValid()
    { return renderer != NULL; }

    virtual bool GetFormat(ALenum *fmt, ALuint *frequency, ALuint *blockalign)
    {
        if(format == AL_NONE)
        {
            format = GetSampleFormat(2, 32, true);
            if(format == AL_NONE)
                format = AL_FORMAT_STEREO16;
        }
        *fmt = format;
        *frequency = samplerate;
        *blockalign = 2 * ((format==AL_FORMAT_STEREO16) ? sizeof(ALshort) :
                                                          sizeof(ALfloat));
        return true;
    }

    virtual ALuint GetData(ALubyte *data, ALuint bytes)
    {
        ALuint ret = 0;

        if(dumb_it_sr_get_speed(duh_get_it_sigrenderer(renderer)) == 0)
            return 0;

        ALuint sample_count = bytes / ((format==AL_FORMAT_STEREO16) ?
                                       sizeof(ALshort) : sizeof(ALfloat));

        sampleBuf.resize(sample_count);
        sample_t *samples[] = {
            &sampleBuf[0]
        };

        dumb_silence(samples[0], sample_count);
        ret = duh_sigrenderer_generate_samples(renderer, 1.0f, 65536.0f/samplerate, sample_count/2, samples);
        ret *= 2;
        if(format == AL_FORMAT_STEREO16)
        {
            for(ALuint i = 0;i < ret;i++)
                ((ALshort*)data)[i] = clamp(samples[0][i]>>8, -32768, 32767);
        }
        else
        {
            for(ALuint i = 0;i < ret;i++)
                ((ALfloat*)data)[i] = samples[0][i] * (1.0/8388607.0);
        }
        ret *= ((format==AL_FORMAT_STEREO16) ? sizeof(ALshort) : sizeof(ALfloat));

        return ret;
    }

    virtual bool Rewind()
    {
        DUH_SIGRENDERER *newrenderer = dumb_it_start_at_order(duh, 2, lastOrder);
        if(!newrenderer)
        {
            SetError("Could not start renderer");
            return false;
        }
        duh_end_sigrenderer(renderer);
        renderer = newrenderer;
        return true;
    }

    virtual bool SetOrder(ALuint order)
    {
        DUH_SIGRENDERER *newrenderer = dumb_it_start_at_order(duh, 2, order);
        if(!newrenderer)
        {
            SetError("Could not set order");
            return false;
        }
        duh_end_sigrenderer(renderer);
        renderer = newrenderer;

        lastOrder = order;
        return true;
    }

    dumbStream(std::istream *_fstream)
      : alureStream(_fstream), dumbFile(NULL), duh(NULL), renderer(NULL),
        lastOrder(0), format(AL_NONE), samplerate(48000)
    {
        ALCdevice *device = alcGetContextsDevice(alcGetCurrentContext());
        if(device) alcGetIntegerv(device, ALC_FREQUENCY, 1, &samplerate);

        DUH* (*funcs[])(DUMBFILE*) = {
            dumb_read_it,
            dumb_read_xm,
            dumb_read_s3m,
            //dumb_read_mod,
            NULL
        };

        vfs.open = NULL;
        vfs.skip = skip;
        vfs.getc = read_char;
        vfs.getnc = read;
        vfs.close = NULL;

        for(size_t i = 0;funcs[i];i++)
        {
            dumbFile = dumbfile_open_ex(this, &vfs);
            if(dumbFile)
            {
                duh = funcs[i](dumbFile);
                if(duh)
                {
                    renderer = dumb_it_start_at_order(duh, 2, lastOrder);
                    if(renderer)
                    {
                        dumb_it_set_loop_callback(duh_get_it_sigrenderer(renderer), loop_cb, this);
                        break;
                    }

                    unload_duh(duh);
                    duh = NULL;
                }

                dumbfile_close(dumbFile);
                dumbFile = NULL;
            }
            fstream->clear();
            fstream->seekg(0);
        }
    }

    virtual ~dumbStream()
    {
        if(renderer)
            duh_end_sigrenderer(renderer);
        renderer = NULL;

        if(duh)
            unload_duh(duh);
        duh = NULL;

        if(dumbFile)
            dumbfile_close(dumbFile);
        dumbFile = NULL;
    }

private:
    // DUMBFILE iostream callbacks
    static int skip(void *user_data, long offset)
    {
        std::istream *stream = static_cast<dumbStream*>(user_data)->fstream;
        stream->clear();

        if(stream->seekg(offset, std::ios_base::cur))
            return 0;
        return -1;
    }

    static long read(char *ptr, long size, void *user_data)
    {
        std::istream *stream = static_cast<dumbStream*>(user_data)->fstream;
        stream->clear();

        stream->read(ptr, size);
        return stream->gcount();
    }

    static int read_char(void *user_data)
    {
        std::istream *stream = static_cast<dumbStream*>(user_data)->fstream;
        stream->clear();

        unsigned char ret;
        stream->read(reinterpret_cast<char*>(&ret), 1);
        if(stream->gcount() > 0)
            return ret;
        return -1;
    }

    static int loop_cb(void *user_data)
    {
        dumbStream *self = static_cast<dumbStream*>(user_data);
        dumb_it_sr_set_speed(duh_get_it_sigrenderer(self->renderer), 0);
        return 0;
    }
};
// Priority = -1, because mod loading can find false-positives
static DecoderDecl<dumbStream,-1> dumbStream_decoder;
