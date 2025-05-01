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

/* Title: Streaming */

#include "config.h"

#include "main.h"

#include <string.h>

#include <memory>

static bool SizeIsUS = false;

static alureStream *InitStream(alureStream *instream, ALsizei chunkLength, ALsizei numBufs, ALuint *bufs)
{
    std::auto_ptr<std::istream> fstream(instream->fstream);
    std::auto_ptr<alureStream> stream(instream);
    ALenum format;
    ALuint freq, blockAlign;

    if(!stream->GetFormat(&format, &freq, &blockAlign))
    {
        SetError("Could not get stream format");
        return NULL;
    }

    if(format == AL_NONE || format == -1)
    {
        SetError("No valid format");
        return NULL;
    }
    if(blockAlign == 0)
    {
        SetError("Invalid block size");
        return NULL;
    }
    if(freq == 0)
    {
        SetError("Invalid sample rate");
        return NULL;
    }

    if(SizeIsUS)
    {
        ALuint framesPerBlock = DetectCompressionRate(format);
        ALuint blockSize = DetectBlockAlignment(format);
        if(framesPerBlock == 0 || blockSize == 0)
        {
            SetError("Unknown compression rate");
            return NULL;
        }

        alureUInt64 len64 = chunkLength;
        len64 = len64 * freq / 1000000 / framesPerBlock * blockSize;
        if(len64 > 0x7FFFFFFF)
        {
            SetError("Chunk length too large");
            return NULL;
        }
        chunkLength = len64;
    }

    chunkLength -= chunkLength%blockAlign;
    if(chunkLength <= 0)
    {
        SetError("Chunk length too small");
        return NULL;
    }

    stream->dataChunk.resize(chunkLength);

    if(numBufs > 0)
    {
        alGenBuffers(numBufs, bufs);
        if(alGetError() != AL_NO_ERROR)
        {
            SetError("Buffer creation failed");
            return NULL;
        }
    }

    ALsizei filled;
    for(filled = 0;filled < numBufs;filled++)
    {
        ALuint got = stream->GetData(&stream->dataChunk[0], stream->dataChunk.size());
        got -= got%blockAlign;
        if(got == 0) break;

        alBufferData(bufs[filled], format, &stream->dataChunk[0], got, freq);
    }

    while(filled < numBufs)
    {
        alBufferData(bufs[filled], format, &stream->dataChunk[0], 0, freq);
        filled++;
    }
    if(alGetError() != AL_NO_ERROR)
    {
        alDeleteBuffers(numBufs, bufs);
        alGetError();

        SetError("Buffering error");
        return NULL;
    }

    fstream.release();
    return stream.release();
}


extern "C" {

/* Function: alureStreamSizeIsMicroSec
 *
 * Specifies if the chunk size value given to the alureCreateStream functions
 * is in bytes (default) or microseconds. Specifying the size in microseconds
 * can help manage the time needed in between needed updates (since the format
 * and sample rate of the stream may not be known), while specifying the size
 * in bytes can help control memory usage.
 *
 * Returns:
 * Previously set value.
 *
 * *Version Added*: 1.1
 *
 * See Also:
 * <alureCreateStreamFromFile>, <alureCreateStreamFromMemory>,
 * <alureCreateStreamFromStaticMemory>, <alureCreateStreamFromCallback>
 */
ALURE_API ALboolean ALURE_APIENTRY alureStreamSizeIsMicroSec(ALboolean useUS)
{
    ALboolean old = (SizeIsUS ? AL_TRUE : AL_FALSE);
    SizeIsUS = !!useUS;
    return old;
}

/* Function: alureCreateStreamFromFile
 *
 * Opens a file and sets it up for streaming. The given chunkLength is the
 * number of bytes, or microseconds worth of bytes if
 * <alureStreamSizeIsMicroSec> was last called with AL_TRUE, each buffer will
 * fill with. ALURE will optionally generate the specified number of buffer
 * objects, fill them with the beginning of the data, then place the new IDs
 * into the provided storage, before returning. Requires an active context.
 *
 * Returns:
 * An opaque handle used to control the opened stream, or NULL on error.
 *
 * See Also:
 * <alureStreamSizeIsMicroSec>, <alureCreateStreamFromMemory>,
 * <alureCreateStreamFromStaticMemory>, <alureCreateStreamFromCallback>
 */
ALURE_API alureStream* ALURE_APIENTRY alureCreateStreamFromFile(const ALchar *fname, ALsizei chunkLength, ALsizei numBufs, ALuint *bufs)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return NULL;
    }

    if(chunkLength < 0)
    {
        SetError("Invalid chunk length");
        return NULL;
    }

    if(numBufs < 0)
    {
        SetError("Invalid buffer count");
        return NULL;
    }

    alureStream *stream = create_stream(fname);
    if(!stream) return NULL;

    return InitStream(stream, chunkLength, numBufs, bufs);
}

