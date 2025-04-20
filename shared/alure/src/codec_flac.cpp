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

#include <FLAC/all.h>


struct flacStream : public alureStream {
private:
    FLAC__StreamDecoder *flacFile;
    ALenum format;
    ALuint samplerate;
    ALuint blockAlign;
    ALboolean useFloat;

    std::vector<ALubyte> initialData;

    ALubyte *outBytes;
    ALuint outMax;
    ALuint outLen;

public:
    static void Init() { }
    static void Deinit() { }

    virtual bool IsValid()
    { return flacFile != NULL; }

    virtual bool GetFormat(ALenum *fmt, ALuint *frequency, ALuint *blockalign)
    {
        *fmt = format;
        *frequency = samplerate;
        *blockalign = blockAlign;
        return true;
    }

    virtual ALuint GetData(ALubyte *data, ALuint bytes)
    {
        outBytes = data;
        outLen = 0;
        outMax = bytes;

        if(initialData.size() > 0)
        {
            size_t rem = std::min(initialData.size(), (size_t)bytes);
            memcpy(data, &initialData[0], rem);
            outLen += rem;
            initialData.erase(initialData.begin(), initialData.begin()+rem);
        }

        while(outLen < outMax)
        {
            if(FLAC__stream_decoder_process_single(flacFile) == false ||
               FLAC__stream_decoder_get_state(flacFile) == FLAC__STREAM_DECODER_END_OF_STREAM)
                break;
        }

        return outLen;
    }

    virtual bool Rewind()
    {
        if(FLAC__stream_decoder_seek_absolute(flacFile, 0) != false)
        {
            initialData.clear();
            return true;
        }

        SetError("Seek failed");
        return false;
    }

    virtual alureInt64 GetLength()
    {
        return FLAC__stream_decoder_get_total_samples(flacFile);
    }

    flacStream(std::istream *_fstream)
      : alureStream(_fstream), flacFile(NULL), format(AL_NONE), samplerate(0),
        blockAlign(0), useFloat(AL_FALSE)
    {
        flacFile = FLAC__stream_decoder_new();
        if(flacFile)
        {
            if(FLAC__stream_decoder_init_stream(flacFile, ReadCallback, SeekCallback, TellCallback, LengthCallback, EofCallback, WriteCallback, MetadataCallback, ErrorCallback, this) == FLAC__STREAM_DECODER_INIT_STATUS_OK)
            {
                if(InitFlac())
                {
                    // all ok
                    return;
                }

                FLAC__stream_decoder_finish(flacFile);
            }
            FLAC__stream_decoder_delete(flacFile);
            flacFile = NULL;
        }
    }

    virtual ~flacStream()
    {
        if(flacFile)
        {
            FLAC__stream_decoder_finish(flacFile);
            FLAC__stream_decoder_delete(flacFile);
            flacFile = NULL;
        }
    }

private:
    bool InitFlac()
    {
        // We need to decode some data to be able to get the channel count, bit
        // depth, and sample rate. It also ensures the file has FLAC data, as
        // the FLAC__stream_decoder_init_* functions can succeed on non-FLAC
        // Ogg files.
        outBytes = NULL;
        outMax = 0;
        outLen = 0;
        while(initialData.size() == 0)
        {
            if(FLAC__stream_decoder_process_single(flacFile) == false ||
               FLAC__stream_decoder_get_state(flacFile) == FLAC__STREAM_DECODER_END_OF_STREAM)
                break;
        }

        if(initialData.size() > 0)
            return true;
        return false;
    }


    template<ALuint shift, ALint offset, typename T>
    static void CopySamples(T *data, const FLAC__int32 *const buffer[], ALuint off, ALuint todo, ALuint channels)
    {
        for(ALuint i = 0;i < todo;i++)
        {
            for(ALuint c = 0;c < channels;c++)
                *(data++) = (buffer[c][off+i]>>shift) + offset;
        }
    }

    template<ALuint bits>
    static void CopySamplesFloat(ALfloat *data, const FLAC__int32 *const buffer[], ALuint off, ALuint todo, ALuint channels)
    {
        for(ALuint i = 0;i < todo;i++)
        {
            for(ALuint c = 0;c < channels;c++)
                *(data++) = buffer[c][off+i] * (1./((1u<<(bits-1))-1));
        }
    }

