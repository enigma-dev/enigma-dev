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


struct aiffStream : public alureStream {
private:
    ALenum format;
    int samplerate;
    int blockAlign;
    int sampleSize;
    int channels;
    long dataStart;
    long dataLen;
    size_t remLen;

public:
    static void Init() { }
    static void Deinit() { }

    virtual bool IsValid()
    { return (dataStart > 0 && format != AL_NONE); }

    virtual bool GetFormat(ALenum *fmt, ALuint *frequency, ALuint *blockalign)
    {
        *fmt = format;
        *frequency = samplerate;
        *blockalign = blockAlign;
        return true;
    }

    virtual ALuint GetData(ALubyte *data, ALuint bytes)
    {
        std::streamsize rem = ((remLen >= bytes) ? bytes : remLen) / blockAlign;
        fstream->read(reinterpret_cast<char*>(data), rem*blockAlign);

        std::streamsize got = fstream->gcount();
        remLen -= got;
        got -= got%blockAlign;

        if(LittleEndian)
        {
            if(sampleSize == 2)
            {
                for(std::streamsize i = 0;i < got;i+=2)
                    swap(data[i], data[i+1]);
            }
            else if(sampleSize == 4)
            {
                for(std::streamsize i = 0;i < got;i+=4)
                {
                    swap(data[i+0], data[i+3]);
                    swap(data[i+1], data[i+2]);
                }
            }
        }

        return got;
    }

    virtual bool Rewind()
    {
        fstream->clear();
        if(fstream->seekg(dataStart))
        {
            remLen = dataLen;
            return true;
        }

        SetError("Seek failed");
        return false;
    }

    virtual alureInt64 GetLength()
    {
        alureInt64 ret = dataLen;
        return ret / channels * 8 / sampleSize;
    }

    aiffStream(std::istream *_fstream)
      : alureStream(_fstream), format(0), dataStart(0)
    {
        ALubyte buffer[25];
        int length;

        if(!fstream->read(reinterpret_cast<char*>(buffer), 12) ||
           memcmp(buffer, "FORM", 4) != 0 || memcmp(buffer+8, "AIFF", 4) != 0)
            return;

        while(!dataStart || format == AL_NONE)
        {
            char tag[4];
            if(!fstream->read(tag, 4))
                break;

            /* read chunk length */
            length = read_be32(fstream);

            if(memcmp(tag, "COMM", 4) == 0 && length >= 18)
            {
                /* mono or stereo data */
                channels = read_be16(fstream);

                /* number of sample frames */
                fstream->ignore(4);

                /* bits per sample */
                sampleSize = read_be16(fstream) / 8;

                /* sample frequency */
                samplerate = read_be80extended(fstream);

                /* block alignment */
                blockAlign = channels * sampleSize;

                format = GetSampleFormat(channels, sampleSize*8, false);

                length -= 18;
            }
            else if(memcmp(tag, "SSND", 4) == 0)
            {
                dataStart = fstream->tellg();
                dataStart += 8;
                dataLen = remLen = length - 8;
            }

            fstream->seekg(length, std::ios_base::cur);
        }

        if(dataStart > 0 && format != AL_NONE)
            fstream->seekg(dataStart);
    }

    virtual ~aiffStream()
    { }
};
// Priority = 9, prefer this decoder over external ones (but not the wave decoder)
static DecoderDecl<aiffStream,9> aiffStream_decoder;