/* Function: alureCreateStreamFromMemory
 *
 * Opens a file image from memory and sets it up for streaming, similar to
 * <alureCreateStreamFromFile>. The given data buffer can be safely deleted
 * after calling this function. Requires an active context.
 *
 * Returns:
 * An opaque handle used to control the opened stream, or NULL on error.
 *
 * See Also:
 * <alureStreamSizeIsMicroSec>, <alureCreateStreamFromFile>,
 * <alureCreateStreamFromStaticMemory>, <alureCreateStreamFromCallback>
 */
ALURE_API alureStream* ALURE_APIENTRY alureCreateStreamFromMemory(const ALubyte *fdata, ALuint length, ALsizei chunkLength, ALsizei numBufs, ALuint *bufs)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return NULL;
    }

    if(chunkLength < 0)
    {
        SetError("Invalid chunk length");
        return NULL;
    }

    if(numBufs < 0)
    {
        SetError("Invalid buffer count");
        return NULL;
    }

    if(length <= 0)
    {
        SetError("Invalid data length");
        return NULL;
    }

    ALubyte *streamData = new ALubyte[length];
    memcpy(streamData, fdata, length);

    MemDataInfo memData;
    memData.Data = streamData;
    memData.Length = length;
    memData.Pos = 0;

    alureStream *stream = create_stream(memData);
    if(!stream) return NULL;

    stream->data = streamData;
    return InitStream(stream, chunkLength, numBufs, bufs);
}

/* Function: alureCreateStreamFromStaticMemory
 *
 * Identical to <alureCreateStreamFromMemory>, except the given memory is used
 * directly and not duplicated. As a consequence, the data buffer must remain
 * valid while the stream is alive. Requires an active context.
 *
 * Returns:
 * An opaque handle used to control the opened stream, or NULL on error.
 *
 * See Also:
 * <alureStreamSizeIsMicroSec>, <alureCreateStreamFromFile>,
 * <alureCreateStreamFromMemory>, <alureCreateStreamFromCallback>
 */
ALURE_API alureStream* ALURE_APIENTRY alureCreateStreamFromStaticMemory(const ALubyte *fdata, ALuint length, ALsizei chunkLength, ALsizei numBufs, ALuint *bufs)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return NULL;
    }

    if(chunkLength < 0)
    {
        SetError("Invalid chunk length");
        return NULL;
    }

    if(numBufs < 0)
    {
        SetError("Invalid buffer count");
        return NULL;
    }

    if(length <= 0)
    {
        SetError("Invalid data length");
        return NULL;
    }

    MemDataInfo memData;
    memData.Data = fdata;
    memData.Length = length;
    memData.Pos = 0;

    alureStream *stream = create_stream(memData);
    if(!stream) return NULL;

    return InitStream(stream, chunkLength, numBufs, bufs);
}

/* Function: alureCreateStreamFromCallback
 *
 * Creates a stream using the specified callback to retrieve data. Requires an
 * active context.
 *
 * Parameters:
 * callback - This is called when more data is needed from the stream. Up to
 *            the specified number of bytes should be written to the data
 *            pointer, and the number of bytes actually written should be
 *            returned. The number of bytes written must be block aligned for
 *            the format (eg. a multiple of 4 for AL_FORMAT_STEREO16), or an
 *            OpenAL error may occur during buffering.
 * userdata - A handle passed through to the callback.
 * format - The format of the data the callback will be giving. The format must
 *          be valid for the context.
 * samplerate - The sample rate (frequency) of the stream
 *
 * Returns:
 * An opaque handle used to control the opened stream, or NULL on error.
 *
 * See Also:
 * <alureStreamSizeIsMicroSec>, <alureCreateStreamFromFile>,
 * <alureCreateStreamFromMemory>, <alureCreateStreamFromStaticMemory>
 */
ALURE_API alureStream* ALURE_APIENTRY alureCreateStreamFromCallback(
      ALuint (*callback)(void *userdata, ALubyte *data, ALuint bytes),
      void *userdata, ALenum format, ALuint samplerate,
      ALsizei chunkLength, ALsizei numBufs, ALuint *bufs)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return NULL;
    }

    if(callback == NULL)
    {
        SetError("Invalid callback");
        return NULL;
    }

    if(chunkLength < 0)
    {
        SetError("Invalid chunk length");
        return NULL;
    }

    if(numBufs < 0)
    {
        SetError("Invalid buffer count");
        return NULL;
    }

    UserCallbacks newcb;
    newcb.open_file = NULL;
    newcb.open_mem  = NULL;
    newcb.get_fmt   = NULL;
    newcb.decode    = callback;
    newcb.rewind    = NULL;
    newcb.close     = NULL;

    alureStream *stream = create_stream(userdata, format, samplerate, newcb);
    return InitStream(stream, chunkLength, numBufs, bufs);
}

/* Function: alureGetStreamFrequency
 *
 * Retrieves the frequency used by the given stream.
 *
 * Returns:
 * 0 on error.
 *
 * *Version Added*: 1.1
 */
ALURE_API ALsizei ALURE_APIENTRY alureGetStreamFrequency(alureStream *stream)
{
    ALenum format;
    ALuint rate, balign;

    if(!alureStream::Verify(stream))
    {
        SetError("Invalid stream pointer");
        return 0;
    }

    if(!stream->GetFormat(&format, &rate, &balign))
    {
        SetError("Could not get stream format");
        return 0;
    }

    return rate;
}

