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

#include <sndfile.h>


struct sndStream : public alureStream {
private:
    SNDFILE *sndFile;
    SF_INFO sndInfo;
    ALenum format;

public:
    static void Init() { }
    static void Deinit() { }

    virtual bool IsValid()
    { return sndFile != NULL; }

    virtual bool GetFormat(ALenum *fmt, ALuint *frequency, ALuint *blockalign)
    {
        if(format == AL_NONE)
            format = GetSampleFormat(sndInfo.channels, 16, false);
        *fmt = format;
        *frequency = sndInfo.samplerate;
        *blockalign = sndInfo.channels*2;
        return true;
    }

    virtual ALuint GetData(ALubyte *data, ALuint bytes)
    {
        const ALuint frameSize = 2*sndInfo.channels;
        return sf_readf_short(sndFile, (short*)data, bytes/frameSize) * frameSize;
    }

    virtual bool Rewind()
    {
        if(sf_seek(sndFile, 0, SEEK_SET) != -1)
            return true;

        SetError("Seek failed");
        return false;
    }

    virtual alureInt64 GetLength()
    {
        if(sndInfo.frames == -1)
            return 0;
        return sndInfo.frames;
    }

    sndStream(std::istream *_fstream)
      : alureStream(_fstream), sndFile(NULL), format(AL_NONE)
    {
        memset(&sndInfo, 0, sizeof(sndInfo));

        static SF_VIRTUAL_IO streamIO = {
            get_filelen, seek,
            read, write, tell
        };
        sndFile = sf_open_virtual(&streamIO, SFM_READ, &sndInfo, this);
    }

    virtual ~sndStream()
    {
        if(sndFile)
            sf_close(sndFile);
        sndFile = NULL;
    }

private:
    // libSndFile iostream callbacks
    static sf_count_t get_filelen(void *user_data)
    {
        std::istream *stream = static_cast<sndStream*>(user_data)->fstream;
        stream->clear();

        std::streampos len = -1;
        std::streampos pos = stream->tellg();
        if(stream->seekg(0, std::ios_base::end))
        {
            len = stream->tellg();
            stream->seekg(pos);
        }

        return len;
    }

    static sf_count_t seek(sf_count_t offset, int whence, void *user_data)
    {
        std::istream *stream = static_cast<sndStream*>(user_data)->fstream;
        stream->clear();

        if((whence == SEEK_CUR && stream->seekg(offset, std::ios_base::cur)) ||
           (whence == SEEK_SET && stream->seekg(offset, std::ios_base::beg)) ||
           (whence == SEEK_END && stream->seekg(offset, std::ios_base::end)))
            return stream->tellg();

        return -1;
    }

    static sf_count_t read(void *ptr, sf_count_t count, void *user_data)
    {
        std::istream *stream = static_cast<sndStream*>(user_data)->fstream;
        stream->clear();
        stream->read(static_cast<char*>(ptr), count);
        return stream->gcount();
    }

    static sf_count_t write(const void*, sf_count_t, void*)
    { return -1; }

    static sf_count_t tell(void *user_data)
    {
        std::istream *stream = static_cast<sndStream*>(user_data)->fstream;
        stream->clear();
        return stream->tellg();
    }
};
static DecoderDecl<sndStream,0> sndStream_decoder;
