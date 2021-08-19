#ifndef _UVPX_VIDEO_H_
#define _UVPX_VIDEO_H_

#include <math.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <chrono>

#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>

#if defined(_WIN32)
#include <webm/mkvreader.hpp>
#else
#include "mkvreader.hpp"
#endif

#include "audio_decoder.hpp"
#include "buffer.hpp"
#include "frame_buffer.hpp"
#include "timer.hpp"

#include "packet.hpp"
#include "packet_queue.hpp"
#include "file_reader.hpp"
#include "object_pool.hpp"
#include "dll_defines.hpp"
#include "error_codes.hpp"

#include "player.hpp"

namespace uvpx
{
    static const double NS_PER_S = 1e9;

    class VideoPlayer
    {
    private:
        typedef mkvparser::Segment seg_t;

        struct VpxData
        {
            vpx_codec_ctx_t     codec;
            vpx_codec_iter_t    iter;
            vpx_image_t         *img;
            vpx_codec_iface_t   *iface;
            vpx_codec_flags_t   flags;
            bool                initialized;
        };

    private:
        bool    m_initialized;

        OnAudioDataDecoded  m_onAudioDataDecoded;
        void    *m_onAudioDataDecodedUserPtr;
        OnVideoFinished     m_onVideoFinished;


        void    reset();
        void    destroy();
        void    updateYUVData(double time);

        // decoding
        VpxData                     m_decoderData;
        FileReader                  *m_reader;
        mkvparser::EBMLHeader       m_header;
        std::unique_ptr<seg_t>      m_segment;
        const mkvparser::Cluster    *m_cluster;
        const mkvparser::Tracks*    m_tracks;
        AudioDecoder                *m_audioDecoder;
        Buffer<unsigned char>       *m_decodeBuffer;
        FrameBuffer                 *m_frameBuffer;
        const mkvparser::BlockEntry *m_blockEntry;
        long                        m_audioTrackIdx;
        bool                        m_hasVideo;
        bool                        m_hasAudio;
        std::string                 m_fileRoot;
        std::atomic<size_t>         m_framesDecoded;
        Timer                       m_timer;

        std::atomic<double>         m_playTime;

        // threading
        std::thread         m_thread;
        std::mutex          m_updateMutex;
        std::atomic<bool>   m_threadRunning;
        char                m_threadErrorDesc[UVPX_THREAD_ERROR_DESC_SIZE];

        std::thread         startDecodingThread();
        void                stopDecodingThread();
        void                decodingThread();
        void                threadError(int errorState, const char* format, ...);

        PacketQueue         m_videoQueue;
        PacketQueue         m_audioQueue;

        Packet  *demuxPacket();
        Packet  *getPacket(Packet::Type type);
        void  decodePacket(Packet *p);

        ObjectPool<Packet>  *m_packetPool;

    public:
        enum class State
        {
            Uninitialized,
            Initialized,
            Buffering,
            Playing,
            Paused,
            Stopped,
            Finished
        };
        std::atomic<State>  m_state;

        Player::VideoInfo   m_info;

        Player::Config  m_config;

    public:

        VideoPlayer(const Player::Config& cfg);
        ~VideoPlayer();

        Player::LoadResult  load(const char *fileName, int audioTrack, bool preloadFile);
        bool    update(float dt);

        Player::VideoInfo  info() const;
        FrameBuffer *frameBuffer();

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

        void    addAudioData(float *values, size_t count);

        static const Player::Config&  defaultConfig();
        bool    readStats(Player::Statistics *dst);
    };
}

#endif  // _UVPX_VIDEO_H_
