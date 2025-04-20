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

#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <istream>
#include <fstream>
#include <iostream>
#include <sstream>


const Decoder::ListType& Decoder::GetList()
{ return AddList(); }

Decoder::ListType& Decoder::AddList(Decoder::FactoryType func, ALint prio)
{
    static ListType FuncList;
    if(func)
    {
        assert(std::find_if(FuncList.begin(), FuncList.end(), FindSecond(func)) == FuncList.end());
        FuncList.insert(std::make_pair(prio, func));
    }
    return FuncList;
}


struct customStream : public alureStream {
    void *usrFile;
    ALenum format;
    ALuint samplerate;
    ALuint blockAlign;
    MemDataInfo memInfo;

    UserCallbacks cb;

    virtual bool IsValid()
    { return usrFile != NULL; }

    virtual bool GetFormat(ALenum *fmt, ALuint *frequency, ALuint *blockalign)
    {
        if(format == AL_NONE)
        {
            if(!cb.get_fmt || !cb.get_fmt(usrFile, &format, &samplerate, &blockAlign))
                return false;

            ALuint detected = DetectBlockAlignment(format);
            if(detected && (blockAlign%detected) != 0)
                blockAlign = 0;
        }

        *fmt = format;
        *frequency = samplerate;
        *blockalign = blockAlign;
        return true;
    }

    virtual ALuint GetData(ALubyte *data, ALuint bytes)
    { return cb.decode(usrFile, data, bytes); }

    virtual bool Rewind()
    {
        if(cb.rewind && cb.rewind(usrFile))
            return true;
        SetError("Rewind failed");
        return false;
    }

    customStream(const char *fname, const UserCallbacks &callbacks)
      : alureStream(NULL), usrFile(NULL), format(AL_NONE), samplerate(0),
        blockAlign(0), cb(callbacks)
    { if(cb.open_file) usrFile = cb.open_file(fname); }

    customStream(const MemDataInfo &memData, const UserCallbacks &callbacks)
      : alureStream(NULL), usrFile(NULL), format(AL_NONE), samplerate(0),
        blockAlign(0), memInfo(memData), cb(callbacks)
    { if(cb.open_mem) usrFile = cb.open_mem(memInfo.Data, memInfo.Length); }

    customStream(void *userdata, ALenum fmt, ALuint srate, const UserCallbacks &callbacks)
      : alureStream(NULL), usrFile(userdata), format(fmt), samplerate(srate),
        blockAlign(DetectBlockAlignment(format)), cb(callbacks)
    { }

    virtual ~customStream()
    {
        if(cb.close && usrFile)
            cb.close(usrFile);
        usrFile = NULL;
    }
};


template <typename T>
static alureStream *get_stream_decoder(const T &fdata)
{
    std::map<ALint,UserCallbacks>::iterator i = InstalledCallbacks.begin();
    while(i != InstalledCallbacks.end() && i->first < 0)
    {
        std::auto_ptr<alureStream> stream(new customStream(fdata, i->second));
        if(stream->IsValid()) return stream.release();
        i++;
    }

    std::istream *file = new InStream(fdata);
    if(!file->fail())
    {
        const Decoder::ListType Factories = Decoder::GetList();
        Decoder::ListType::const_reverse_iterator factory = Factories.rbegin();
        Decoder::ListType::const_reverse_iterator end = Factories.rend();
        while(factory != end)
        {
            file->clear();
            file->seekg(0, std::ios_base::beg);

            std::auto_ptr<alureStream> stream(factory->second(file));
            if(stream.get() != NULL) return stream.release();

            factory++;
        }

        SetError("Unsupported type");
        delete file;
    }
    else
    {
        SetError("Failed to open file");
        delete file;
    }

    while(i != InstalledCallbacks.end())
    {
        std::auto_ptr<alureStream> stream(new customStream(fdata, i->second));
        if(stream->IsValid()) return stream.release();
        i++;
    }

    return NULL;
}

alureStream *create_stream(const char *fname)
{ return get_stream_decoder(fname); }
alureStream *create_stream(const MemDataInfo &memData)
{ return get_stream_decoder(memData); }

alureStream *create_stream(ALvoid *userdata, ALenum format, ALuint rate, const UserCallbacks &cb)
{ return new customStream(userdata, format, rate, cb); }
