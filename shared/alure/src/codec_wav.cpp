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


struct wavStream : public alureStream {
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

        if(BigEndian && sampleSize == 16)
        {
            for(std::streamsize i = 0;i < got;i+=2)
                swap(data[i], data[i+1]);
        }
        else if(BigEndian && sampleSize == 32)
        {
            for(std::streamsize i = 0;i < got;i+=4)
            {
                swap(data[i+0], data[i+3]);
                swap(data[i+1], data[i+2]);
            }
        }
        else if(BigEndian && sampleSize == 64)
        {
            for(std::streamsize i = 0;i < got;i+=8)
            {
                swap(data[i+0], data[i+7]);
                swap(data[i+1], data[i+6]);
                swap(data[i+2], data[i+5]);
                swap(data[i+3], data[i+4]);
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

    wavStream(std::istream *_fstream)
      : alureStream(_fstream), format(0), dataStart(0)
    {
        ALubyte buffer[25];
        ALuint length;

        if(!fstream->read(reinterpret_cast<char*>(buffer), 12) ||
           memcmp(buffer, "RIFF", 4) != 0 || memcmp(buffer+8, "WAVE", 4) != 0)
            return;

        while(!dataStart || format == AL_NONE)
        {
            char tag[4];
            if(!fstream->read(tag, 4))
                break;

            /* read chunk length */
            length = read_le32(fstream);

            if(memcmp(tag, "fmt ", 4) == 0 && length >= 16)
            {
                /* Data type (should be 1 for PCM data, 3 for float PCM data,
                 * 7 for muLaw, and 17 for IMA4 data) */
                int type = read_le16(fstream);
                if(type != 0x0001 && type != 0x0003 && type != 0x0007 &&
                   type != 0x0011)
                    break;

                /* mono or stereo data */
                channels = read_le16(fstream);

                /* sample frequency */
                samplerate = read_le32(fstream);

                /* skip average bytes per second */
                fstream->ignore(4);

                /* bytes per block */
                blockAlign = read_le16(fstream);
                if(blockAlign == 0)
                    break;

                /* bits per sample */
                sampleSize = read_le16(fstream);

                length -= 16;

                /* Look for any extra data and try to find the format */
                ALuint extrabytes = 0;
                if(length >= 2)
                {
                    extrabytes = read_le16(fstream);
                    length -= 2;
                }
                extrabytes = std::min<ALuint>(extrabytes, length);

                if(type == 0x0001)
                    format = GetSampleFormat(channels, sampleSize, false);
                else if(type == 0x0003)
                    format = GetSampleFormat(channels, sampleSize, true);
                else if(type == 0x0007)
                {
                    if(sampleSize == 8)
                    {
                        if(channels == 1)
                            format = AL_FORMAT_MONO_MULAW;
                        else if(channels == 2)
                            format = AL_FORMAT_STEREO_MULAW;
                        else if(channels == 4)
                            format = AL_FORMAT_QUAD_MULAW;
                        else if(channels == 6)
                            format = AL_FORMAT_51CHN_MULAW;
                        else if(channels == 7)
                            format = AL_FORMAT_61CHN_MULAW;
                        else if(channels == 8)
                            format = AL_FORMAT_71CHN_MULAW;
                    }
                }
                else if(type == 0x0011 && extrabytes >= 2)
                {
                    int samples = read_le16(fstream);
                    length -= 2;

                    /* AL_EXT_IMA4 only supports 36 bytes-per-channel block
                     * alignment, which has 65 uncompressed sample frames */
                    if(blockAlign == 36*channels && samples == 65*channels &&
                       alIsExtensionPresent("AL_EXT_IMA4"))
                    {
                         if(channels == 1)
                             format = AL_FORMAT_MONO_IMA4;
                         else if(channels == 2)
                             format = AL_FORMAT_STEREO_IMA4;
                    }
                }
            }
            else if(memcmp(tag, "data", 4) == 0)
            {
                dataStart = fstream->tellg();
                dataLen = remLen = length;
            }

            fstream->seekg(length, std::ios_base::cur);
        }

        if(dataStart > 0 && format != AL_NONE)
            fstream->seekg(dataStart);
    }

    virtual ~wavStream()
    { }
};
// Priority = 10, prefer this decoder over external ones
static DecoderDecl<wavStream,10> wavStream_decoder;