    static FLAC__StreamDecoderWriteStatus WriteCallback(const FLAC__StreamDecoder*, const FLAC__Frame *frame, const FLAC__int32 *const buffer[], void *client_data)
    {
        flacStream *self = static_cast<flacStream*>(client_data);

        if(self->format == AL_NONE)
        {
            ALuint bps = frame->header.bits_per_sample;
            if(bps == 24 || bps == 32)
            {
                self->format = GetSampleFormat(frame->header.channels, 32, true);
                if(self->format != AL_NONE)
                {
                    self->useFloat = AL_TRUE;
                    bps = 32;
                }
                else bps = 16;
            }
            if(self->format == AL_NONE)
                self->format = GetSampleFormat(frame->header.channels, bps, false);
            self->blockAlign = frame->header.channels * bps/8;
            self->samplerate = frame->header.sample_rate;
        }

        ALubyte *data = self->outBytes + self->outLen;
        ALuint todo = std::min<ALuint>((self->outMax-self->outLen) / self->blockAlign,
                                       frame->header.blocksize);
        if(frame->header.bits_per_sample == 8)
            CopySamples<0,128>((ALubyte*)data, buffer, 0,
                               todo, frame->header.channels);
        else if(frame->header.bits_per_sample == 16)
            CopySamples<0,0>((ALshort*)data, buffer, 0,
                             todo, frame->header.channels);
        else if(frame->header.bits_per_sample == 24)
        {
            if(self->useFloat)
                CopySamplesFloat<24>((ALfloat*)data, buffer, 0,
                                     todo, frame->header.channels);
            else
                CopySamples<8,0>((ALshort*)data, buffer, 0,
                                 todo, frame->header.channels);
        }
        else if(frame->header.bits_per_sample == 32)
        {
            if(self->useFloat)
                CopySamplesFloat<32>((ALfloat*)data, buffer, 0,
                                     todo, frame->header.channels);
            else
                CopySamples<16,0>((ALshort*)data, buffer, 0,
                                  todo, frame->header.channels);
        }
        self->outLen += self->blockAlign * todo;

        if(todo < frame->header.blocksize)
        {
            ALuint offset = todo;
            todo = frame->header.blocksize - todo;

            ALuint blocklen = todo * self->blockAlign;
            ALuint start = self->initialData.size();

            self->initialData.resize(start+blocklen);
            data = &self->initialData[start];

            if(frame->header.bits_per_sample == 8)
                CopySamples<0,128>((ALubyte*)data, buffer, offset,
                                   todo, frame->header.channels);
            else if(frame->header.bits_per_sample == 16)
                CopySamples<0,0>((ALshort*)data, buffer, offset,
                                 todo, frame->header.channels);
            else if(frame->header.bits_per_sample == 24)
            {
                if(self->useFloat)
                    CopySamplesFloat<24>((ALfloat*)data, buffer, offset,
                                         todo, frame->header.channels);
                else
                    CopySamples<8,0>((ALshort*)data, buffer, offset,
                                     todo, frame->header.channels);
            }
            else if(frame->header.bits_per_sample == 32)
            {
                if(self->useFloat)
                    CopySamplesFloat<32>((ALfloat*)data, buffer, offset,
                                         todo, frame->header.channels);
                else
                    CopySamples<16,0>((ALshort*)data, buffer, offset,
                                      todo, frame->header.channels);
            }
        }

        return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
    }
    static void MetadataCallback(const FLAC__StreamDecoder*,const FLAC__StreamMetadata*,void*)
    {
    }
    static void ErrorCallback(const FLAC__StreamDecoder*,FLAC__StreamDecoderErrorStatus,void*)
    {
    }

    static FLAC__StreamDecoderReadStatus ReadCallback(const FLAC__StreamDecoder*, FLAC__byte buffer[], size_t *bytes, void *client_data)
    {
        std::istream *stream = static_cast<flacStream*>(client_data)->fstream;
        stream->clear();

        if(*bytes <= 0)
            return FLAC__STREAM_DECODER_READ_STATUS_ABORT;

        stream->read(reinterpret_cast<char*>(buffer), *bytes);
        *bytes = stream->gcount();
        if(*bytes == 0 && stream->eof())
            return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;

        return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
    }
    static FLAC__StreamDecoderSeekStatus SeekCallback(const FLAC__StreamDecoder*, FLAC__uint64 absolute_byte_offset, void *client_data)
    {
        std::istream *stream = static_cast<flacStream*>(client_data)->fstream;
        stream->clear();

        if(!stream->seekg(absolute_byte_offset))
            return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
        return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
    }
    static FLAC__StreamDecoderTellStatus TellCallback(const FLAC__StreamDecoder*, FLAC__uint64 *absolute_byte_offset, void *client_data)
    {
        std::istream *stream = static_cast<flacStream*>(client_data)->fstream;
        stream->clear();

        *absolute_byte_offset = stream->tellg();
        return FLAC__STREAM_DECODER_TELL_STATUS_OK;
    }
    static FLAC__StreamDecoderLengthStatus LengthCallback(const FLAC__StreamDecoder*, FLAC__uint64 *stream_length, void *client_data)
    {
        std::istream *stream = static_cast<flacStream*>(client_data)->fstream;
        stream->clear();

        std::streampos pos = stream->tellg();
        if(stream->seekg(0, std::ios_base::end))
        {
            *stream_length = stream->tellg();
            stream->seekg(pos);
        }

        if(!stream->good())
            return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
        return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
    }
    static FLAC__bool EofCallback(const FLAC__StreamDecoder*, void *client_data)
    {
        std::istream *stream = static_cast<flacStream*>(client_data)->fstream;
        return (stream->eof()) ? true : false;
    }
};
// Priority = 1, so it's preferred over libsndfile
static DecoderDecl<flacStream,1> flacStream_decoder;
