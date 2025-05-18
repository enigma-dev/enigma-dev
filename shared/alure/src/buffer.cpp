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

/* Title: File Loading */

#include "config.h"

#include "main.h"

#include <string.h>

#include <vector>
#include <memory>


static bool load_stream(alureStream *_stream, ALuint buffer)
{
    if(!_stream)
        return false;

    std::auto_ptr<std::istream> fstream(_stream->fstream);
    std::auto_ptr<alureStream> stream(_stream);

    ALenum format;
    ALuint freq, blockAlign;

    if(!stream->GetFormat(&format, &freq, &blockAlign))
    {
        SetError("Could not get sample format");
        return false;
    }

    if(format == AL_NONE)
    {
        SetError("No valid format");
        return false;
    }
    if(blockAlign == 0)
    {
        SetError("Invalid block size");
        return false;
    }
    if(freq == 0)
    {
        SetError("Invalid sample rate");
        return false;
    }

    ALuint writePos = 0, got;
    std::vector<ALubyte> data(freq*blockAlign);
    while((got=stream->GetData(&data[writePos], data.size()-writePos)) > 0)
    {
        writePos += got;
        data.resize(writePos + freq*blockAlign);
    }
    data.resize(writePos - (writePos%blockAlign));
    stream.reset(NULL);

    alBufferData(buffer, format, &data[0], data.size(), freq);
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Buffer load failed");
        return false;
    }

    return true;
}

extern "C" {

/* Function: alureCreateBufferFromFile
 *
 * Loads the given file into a new OpenAL buffer object. The formats supported
 * depend on the options the library was compiled with, what libraries are
 * available at runtime, and the installed decode callbacks. Requires an active
 * context.
 *
 * Returns:
 * A new buffer ID with the loaded sound, or AL_NONE on error.
 *
 * See Also:
 * <alureBufferDataFromFile>
 */
ALURE_API ALuint ALURE_APIENTRY alureCreateBufferFromFile(const ALchar *fname)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_NONE;
    }

    ALuint buf;
    alGenBuffers(1, &buf);
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Buffer creation failed");
        return AL_NONE;
    }

    if(alureBufferDataFromFile(fname, buf) == AL_FALSE)
    {
        alDeleteBuffers(1, &buf);
        alGetError();
        buf = AL_NONE;
    }

    return buf;
}

/* Function: alureCreateBufferFromMemory
 *
 * Loads a file image from memory into a new OpenAL buffer object, similar to
 * alureCreateBufferFromFile. Requires an active context.
 *
 * Returns:
 * A new buffer ID with the loaded sound, or AL_NONE on error.
 *
 * See Also:
 * <alureBufferDataFromMemory>
 */
ALURE_API ALuint ALURE_APIENTRY alureCreateBufferFromMemory(const ALubyte *fdata, ALsizei length)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_NONE;
    }

    ALuint buf;
    alGenBuffers(1, &buf);
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Buffer creation failed");
        return AL_NONE;
    }

    if(alureBufferDataFromMemory(fdata, length, buf) == AL_FALSE)
    {
        alDeleteBuffers(1, &buf);
        alGetError();
        buf = AL_NONE;
    }

    return buf;
}

/* Function: alureBufferDataFromFile
 *
 * Loads the given file into an existing OpenAL buffer object. The previous
 * contents of the buffer are replaced. Requires an active context.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureCreateBufferFromFile>
 */
ALURE_API ALboolean ALURE_APIENTRY alureBufferDataFromFile(const ALchar *fname, ALuint buffer)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_FALSE;
    }

    if(!buffer || !alIsBuffer(buffer))
    {
        SetError("Invalid buffer ID");
        return false;
    }

    if(load_stream(create_stream(fname), buffer) == false)
        return AL_FALSE;
    return AL_TRUE;
}

/* Function: alureBufferDataFromMemory
 *
 * Loads a file image from memory into an existing OpenAL buffer object,
 * similar to alureBufferDataFromFile. Requires an active context.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureCreateBufferFromMemory>
 */
ALURE_API ALboolean ALURE_APIENTRY alureBufferDataFromMemory(const ALubyte *fdata, ALsizei length, ALuint buffer)
{
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_FALSE;
    }

    if(!buffer || !alIsBuffer(buffer))
    {
        SetError("Invalid buffer ID");
        return false;
    }

    if(length < 0)
    {
        SetError("Invalid data length");
        return AL_FALSE;
    }

    MemDataInfo memData;
    memData.Data = fdata;
    memData.Length = length;
    memData.Pos = 0;

    if(load_stream(create_stream(memData), buffer) == false)
        return AL_FALSE;
    return AL_TRUE;
}

} // extern "C"
