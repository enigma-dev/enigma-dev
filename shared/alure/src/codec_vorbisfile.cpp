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

#ifdef HAS_VORBISIDEC
#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>
#else
#include <vorbis/vorbisfile.h>
#endif


struct oggStream : public alureStream {
private:
    OggVorbis_File oggFile;
    vorbis_info *oggInfo;
    int oggBitstream;
    ALenum format;

public:
    static void Init() { }
    static void Deinit() { }

    virtual bool IsValid()
    { return oggInfo != NULL; }

    virtual bool GetFormat(ALenum *fmt, ALuint *frequency, ALuint *blockalign)
    {
        if(format == AL_NONE)
            format = GetSampleFormat(oggInfo->channels, 16, false);
        *fmt = format;
        *frequency = oggInfo->rate;
        *blockalign = oggInfo->channels*2;
        return true;
    }

    virtual ALuint GetData(ALubyte *data, ALuint bytes)
    {
        ALuint got = 0;
        while(bytes > 0)
        {
#ifdef HAS_VORBISIDEC
            int res = ov_read(&oggFile, (char*)&data[got], bytes, &oggBitstream);
#else
            int res = ov_read(&oggFile, (char*)&data[got], bytes, BigEndian?1:0, 2, 1, &oggBitstream);
#endif
            if(res <= 0)
                break;
            bytes -= res;
            got += res;
        }
        // 1, 2, and 4 channel files decode into the same channel order as
        // OpenAL, however 6(5.1), 7(6.1), and 8(7.1) channel files need to be
        // re-ordered
        if(oggInfo->channels == 6)
        {
            ALshort *samples = (ALshort*)data;
            for(ALuint i = 0;i < got/sizeof(ALshort);i+=6)
            {
                // OpenAL : FL, FR, FC, LFE, RL, RR
                // Vorbis : FL, FC, FR,  RL, RR, LFE
                swap(samples[i+1], samples[i+2]);
                swap(samples[i+3], samples[i+5]);
                swap(samples[i+4], samples[i+5]);
            }
        }
        else if(oggInfo->channels == 7)
        {
            ALshort *samples = (ALshort*)data;
            for(ALuint i = 0;i < got/sizeof(ALshort);i+=7)
            {
                // OpenAL : FL, FR, FC, LFE, RC, SL, SR
                // Vorbis : FL, FC, FR,  SL, SR, RC, LFE
                swap(samples[i+1], samples[i+2]);
                swap(samples[i+3], samples[i+6]);
                swap(samples[i+4], samples[i+5]);
                swap(samples[i+5], samples[i+6]);
            }
        }
        else if(oggInfo->channels == 8)
        {
            ALshort *samples = (ALshort*)data;
            for(ALuint i = 0;i < got/sizeof(ALshort);i+=8)
            {
                // OpenAL : FL, FR, FC, LFE, RL, RR, SL, SR
                // Vorbis : FL, FC, FR,  SL, SR, RL, RR, LFE
                swap(samples[i+1], samples[i+2]);
                swap(samples[i+3], samples[i+7]);
                swap(samples[i+4], samples[i+5]);
                swap(samples[i+5], samples[i+6]);
                swap(samples[i+6], samples[i+7]);
            }
        }
        return got;
    }

    virtual bool Rewind()
    {
        if(ov_pcm_seek(&oggFile, 0) == 0)
            return true;

        SetError("Seek failed");
        return false;
    }

    virtual alureInt64 GetLength()
    {
        ogg_int64_t len = ov_pcm_total(&oggFile, oggBitstream);
        if(len < 0) return 0;
        return len;
    }

    oggStream(std::istream *_fstream)
      : alureStream(_fstream), oggInfo(NULL), oggBitstream(0), format(AL_NONE)
    {
        const ov_callbacks streamIO = {
            read, seek, close, tell
        };

        if(ov_open_callbacks(this, &oggFile, NULL, 0, streamIO) == 0)
        {
            oggInfo = ov_info(&oggFile, -1);
            if(!oggInfo)
                ov_clear(&oggFile);
        }
    }

    virtual ~oggStream()
    {
        if(oggInfo)
            ov_clear(&oggFile);
        oggInfo = NULL;
    }

private:
    // libVorbisFile iostream callbacks
    static int seek(void *user_data, ogg_int64_t offset, int whence)
    {
        std::istream *stream = static_cast<oggStream*>(user_data)->fstream;
        stream->clear();

        if(whence == SEEK_CUR)
            stream->seekg(offset, std::ios_base::cur);
        else if(whence == SEEK_SET)
            stream->seekg(offset, std::ios_base::beg);
        else if(whence == SEEK_END)
            stream->seekg(offset, std::ios_base::end);
        else
            return -1;

        return stream->tellg();
    }

    static size_t read(void *ptr, size_t size, size_t nmemb, void *user_data)
    {
        std::istream *stream = static_cast<oggStream*>(user_data)->fstream;
        stream->clear();

        stream->read(static_cast<char*>(ptr), nmemb*size);
        size_t ret = stream->gcount();
        return ret/size;
    }

    static long tell(void *user_data)
    {
        std::istream *stream = static_cast<oggStream*>(user_data)->fstream;
        stream->clear();
        return stream->tellg();
    }

    static int close(void*)
    {
        return 0;
    }
};
// Priority = 2, so it's preferred over libsndfile and libFLAC
static DecoderDecl<oggStream,2> oggStream_decoder;
