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

/* Title: File I/O */

#include "config.h"

#include "main.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>


class MemStreamBuf : public std::streambuf {
    MemDataInfo memInfo;

    virtual int_type underflow()
    {
        if(gptr() == egptr())
        {
            char_type *data = (char_type*)memInfo.Data;
            setg(data, data + memInfo.Pos, data + memInfo.Length);
            memInfo.Pos = memInfo.Length;
        }
        if(gptr() == egptr())
            return traits_type::eof();
        return (*gptr())&0xFF;
    }

    virtual pos_type seekoff(off_type offset, std::ios_base::seekdir whence, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
    {
        if((mode&std::ios_base::out))
            return traits_type::eof();

        pos_type pos;
        switch(whence)
        {
            case std::ios_base::beg:
                pos = pos_type(offset);
                break;
            case std::ios_base::cur:
                pos = pos_type(memInfo.Pos) - pos_type(egptr()-gptr());
                pos += offset;
                break;
            case std::ios_base::end:
                pos = memInfo.Length + pos_type(offset);
                break;
            default:
                return traits_type::eof();
        }

        return seekpos(pos, mode);
    }

    virtual pos_type seekpos(pos_type pos, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
    {
        if((mode&std::ios_base::out))
            return traits_type::eof();

        if(pos < 0 || pos > pos_type(memInfo.Length) || pos != pos_type(size_t(pos)))
            return traits_type::eof();
        memInfo.Pos = pos;

        setg(0, 0, 0);
        return pos;
    }

public:
    MemStreamBuf(const MemDataInfo &data)
      : memInfo(data)
    {
        memInfo.Pos /= sizeof(char_type);
        memInfo.Length /= sizeof(char_type);
    }
    virtual ~MemStreamBuf() { }
};

class FileStreamBuf : public std::streambuf {
    void *usrFile;
    UserFuncs fio;

    char buffer[4096];

    virtual int_type underflow()
    {
        if(usrFile && gptr() == egptr())
        {
            ALsizei amt = fio.read(usrFile, reinterpret_cast<ALubyte*>(&buffer[0]), sizeof(buffer));
            if(amt >= 0) setg(buffer, buffer, buffer+amt);
        }
        if(gptr() == egptr())
            return traits_type::eof();
        return (*gptr())&0xFF;
    }

    virtual pos_type seekoff(off_type offset, std::ios_base::seekdir whence, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
    {
        if(!usrFile || (mode&std::ios_base::out))
            return traits_type::eof();

        pos_type pos = traits_type::eof();
        switch(whence)
        {
            case std::ios_base::beg:
                if(offset == off_type(alureInt64(offset)))
                    pos = pos_type(fio.seek(usrFile, offset, SEEK_SET));
                break;

            case std::ios_base::cur:
                offset -= off_type(egptr()-gptr());
                if(offset == off_type(alureInt64(offset)))
                    pos = pos_type(fio.seek(usrFile, offset, SEEK_CUR));
                break;

            case std::ios_base::end:
                if(offset == off_type(alureInt64(offset)))
                    pos = pos_type(fio.seek(usrFile, offset, SEEK_END));
                break;

            default:
                break;
        }
        if(pos >= 0)
            setg(0, 0, 0);
        return pos;
    }

    virtual pos_type seekpos(pos_type pos, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
    {
        if(pos != pos_type(off_type(pos)))
            return traits_type::eof();
        return seekoff(off_type(pos), std::ios_base::beg, mode);
    }

public:
    bool IsOpen()
    {
        return usrFile != NULL;
    }

    FileStreamBuf(const char *filename, ALint mode)
      : usrFile(NULL), fio(Funcs)
    {
        usrFile = fio.hasUserdata ? fio.openWithUserdata(fio.userdata, filename, mode)
                                  : fio.open(filename, mode);
    }
    virtual ~FileStreamBuf()
    {
        if(usrFile)
            fio.close(usrFile);
    }
};


InStream::InStream(const char *filename)
  : std::istream(new FileStreamBuf(filename, 0))
{
    if(!(static_cast<FileStreamBuf*>(rdbuf())->IsOpen()))
        clear(failbit);
}

InStream::InStream(const MemDataInfo &memInfo)
  : std::istream(new MemStreamBuf(memInfo))
{
}

InStream::~InStream()
{
    delete rdbuf();
}


static void *open_wrap(const char *filename, ALuint mode)
{
    if(mode != 0)
        return NULL;

    return fopen(filename, "rb");
}

static void close_wrap(void *user_data)
{
    FILE *f = (FILE*)user_data;
    fclose(f);
}

static ALsizei read_wrap(void *user_data, ALubyte *buf, ALuint bytes)
{
    FILE *f = (FILE*)user_data;
    return fread(buf, 1, bytes, f);
}

static ALsizei write_wrap(void *user_data, const ALubyte *buf, ALuint bytes)
{
    FILE *f = (FILE*)user_data;
    return fwrite(buf, 1, bytes, f);
}

static alureInt64 seek_wrap(void *user_data, alureInt64 offset, int whence)
{
    FILE *f = (FILE*)user_data;
#ifdef HAVE_FSEEKO
    if(offset != (off_t)offset || fseeko(f, offset, whence) != 0)
        return -1;
    return ftello(f);
#elif defined(HAVE__FSEEKI64)
    if(_fseeki64(f, offset, whence) != 0)
        return -1;
    return _ftelli64(f);
#else
    if(offset != (long)offset || fseek(f, offset, whence) != 0)
        return -1;
    return ftell(f);
#endif
}

UserFuncs Funcs = {
    false,
    NULL,
    { open_wrap },
    close_wrap,
    read_wrap,
    write_wrap,
    seek_wrap
};
bool UsingSTDIO = true;

extern "C" {

/* Function: alureSetIOCallbacks
 *
 * Provides callbacks for alternative methods to handle file I/O. Passing NULL
 * for all callbacks is a valid way to revert to normal I/O, otherwise they
 * must all be specified. Changing the callbacks will not affect open files
 * (they will continue using the callbacks that were set at the time they were
 * opened).
 *
 * Parameters:
 * open - This callback is called to open the named file. The given mode is the
 *        access rights the open file should have. Currently, this will always
 *        be 0 for read-only (applications should check this to make sure, as
 *        future versions may pass other values for other modes). Upon success,
 *        a non-NULL handle must be returned which will be used as a unique
 *        identifier for the file.
 * close - This callback is called to close an opened file handle. The handle
 *         will no longer be used after this function.
 * read - This callback is called when data needs to be read from the given
 *        handle. Up to the given number of bytes should be copied into 'buf'
 *        and the number of bytes actually copied should be returned. Returning
 *        0 means the end of the file has been reached (so non-blocking I/O
 *        methods should ensure at least 1 byte gets read), and negative
 *        indicates an error.
 * write - This callback is called when data needs to be written to the given
 *         handle. Up to the given number of bytes should be copied from 'buf'
 *         and the number of bytes actually copied should be returned. A return
 *         value of 0 means no more data can be written (so non-blocking I/O
 *         methods should ensure at least 1 byte gets written), and negative
 *         indicates an error.
 * seek - This callback is called to reposition the offset of the file handle.
 *        The given offset is interpreted according to 'whence', which may be
 *        SEEK_SET (absolute position from the start of the file), SEEK_CUR
 *        (relative position from the current offset), or SEEK_END (absolute
 *        position from the end of the file), as defined by standard C. The new
 *        offset from the beginning of the file should be returned. If the file
 *        cannot seek, such as when using a FIFO, -1 should be returned.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See also:
 * <alureSetIOCallbacksUserdata>
 *
 * *Version Added*: 1.1
 */
ALURE_API ALboolean ALURE_APIENTRY alureSetIOCallbacks(
      void* (*open)(const char *filename, ALuint mode),
      void (*close)(void *handle),
      ALsizei (*read)(void *handle, ALubyte *buf, ALuint bytes),
      ALsizei (*write)(void *handle, const ALubyte *buf, ALuint bytes),
      alureInt64 (*seek)(void *handle, alureInt64 offset, int whence))
{
    if(open && close && read && write && seek)
    {
        Funcs.hasUserdata = false;
        Funcs.open = open;
        Funcs.close = close;
        Funcs.read = read;
        Funcs.write = write;
        Funcs.seek = seek;
        UsingSTDIO = false;
        return AL_TRUE;
    }

    if(!open && !close && !read && !write && !seek)
    {
        Funcs.hasUserdata = false;
        Funcs.open = open_wrap;
        Funcs.close = close_wrap;
        Funcs.read = read_wrap;
        Funcs.write = write_wrap;
        Funcs.seek = seek_wrap;
        UsingSTDIO = true;
        return AL_TRUE;
    }

    SetError("Missing callback functions");
    return AL_FALSE;
}

/* Function: alureSetIOCallbacksUserdata
 *
 * Provides callbacks for alternative methods to handle file I/O. Passing NULL
 * for all callbacks is a valid way to revert to normal I/O, otherwise they
 * must all be specified. Changing the callbacks will not affect open files
 * (they will continue using the callbacks that were set at the time they were
 * opened).
 *
 * Parameters:
 * userdata - A handle passed through to the open-callback.
 * open - This callback is called to open the named file. The given mode is the
 *        access rights the open file should have. Currently, this will always
 *        be 0 for read-only (applications should check this to make sure, as
 *        future versions may pass other values for other modes). Upon success,
 *        a non-NULL handle must be returned which will be used as a unique
 *        identifier for the file.
 * close - This callback is called to close an opened file handle. The handle
 *         will no longer be used after this function.
 * read - This callback is called when data needs to be read from the given
 *        handle. Up to the given number of bytes should be copied into 'buf'
 *        and the number of bytes actually copied should be returned. Returning
 *        0 means the end of the file has been reached (so non-blocking I/O
 *        methods should ensure at least 1 byte gets read), and negative
 *        indicates an error.
 * write - This callback is called when data needs to be written to the given
 *         handle. Up to the given number of bytes should be copied from 'buf'
 *         and the number of bytes actually copied should be returned. A return
 *         value of 0 means no more data can be written (so non-blocking I/O
 *         methods should ensure at least 1 byte gets written), and negative
 *         indicates an error.
 * seek - This callback is called to reposition the offset of the file handle.
 *        The given offset is interpreted according to 'whence', which may be
 *        SEEK_SET (absolute position from the start of the file), SEEK_CUR
 *        (relative position from the current offset), or SEEK_END (absolute
 *        position from the end of the file), as defined by standard C. The new
 *        offset from the beginning of the file should be returned. If the file
 *        cannot seek, such as when using a FIFO, -1 should be returned.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See also:
 * <alureSetIOCallbacks>
 *
 * *Version Added*: 1.3
 */
ALURE_API ALboolean ALURE_APIENTRY alureSetIOCallbacksUserdata(
      void *userdata,
      void* (*open)(void *userdata, const char *filename, ALuint mode),
      void (*close)(void *handle),
      ALsizei (*read)(void *handle, ALubyte *buf, ALuint bytes),
      ALsizei (*write)(void *handle, const ALubyte *buf, ALuint bytes),
      alureInt64 (*seek)(void *handle, alureInt64 offset, int whence))
{
    if(open && close && read && write && seek)
    {
        Funcs.hasUserdata = true;
        Funcs.userdata = userdata;
        Funcs.openWithUserdata = open;
        Funcs.close = close;
        Funcs.read = read;
        Funcs.write = write;
        Funcs.seek = seek;
        UsingSTDIO = false;
        return AL_TRUE;
    }

    if(!open && !close && !read && !write && !seek)
    {
        Funcs.hasUserdata = false;
        Funcs.open = open_wrap;
        Funcs.close = close_wrap;
        Funcs.read = read_wrap;
        Funcs.write = write_wrap;
        Funcs.seek = seek_wrap;
        UsingSTDIO = true;
        return AL_TRUE;
    }

    SetError("Missing callback functions");
    return AL_FALSE;
}
} // extern "C"