/* Function: alureBufferDataFromStream
 *
 * Buffers the given buffer objects with the next chunks of data from the
 * stream. The given buffer objects do not need to be ones given by the
 * alureCreateStream functions. Requires an active context.
 *
 * Returns:
 * The number of buffers filled with new data, or -1 on error. If the value
 * returned is less than the number requested, the end of the stream has been
 * reached.
 */
ALURE_API ALsizei ALURE_APIENTRY alureBufferDataFromStream(alureStream *stream, ALsizei numBufs, ALuint *bufs)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return -1;
    }

    if(!alureStream::Verify(stream))
    {
        SetError("Invalid stream pointer");
        return -1;
    }

    if(numBufs < 0)
    {
        SetError("Invalid buffer count");
        return -1;
    }

    for(ALsizei i = 0;i < numBufs;i++)
    {
        if(!bufs[i] || !alIsBuffer(bufs[i]))
        {
            SetError("Invalid buffer ID");
            return -1;
        }
    }

    ALenum format;
    ALuint freq, blockAlign;

    if(!stream->GetFormat(&format, &freq, &blockAlign))
    {
        SetError("Could not get stream format");
        return -1;
    }

    ALsizei filled;
    for(filled = 0;filled < numBufs;filled++)
    {
        ALuint got = stream->GetData(&stream->dataChunk[0], stream->dataChunk.size());
        got -= got%blockAlign;
        if(got == 0) break;

        alBufferData(bufs[filled], format, &stream->dataChunk[0], got, freq);
        if(alGetError() != AL_NO_ERROR)
        {
            SetError("Buffer load failed");
            return -1;
        }
    }

    return filled;
}

/* Function: alureRewindStream
 *
 * Rewinds the stream so that the next alureBufferDataFromStream call will
 * restart from the beginning of the audio file.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureSetStreamOrder>
 */
ALURE_API ALboolean ALURE_APIENTRY alureRewindStream(alureStream *stream)
{
    if(!alureStream::Verify(stream))
    {
        SetError("Invalid stream pointer");
        return AL_FALSE;
    }

    return stream->Rewind();
}

/* Function: alureSetStreamOrder
 *
 * Skips the module decoder to the specified order, so following buffering
 * calls will decode from the specified order. For non-module formats, setting
 * order 0 is identical to rewinding the stream (other orders will fail).
 *
 * Returns:
 * AL_FALSE on error.
 *
 * *Version Added*: 1.1
 *
 * See Also:
 * <alureRewindStream>
 */
ALURE_API ALboolean ALURE_APIENTRY alureSetStreamOrder(alureStream *stream, ALuint order)
{
    if(!alureStream::Verify(stream))
    {
        SetError("Invalid stream pointer");
        return AL_FALSE;
    }

    return stream->SetOrder(order);
}

/* Function: alureSetStreamPatchset
 *
 * Specifies the patchset to use for MIDI streams. By default, the FluidSynth
 * decoder will look for one in the FLUID_SOUNDFONT environment variable, but
 * this can be used to change it to something different. On non-MIDI streams,
 * this has no effect.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * *Version Added*: 1.1
 */
ALURE_API ALboolean ALURE_APIENTRY alureSetStreamPatchset(alureStream *stream, const ALchar *patchset)
{
    if(!alureStream::Verify(stream))
    {
        SetError("Invalid stream pointer");
        return AL_FALSE;
    }

    return stream->SetPatchset(patchset);
}

/* Function: alureGetStreamLength
 *
 * Retrieves an approximate number of samples for the stream. Not all streams
 * or decoders can return such info, and may return 0 if the stream length is
 * unknown.
 *
 * Returns:
 * -1 on error.
 *
 * *Version Added*: 1.2
 */
ALURE_API alureInt64 ALURE_APIENTRY alureGetStreamLength(alureStream *stream)
{
    if(!alureStream::Verify(stream))
    {
        SetError("Invalid stream pointer");
        return -1;
    }

    return stream->GetLength();
}

/* Function: alureDestroyStream
 *
 * Closes an opened stream. For convenience, it will also delete the given
 * buffer objects. The given buffer objects do not need to be ones given by the
 * alureCreateStream functions. Requires an active context.
 *
 * Returns:
 * AL_FALSE on error.
 */
ALURE_API ALboolean ALURE_APIENTRY alureDestroyStream(alureStream *stream, ALsizei numBufs, ALuint *bufs)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_FALSE;
    }

    if(numBufs < 0)
    {
        SetError("Invalid buffer count");
        return AL_FALSE;
    }

    if(stream && !alureStream::Verify(stream))
    {
        SetError("Invalid stream pointer");
        return AL_FALSE;
    }

    if(numBufs > 0)
    {
        alDeleteBuffers(numBufs, bufs);
        if(alGetError() != AL_NO_ERROR)
        {
            SetError("Buffer deletion failed");
            return AL_FALSE;
        }
    }

    if(stream)
    {
        StopStream(stream);
        std::istream *f = stream->fstream;
        delete stream;
        delete f;
    }
    return AL_TRUE;
}

}
