#ifndef MAIN_H
#define MAIN_H

#include "AL/alure.h"
#include "alext.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_WINDOWS_H

#include <windows.h>

#else

#include <assert.h>
#include <pthread.h>
#ifdef HAVE_PTHREAD_NP_H
#include <pthread_np.h>
#endif
#include <errno.h>

typedef pthread_mutex_t CRITICAL_SECTION;
void EnterCriticalSection(CRITICAL_SECTION *cs);
void LeaveCriticalSection(CRITICAL_SECTION *cs);
void InitializeCriticalSection(CRITICAL_SECTION *cs);
void DeleteCriticalSection(CRITICAL_SECTION *cs);

#endif

#include <map>
#include <streambuf>
#include <istream>
#include <list>
#include <algorithm>
#include <vector>
#include <memory>

static const union {
    int val;
    char b[sizeof(int)];
} endian_test = { 1 };
static const bool LittleEndian = (endian_test.b[0] != 0);
static const bool BigEndian = !LittleEndian;


extern PFNALCSETTHREADCONTEXTPROC palcSetThreadContext;
extern PFNALCGETTHREADCONTEXTPROC palcGetThreadContext;
#define alcSetThreadContext palcSetThreadContext
#define alcGetThreadContext palcGetThreadContext

void SetError(const char *err);
ALuint DetectBlockAlignment(ALenum format);
ALuint DetectCompressionRate(ALenum format);
ALenum GetSampleFormat(ALuint channels, ALuint bits, bool isFloat);

struct UserCallbacks {
    void*     (*open_file)(const ALchar*);
    void*     (*open_mem)(const ALubyte*,ALuint);
    ALboolean (*get_fmt)(void*,ALenum*,ALuint*,ALuint*);
    ALuint    (*decode)(void*,ALubyte*,ALuint);
    ALboolean (*rewind)(void*);
    void      (*close)(void*);
};
extern std::map<ALint,UserCallbacks> InstalledCallbacks;


void StopStream(alureStream *stream);
struct alureStream {
    // Local copy of memory data
    ALubyte *data;

    // Storage when reading chunks
    std::vector<ALubyte> dataChunk;

    // Abstracted input stream
    std::istream *fstream;

    virtual bool IsValid() = 0;
    virtual bool GetFormat(ALenum*,ALuint*,ALuint*) = 0;
    virtual ALuint GetData(ALubyte*,ALuint) = 0;
    virtual bool Rewind() = 0;
    virtual bool SetOrder(ALuint order)
    {
        if(!order) return Rewind();
        SetError("Invalid order for stream");
        return false;
    }
    virtual bool SetPatchset(const char*)
    { return true; }
    virtual alureInt64 GetLength()
    { return 0; }

    alureStream(std::istream *_stream)
      : data(NULL), fstream(_stream)
    { StreamList.push_front(this); }
    virtual ~alureStream()
    {
        delete[] data;
        StreamList.erase(std::find(StreamList.begin(), StreamList.end(), this));
    }

    static void Clear(void)
    {
        while(StreamList.size() > 0)
        {
            alureStream *stream = *(StreamList.begin());
            StopStream(stream);
            std::istream *f = stream->fstream;
            delete stream;
            delete f;
        }
    }

    static bool Verify(alureStream *stream)
    {
        ListType::iterator i = std::find(StreamList.begin(), StreamList.end(), stream);
        return (i != StreamList.end());
    }

private:
    typedef std::list<alureStream*> ListType;
    static ListType StreamList;
};


struct UserFuncs {
    bool hasUserdata;
    void *userdata;

    union {
        void* (*open)(const char *filename, ALuint mode);
        void* (*openWithUserdata)(void *userdata, const char *filename, ALuint mode);
    };
    void (*close)(void *f);
    ALsizei (*read)(void *f, ALubyte *buf, ALuint count);
    ALsizei (*write)(void *f, const ALubyte *buf, ALuint count);
    alureInt64 (*seek)(void *f, alureInt64 offset, int whence);
};
extern UserFuncs Funcs;
extern bool UsingSTDIO;


