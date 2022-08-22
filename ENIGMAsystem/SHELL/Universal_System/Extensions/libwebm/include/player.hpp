#ifndef _UVPX_PLAYER_H_
#define _UVPX_PLAYER_H_

#include "dll_defines.hpp"
#include "error_codes.hpp"
#include "frame.hpp"

namespace uvpx
{
    typedef void(*OnAudioDataDecoded)(void *userPtr, float* values, size_t count);
    typedef void(*OnVideoFinished)();
    typedef void(*DebugLogFuncPtr)(const char *msg);

    class VideoPlayer;

    class UVPX_EXPORT Player
    {
    protected:
        VideoPlayer *m_videoPlayer;

    public:
        struct Config
        {
            const char *fileRoot;
            int decodeThreadsCount;
            int videoDecodeBufferSize;
            int audioDecodeBufferSize;
            int frameBufferCount;

            Config() :
                fileRoot(nullptr),
                decodeThreadsCount(1),
                videoDecodeBufferSize(2 * 1024 * 1024),
                audioDecodeBufferSize(4 * 1024),
                frameBufferCount(4)
            {}
        };

        struct Statistics
        {
            int  framesDecoded;
            int  videoBufferSize;
            int  audioBufferSize;
        };

        struct VideoInfo
        {
            int width;
            int height;
            float duration;
            float frameRate;
            int hasAudio;
            int audioChannels;
            int audioFrequency;
            int audioSamples;
            int decodeThreadsCount;
        };

        struct TextureInfo
        {
            int width;
            int height;
            void * texture;
        };

        enum class LoadResult
        {
            Success = 0,
            FileNotExists,
            FailedParseHeader,
            FailedCreateInstance,
            FailedLoadSegment,
            FailedGetSegmentInfo,
            FailedInitializeVideoDecoder,
            FailedDecodeAudioHeader,
            FailedInitializeAudioDecoder,
            UnsupportedVideoCodec,
            NotInitialized,
            InternalError
        };

    public:

        Player(const Config& cfg);
        ~Player();

        LoadResult  load(const char *fileName, int audioTrack, bool preloadFile);
        bool    update(float dt);

        VideoInfo  info() const;

        Frame *lockRead();
        void unlockRead();

        void    setOnAudioData(OnAudioDataDecoded func, void *userPtr);
        void    setOnVideoFinished(OnVideoFinished func);

        double   playTime();
        float   duration();

        void    play();
        void    pause();
        void    stop();
        bool    isStopped();
        bool    isPaused();
        bool    isPlaying();

        static const Config&  defaultConfig();
        bool    readStats(Statistics *dst);

        static void  setDebugLog(DebugLogFuncPtr func);
    };
}

#endif  // _UVPX_PLAYER_H_
