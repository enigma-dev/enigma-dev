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

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <istream>

#include <mpg123.h>


struct mp3Stream : public alureStream {
private:
    mpg123_handle *mp3File;
    long samplerate;
    int channels;
    std::ios::pos_type dataStart;
    std::ios::pos_type dataEnd;

public:
    static void Init() { mpg123_init(); }
    static void Deinit() { mpg123_exit(); }

    virtual bool IsValid()
    { return mp3File != NULL; }

    virtual bool GetFormat(ALenum *fmt, ALuint *frequency, ALuint *blockalign)
    {
        *fmt = GetSampleFormat(channels, 16, false);
        *frequency = samplerate;
        *blockalign = channels*2;
        return true;
    }

    virtual ALuint GetData(ALubyte *data, ALuint bytes)
    {
        if(!mp3File)
            return 0;

        ALuint amt = 0;
        while(bytes > 0)
        {
            size_t got = 0;
            int ret = mpg123_read(mp3File, data, bytes, &got);

            bytes -= got;
            data += got;
            amt += got;

            if(ret == MPG123_NEW_FORMAT || ret == MPG123_DONE)
                break;
            if(got == 0)
                break;
        }
        return amt;
    }

    virtual alureInt64 GetLength()
    {
        alureInt64 len = mpg123_length(mp3File);
        if(len < 0) return 0;
        return len;
    }

    virtual bool Rewind()
    {
        if(mpg123_seek(mp3File, 0, SEEK_SET) >= 0)
            return true;
        SetError("Restart failed");
        return false;
    }

    mp3Stream(std::istream *_fstream)
      : alureStream(_fstream), mp3File(NULL), dataStart(0), dataEnd(0)
    {
        if(!FindDataChunk())
            return;

        mp3File = mpg123_new(NULL, NULL);
        if(mpg123_replace_reader_handle(mp3File, read, lseek, NULL) == MPG123_OK &&
           mpg123_open_handle(mp3File, this) == MPG123_OK)
        {
            int enc;

            if(mpg123_getformat(mp3File, &samplerate, &channels, &enc) == MPG123_OK)
            {
                if(mpg123_format_none(mp3File) == MPG123_OK &&
                   mpg123_format(mp3File, samplerate, channels, MPG123_ENC_SIGNED_16) == MPG123_OK)
                {
                    // All OK
                    return;
                }
            }
            mpg123_close(mp3File);
        }
        mpg123_delete(mp3File);
        mp3File = NULL;
    }

    virtual ~mp3Stream()
    {
        if(mp3File)
        {
            mpg123_close(mp3File);
            mpg123_delete(mp3File);
            mp3File = NULL;
        }
    }

private:
    bool FindDataChunk()
    {
        ALubyte buffer[25];
        int length;

        if(!fstream->read(reinterpret_cast<char*>(buffer), 12))
            return false;

        if(memcmp(buffer, "RIFF", 4) != 0 || memcmp(buffer+8, "WAVE", 4) != 0)
        {
            dataStart = 0;

            // Check for an ID3v2 tag, and skip it
            if(memcmp(buffer, "ID3", 3) == 0 &&
               buffer[3] <= 4 && buffer[4] != 0xff &&
               (buffer[5]&0x0f) == 0 && (buffer[6]&0x80) == 0 &&
               (buffer[7]&0x80) == 0 && (buffer[8]&0x80) == 0 &&
               (buffer[9]&0x80) == 0)
            {
                dataStart = (buffer[6]<<21) | (buffer[7]<<14) |
                            (buffer[8]<< 7) | (buffer[9]    );
                dataStart += ((buffer[5]&0x10) ? 20 : 10);
            }

            if(fstream->seekg(0, std::ios_base::end))
            {
                dataEnd = fstream->tellg();
                fstream->seekg(dataStart);
            }
            return fstream->good();
        }

        int type = 0;
        while(1)
        {
            char tag[4];
            if(!fstream->read(tag, 4))
                break;

            /* read chunk length */
            length = read_le32(fstream);

            if(memcmp(tag, "fmt ", 4) == 0 && length >= 16)
            {
                /* Data type (should be 0x0050 or 0x0055 for MP3 data) */
                type = read_le16(fstream);
                if(type != 0x0050 && type != 0x0055)
                    break;
                length -= 2;
                /* Ignore the rest of the chunk. Everything we need is in the
                 * data stream */
            }
            else if(memcmp(tag, "data", 4) == 0)
            {
                if(type == 0x0050 || type == 0x0055)
                {
                    dataStart = fstream->tellg();
                    dataEnd = dataStart;
                    dataEnd += length;
                    return fstream->good();
                }
            }

            fstream->seekg(length, std::ios_base::cur);
        }

        return false;
    }

    static ssize_t read(void *handle, void *buffer, size_t bytes)
    {
        mp3Stream *self = reinterpret_cast<mp3Stream*>(handle);
        std::istream *fstream = self->fstream;
        fstream->clear();

        std::ios::pos_type rem = self->dataEnd - fstream->tellg();

        fstream->read(reinterpret_cast<char*>(buffer),
                      std::min<std::ios::pos_type>(bytes, rem));
        return fstream->gcount();
    }

    static off_t lseek(void *handle, off_t offset, int whence)
    {
        mp3Stream *self = reinterpret_cast<mp3Stream*>(handle);
        std::istream *fstream = self->fstream;
        fstream->clear();

        if(whence == SEEK_END)
        {
            offset = -offset;
            if(offset >= 0)
                offset = (self->dataEnd - self->dataStart) - offset;
        }
        else if(whence == SEEK_CUR)
            offset = offset + fstream->tellg() - self->dataStart;
        else if(whence != SEEK_SET)
            return -1;

        if(offset >= 0 && std::ios::pos_type(offset) <= (self->dataEnd - self->dataStart))
        {
            if(fstream->seekg(std::ios::pos_type(offset) + self->dataStart))
                return offset;
        }

        return -1;
    }
};
// Priority = -2, because mp3 loading can find false-positives, and interferes
// with mod files
static DecoderDecl<mp3Stream,-2> mp3Stream_decoder;