struct MemDataInfo {
    const ALubyte *Data;
    size_t Length;
    size_t Pos;

    MemDataInfo() : Data(NULL), Length(0), Pos(0)
    { }
};

class InStream : public std::istream {
public:
    InStream(const char *filename);
    InStream(const MemDataInfo &memInfo);
    virtual ~InStream();
};


static inline ALuint read_le32(std::istream *file)
{
    ALubyte buffer[4];
    if(!file->read(reinterpret_cast<char*>(buffer), 4)) return 0;
    return buffer[0] | (buffer[1]<<8) | (buffer[2]<<16) | (buffer[3]<<24);
}

static inline ALushort read_le16(std::istream *file)
{
    ALubyte buffer[2];
    if(!file->read(reinterpret_cast<char*>(buffer), 2)) return 0;
    return buffer[0] | (buffer[1]<<8);
}

static inline ALuint read_be32(std::istream *file)
{
    ALubyte buffer[4];
    if(!file->read(reinterpret_cast<char*>(buffer), 4)) return 0;
    return (buffer[0]<<24) | (buffer[1]<<16) | (buffer[2]<<8) | buffer[3];
}

static inline ALushort read_be16(std::istream *file)
{
    ALubyte buffer[2];
    if(!file->read(reinterpret_cast<char*>(buffer), 2)) return 0;
    return (buffer[0]<<8) | buffer[1];
}

static inline ALuint read_be80extended(std::istream *file)
{
    ALubyte buffer[10];
    if(!file->read(reinterpret_cast<char*>(buffer), 10)) return 0;
    ALuint mantissa, last = 0;
    ALubyte exp = buffer[1];
    exp = 30 - exp;
    mantissa = (buffer[2]<<24) | (buffer[3]<<16) | (buffer[4]<<8) | buffer[5];
    while (exp--)
    {
        last = mantissa;
        mantissa >>= 1;
    }
    if((last&1)) mantissa++;
    return mantissa;
}


extern CRITICAL_SECTION cs_StreamPlay;

alureStream *create_stream(const char *fname);
alureStream *create_stream(const MemDataInfo &memData);
alureStream *create_stream(ALvoid *userdata, ALenum format, ALuint rate, const UserCallbacks &cb);

template <typename T>
const T& clamp(const T& val, const T& min, const T& max)
{ return std::max(std::min(val, max), min); }

template <typename T>
void swap(T &val1, T &val2)
{
    val1 ^= val2;
    val2 ^= val1;
    val1 ^= val2;
}


struct Decoder {
    typedef std::auto_ptr<alureStream>(*FactoryType)(std::istream*);
    typedef std::multimap<ALint,FactoryType> ListType;
    typedef std::pair<ALint,FactoryType> PairType;

    struct FindSecond {
        FactoryType mFactory;
        FindSecond(FactoryType _s) : mFactory(_s)
        { }

        bool operator()(const PairType &entry) const
        { return mFactory == entry.second; }
    };

    static const ListType& GetList();

protected:
    static ListType& AddList(FactoryType func=NULL, ALint prio=0);
};

template<typename T, ALint prio>
struct DecoderDecl : public Decoder {
    DecoderDecl()
    {
        T::Init();
        AddList(Factory, prio);
    }
    ~DecoderDecl()
    {
        ListType &list = AddList();
        list.erase(std::find_if(list.begin(), list.end(), FindSecond(Factory)));
        T::Deinit();
    }

private:
    static std::auto_ptr<alureStream> Factory(std::istream *file)
    {
        std::auto_ptr<alureStream> ret(new T(file));
        if(ret->IsValid()) return ret;
        return std::auto_ptr<alureStream>();
    }
};

#endif // MAIN_H
