#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "video_player.hpp"
#include "timer.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <ratio>
#include <cstring>

#if _WIN32
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <Windows.h>
#endif

#define _UNUSED(expr) do { (void)(expr); } while (0)

#define UVPX_USE_INTERNAL_TIMER

namespace uvpx
{
    VideoPlayer::VideoPlayer(const Player::Config& cfg) :
        m_initialized(false),
        m_onAudioDataDecoded(nullptr),
        m_onAudioDataDecodedUserPtr(nullptr),
        m_onVideoFinished(nullptr),

        m_reader(nullptr),
        m_cluster(nullptr),

        m_audioDecoder(nullptr),

        m_decodeBuffer(nullptr),
        m_frameBuffer(nullptr),

        m_blockEntry(nullptr),
        m_audioTrackIdx(0),
        m_hasVideo(false),
        m_hasAudio(false),

        m_framesDecoded(0),

        m_playTime(0.0),
        m_threadRunning(false),
        m_state(State::Uninitialized)
    {
        m_config = cfg;

        if (cfg.fileRoot)
        {
            m_fileRoot = cfg.fileRoot;
            if (m_fileRoot.length() > 0 && m_fileRoot[m_fileRoot.length() - 1] != '/')
                m_fileRoot += '/';
        }
        else
        {
            m_fileRoot = "";
        }

        m_packetPool = new uvpx::ObjectPool<Packet>(1024 * 4);

        reset();
    }

    VideoPlayer::~VideoPlayer()
    {
        destroy();
    }

    /**
    * Loads the video file.
    * @param fileName file name relative to the file root
    * @param audioTrack which audio track to select
    */
    Player::LoadResult VideoPlayer::load(const char *fileName, int audioTrack, bool preloadFile)
    {
        reset();

        debugLog("vpx loading: %s", fileName);

        std::string videoFullPath = m_fileRoot + fileName;

        // open file
        m_reader = new FileReader();
        if (m_reader->open(videoFullPath.c_str(), preloadFile))
        {
            debugLog("Failed to open video file: %s", videoFullPath.c_str());
            return Player::LoadResult::FileNotExists;
        }

        long long pos = 0;

        long long ret = m_header.Parse(m_reader, pos);
        if (ret < 0)
        {
            debugLog("EBMLHeader::Parse() failed.");
            return Player::LoadResult::FailedParseHeader;
        }

        seg_t* pSegment_;

        ret = seg_t::CreateInstance(m_reader, pos, pSegment_);
        if (ret)
        {
            debugLog("Segment::CreateInstance() failed.");
            return Player::LoadResult::FailedCreateInstance;
        }

        m_segment.reset(pSegment_);

        ret = m_segment->Load();
        if (ret < 0)
        {
            debugLog("Segment::Load() failed.");
            return Player::LoadResult::FailedLoadSegment;
        }

        const mkvparser::SegmentInfo* const pSegmentInfo = m_segment->GetInfo();
        if (pSegmentInfo == NULL)
        {
            debugLog("Segment::GetInfo() failed.");
            return Player::LoadResult::FailedGetSegmentInfo;
        }

        m_info.duration = (float)(pSegmentInfo->GetDuration() / NS_PER_S);
        m_tracks = m_segment->GetTracks();

        unsigned long track_num = 0;
        const unsigned long num_tracks = m_tracks->GetTracksCount();
        int currentAudioTrack = 0;

        m_hasVideo = false;
        m_hasAudio = false;

        while (track_num != num_tracks)
        {
            const mkvparser::Track* const pTrack = m_tracks->GetTrackByIndex(track_num++);

            if (pTrack == NULL)
                continue;

            const long trackType = pTrack->GetType();
            const long trackNumber = pTrack->GetNumber();

            if (trackType == mkvparser::Track::kVideo)
            {
                const mkvparser::VideoTrack* const pVideoTrack = static_cast<const mkvparser::VideoTrack*>(pTrack);

                m_info.width = (int)pVideoTrack->GetWidth();
                m_info.height = (int)pVideoTrack->GetHeight();
                m_info.frameRate = 1.0f / (float)(pVideoTrack->GetDefaultDuration() / NS_PER_S);
                m_info.decodeThreadsCount = std::min(m_config.decodeThreadsCount, getSystemThreadsCount());

                // configure codec
                const char *codecId = pVideoTrack->GetCodecId();

                if (!strcmp(codecId, "V_VP8"))
                {
                    m_decoderData.iface = &vpx_codec_vp8_dx_algo;
                }
                else if (!strcmp(codecId, "V_VP9"))
                {
                    m_decoderData.iface = &vpx_codec_vp9_dx_algo;
                }
                else
                {
                    debugLog("Unsupported video codec: %s", codecId);
                    return Player::LoadResult::UnsupportedVideoCodec;
                }

                vpx_codec_dec_cfg_t vpx_config = { 0 };
                vpx_config.w = m_info.width;
                vpx_config.h = m_info.height;
                vpx_config.threads = m_info.decodeThreadsCount;

                // initialize decoder
                if (vpx_codec_dec_init(&m_decoderData.codec, m_decoderData.iface, &vpx_config, m_decoderData.flags))
                {
                    debugLog("Failed to initialize decoder (%s)", vpx_codec_error_detail(&m_decoderData.codec));
                    return Player::LoadResult::FailedInitializeVideoDecoder;
                }

                // alloc framebuffer
                m_frameBuffer = new FrameBuffer(this, m_info.width, m_info.height, std::max(1, m_config.frameBufferCount));

                m_decoderData.initialized = true;
                m_hasVideo = true;

                debugLog("vpx video found!");
            }

            if (trackType == mkvparser::Track::kAudio)
            {
                currentAudioTrack++;

                if (audioTrack == currentAudioTrack - 1)
                {
                    m_audioTrackIdx = trackNumber;

                    const mkvparser::AudioTrack* const pAudioTrack = static_cast<const mkvparser::AudioTrack*>(pTrack);

                    // Don't leak audio decoders
                    SafeDelete<AudioDecoder>(m_audioDecoder);

                    m_audioDecoder = new AudioDecoder(this, m_config.audioDecodeBufferSize);
                    m_audioDecoder->init();

                    // parse Vorbis headers
                    size_t size;
                    unsigned char * data = (unsigned char *)pAudioTrack->GetCodecPrivate(size);

                    if (!m_audioDecoder->initHeader(data, size))
                    {
                        debugLog("Failed to decode audio header");
                        SafeDelete<AudioDecoder>(m_audioDecoder);
                    }
                    else
                    {
                        if (!m_audioDecoder->postInit())
                        {
                            debugLog("Failed to post-init audio decoder");
                            SafeDelete<AudioDecoder>(m_audioDecoder);
                        }
                        else
                        {
                            m_info.audioChannels = m_audioDecoder->channels();
                            m_info.audioFrequency = m_audioDecoder->rate();
                            m_info.audioSamples = (int)((m_info.audioChannels * m_info.audioFrequency) * m_info.duration);

                            m_hasAudio = true;

                            debugLog("vpx audio found!");
                        }
                    }
                }
            }
        }

        m_info.hasAudio = m_hasAudio;

        m_decodeBuffer = new Buffer<unsigned char>(m_config.videoDecodeBufferSize);

        m_state = State::Initialized;
        m_initialized = true;

        return Player::LoadResult::Success;
    }

    /**
    * Destroys the video.
    */
    void VideoPlayer::destroy()
    {
        stopDecodingThread();

        m_videoQueue.destroy();
        m_audioQueue.destroy();

        SafeDelete<Buffer<unsigned char>>(m_decodeBuffer);

        SafeDelete<FileReader>(m_reader);

        SafeDelete<AudioDecoder>(m_audioDecoder);

        if (m_decoderData.initialized)
            vpx_codec_destroy(&m_decoderData.codec);

        SafeDelete<FrameBuffer>(m_frameBuffer);

        SafeDelete<ObjectPool<Packet>>(m_packetPool);
    }

    void VideoPlayer::reset()
    {
        memset(&m_decoderData, 0, sizeof(m_decoderData));
        memset(&m_info, 0, sizeof(m_info));
        memset(m_threadErrorDesc, 0, UVPX_THREAD_ERROR_DESC_SIZE);

        m_hasVideo = false;
        m_hasAudio = false;
    }

    /**
    * Updates the video.
    * Should be called each frame.
    * @param dt frame delta time (in seconds)
    */
    bool VideoPlayer::update(float dt)
    {
        bool callOnFinishEvent = false;

        if (m_state == State::Playing)
            m_playTime = m_playTime + dt;

        m_updateMutex.lock();
        {
            if (m_state == State::Playing)
            {
                // thread error messages
                if (strlen(m_threadErrorDesc) > 0)
                {
                    debugLog(m_threadErrorDesc);
                    m_threadErrorDesc[0] = '\0';
                }

                if (playTime() >= duration())
                {
                    m_state = State::Finished;
                    callOnFinishEvent = true;
                }
            }
        }
        m_updateMutex.unlock();

        if (callOnFinishEvent)
        {
            if (m_onVideoFinished != nullptr)
                m_onVideoFinished();
        }

        return true;
    }

    /**
    * Copies vpx image YUV data into allocated area, which will later be
    * directly copied into Unity textures.
    */
    void  VideoPlayer::updateYUVData(double time)
    {
        if (m_decoderData.img == nullptr)
            return;

        //const int w = m_decoderData.img->d_w;
        const int h = m_decoderData.img->d_h;
        //const int w2 = w / 2;
        const int h2 = h / 2;

        Frame *curYUV = m_frameBuffer->lockWrite(time);

        unsigned char *y = curYUV->y();
        unsigned char *u = curYUV->u();
        unsigned char *v = curYUV->v();

        const unsigned char *src_y = m_decoderData.img->planes[VPX_PLANE_Y];
        const unsigned char *src_u = m_decoderData.img->planes[VPX_PLANE_U];
        const unsigned char *src_v = m_decoderData.img->planes[VPX_PLANE_V];

        const int stride_y = m_decoderData.img->stride[VPX_PLANE_Y];
        const int stride_u = m_decoderData.img->stride[VPX_PLANE_U];
        const int stride_v = m_decoderData.img->stride[VPX_PLANE_V];

        memcpy(y, src_y, stride_y * h);
        memcpy(u, src_u, stride_u * h2);
        memcpy(v, src_v, stride_v * h2);

        m_frameBuffer->unlockWrite();
    }

    Player::VideoInfo  VideoPlayer::info() const
    {
        return m_info;
    }

    FrameBuffer *VideoPlayer::frameBuffer()
    {
        return m_frameBuffer;
    }

    /// Plays the video.
    void  VideoPlayer::play()
    {
        debugLog("playing video...");

        if (!m_initialized)
            return;

        if (m_state == State::Paused)
        {
            debugLog("play: resuming video...");
            m_timer.resume();
            m_state = State::Playing;
        }
        else
        {
            debugLog("play: buffering video...");

            if (!m_threadRunning.load())
                m_thread = startDecodingThread();

            m_state = State::Buffering;
        }
    }

    /// Pauses the video.
    void  VideoPlayer::pause()
    {
        if (!m_initialized)
            return;

        m_state = State::Paused;
        m_timer.pause();
    }

    /// Stops the video.
    void  VideoPlayer::stop()
    {
        if (!m_initialized)
            return;

        stopDecodingThread();

        std::lock_guard<std::mutex> lock(m_updateMutex);
        _UNUSED(lock);

        if (m_state != State::Stopped)
        {
            m_state = State::Stopped;
            m_framesDecoded = 0;

            if (m_audioDecoder)
                m_audioDecoder->resetDecode();

            m_videoQueue.destroy();
            m_audioQueue.destroy();

            m_frameBuffer->reset();

            m_blockEntry = nullptr;
            m_cluster = nullptr;

            m_timer.stop();
            m_playTime = 0.0;

            m_thread = startDecodingThread();
        }
    }

    /// Checks whether the video is paused.
    bool  VideoPlayer::isPaused()
    {
        return m_state == State::Paused;
    }

    /// Checks whether the video is playing.
    bool  VideoPlayer::isPlaying()
    {
        return m_state == State::Playing;
    }

    /// Checks whether the video is stopped.
    bool  VideoPlayer::isStopped()
    {
        return m_state == State::Stopped;
    }

    /// Returns video playing time (in seconds).
    double  VideoPlayer::playTime()
    {
#ifdef UVPX_USE_INTERNAL_TIMER
        return m_timer.elapsedSeconds();
#else
        return m_playTime;
#endif
    }

    /// Returns video duration (in seconds).
    float  VideoPlayer::duration()
    {
        return m_info.duration;
    }

    /**
    * Main decoding thread.
    */
    void  VideoPlayer::decodingThread()
    {
        debugLog("decodingThread: running...");

        m_cluster = m_segment->GetFirst();

        while (m_threadRunning.load())
        {
            if (m_state != State::Playing && m_state != State::Stopped && m_state != State::Buffering)
            {
                std::this_thread::yield();
                continue;
            }

            // try to decode Video packet(s)
            if (m_hasVideo)
            {
                Packet *videoPacket = getPacket(Packet::Type::Video);

                while (videoPacket && !m_frameBuffer->isFull())
                {
                    decodePacket(videoPacket);
                    m_videoQueue.pop();
                    videoPacket = getPacket(Packet::Type::Video);
                }

                m_frameBuffer->update(playTime(), 1.0 / m_info.frameRate);
            }

            // try to decode Audio packet(s)
            if (m_hasAudio)
            {
                // how much audio seconds ahead should be decoded
                const double audioPreloadAdvance = 0.2;
                const double preloadTime = playTime() + audioPreloadAdvance;

                Packet *audioPacket = getPacket(Packet::Type::Audio);
                while (audioPacket && audioPacket->time() <= preloadTime)
                {
                    decodePacket(audioPacket);
                    m_audioQueue.pop();
                    audioPacket = getPacket(Packet::Type::Audio);
                }
            }

            // if we're in buffering state and got there, all is buffered
            if (m_state == State::Buffering)
            {
                m_state = State::Playing;
                m_timer.start();
            }

            //std::this_thread::yield();
        }
    }

    /**
    * Called when some error occurs in main thread.
    */
    void  VideoPlayer::threadError(int errorState, const char* format, ...)
    {
        std::lock_guard<std::mutex> lock(m_updateMutex);
        _UNUSED(lock);

        char buffer[UVPX_THREAD_ERROR_DESC_SIZE];

        va_list arglist;
        va_start(arglist, format);
        vsprintf(buffer, format, arglist);
        va_end(arglist);

        strncpy(m_threadErrorDesc, buffer, UVPX_THREAD_ERROR_DESC_SIZE);
    }

    /**
    * Starts decoding thread.
    * @return decoding thread
    */
    std::thread  VideoPlayer::startDecodingThread()
    {
        m_threadRunning.store(true);
        return std::thread(&VideoPlayer::decodingThread, this);
    }

    /// Stops decoding thread.
    void  VideoPlayer::stopDecodingThread()
    {
        if (m_threadRunning.load())
        {
            m_threadRunning.store(false);
            m_thread.join();
        }
    }

    /**
    * Called from AudioDecoder after PCM data has been decoded.
    * @param values decoded PCM data (-1 <-> 1)
    * @param count PCM data count
    */
    void  VideoPlayer::addAudioData(float *values, size_t count)
    {
        if (m_onAudioDataDecoded != nullptr)
            m_onAudioDataDecoded(m_onAudioDataDecodedUserPtr, values, count);
    }

    /// Sets OnAudioDataDecoded callback
    void  VideoPlayer::setOnAudioData(OnAudioDataDecoded func, void *userPtr)
    {
        m_onAudioDataDecoded = func;
        m_onAudioDataDecodedUserPtr = userPtr;
    }

    /// Sets OnVideoFinished callback
    void  VideoPlayer::setOnVideoFinished(OnVideoFinished func)
    {
        m_onVideoFinished = func;
    }

    /**
    * Returns next packet from stream.
    */
    Packet  *VideoPlayer::demuxPacket()
    {
        if (m_blockEntry && m_blockEntry->EOS())
        {
            return nullptr;
        }
        else if (m_blockEntry == nullptr && m_cluster && !m_cluster->EOS())
        {
            long status = m_cluster->GetFirst(m_blockEntry);
            if (status < 0)
            {
                threadError(-1, "Error parsing first block of cluster");
                return nullptr;
            }
        }

        Packet *ret = nullptr;

        while (m_cluster && m_blockEntry)
        {
            const mkvparser::Block* const pBlock = m_blockEntry->GetBlock();
            const long long trackNum = pBlock->GetTrackNumber();
            const unsigned long tn = static_cast<unsigned long>(trackNum);
            const mkvparser::Track* const pTrack = m_tracks->GetTrackByNumber(tn);
            const long long time_ns = pBlock->GetTime(m_cluster);
            //const long long discard_ns = pBlock->GetDiscardPadding();
            const double time_sec = (double(time_ns) / NS_PER_S);

            if (pTrack == nullptr)
            {
                threadError(-1, "Unknown block track type");
                return ret;
            }
            else if (pBlock)
            {
                switch (pTrack->GetType())
                {
                    case mkvparser::Track::kVideo:
                    {
                        ret = m_packetPool->GetNextWithoutInitializing();
                        ret = new(ret) Packet(pBlock, Packet::Type::Video, time_sec);

                        m_videoQueue.enqueue(ret);
                        break;
                    }

                    case mkvparser::Track::kAudio:
                    {
                        if (trackNum == m_audioTrackIdx)
                        {
                            ret = m_packetPool->GetNextWithoutInitializing();
                            ret = new(ret) Packet(pBlock, Packet::Type::Audio, time_sec);

                            m_audioQueue.enqueue(ret);
                        }
                        break;
                    }
                }
            }

            long status = m_cluster->GetNext(m_blockEntry, m_blockEntry);
            if (status < 0)
            {
                threadError(-1, "Error parsing next block of cluster");
                return ret;
            }

            if (m_blockEntry == nullptr || m_blockEntry->EOS())
            {
                m_cluster = m_segment->GetNext(m_cluster);
            }

            if (ret)
                return ret;
        }

        return ret;
    }

    /**
    * Decodes packet automatically by its type.
    * Packet will be deallocated.
    */
    void  VideoPlayer::decodePacket(Packet *p)
    {
        const mkvparser::Block *pBlock = p->block();
        const int frameCount = pBlock->GetFrameCount();
        const long long discard_padding = pBlock->GetDiscardPadding();

        for (int i = 0; i < frameCount; ++i)
        {
            const mkvparser::Block::Frame& theFrame = pBlock->GetFrame(i);
            const long size = theFrame.len;
            //const long long offset = theFrame.pos;

            unsigned char *data = m_decodeBuffer->get(size);
            theFrame.Read(m_reader, data);

            switch (p->type())
            {
            case Packet::Type::Video:
            {
                vpx_codec_stream_info_t si;
                memset(&si, 0, sizeof(si));
                si.sz = sizeof(si);
                vpx_codec_peek_stream_info(m_decoderData.iface, data, size, &si);

                vpx_codec_err_t e = vpx_codec_decode(&m_decoderData.codec, data, size, NULL, 0);
                if (e)
                {
                    threadError(UVPX_FAILED_TO_DECODE_FRAME, "Failed to decode frame (%s).", vpx_codec_err_to_string(e));
                    return;
                }

                vpx_codec_iter_t  iter = NULL;
                vpx_image *img = nullptr;

                while ((img = vpx_codec_get_frame(&m_decoderData.codec, &iter)))
                {
                    if (img && img->fmt != VPX_IMG_FMT_I420)
                    {
                        threadError(UVPX_UNSUPPORTED_IMAGE_FORMAT, "Unsupported image format: %d", img->fmt);
                        break;
                    }

                    m_decoderData.img = img;

                    updateYUVData(p->time());
                    m_framesDecoded++;
                }

                break;
            }

            case Packet::Type::Audio:
            {
                if (!m_audioDecoder)
                    break;

                int32_t total_frames = 0;
                int aerr = m_audioDecoder->decode(data, size, 0, 0, 0, discard_padding, &total_frames);
                if (aerr)
                {
                    threadError(UVPX_AUDIO_DECODE_ERROR, "Failed to decode audio (%d)", aerr);
                    return;
                }
            }

            default:
                break;
            }
        }

        //SafeDelete<Packet>(p);
        m_packetPool->DeleteWithoutDestroying(p);
    }

    /**
    * Returns current packet of given type.
    * Return nullptr when no packet of given type exists.
    */
    Packet  *VideoPlayer::getPacket(Packet::Type type)
    {
        Packet *p = nullptr;

        do
        {
            switch (type)
            {
            case Packet::Type::Audio:
                p = m_audioQueue.first();
                break;

            case Packet::Type::Video:
                p = m_videoQueue.first();
                break;
            }

            // when packet does not exists, demux new packet
            if (p == nullptr)
            {
                Packet *demux = demuxPacket();
                if (demux == nullptr) // no more packets of given type
                    return nullptr;
            }

        } while (p == nullptr);

        return p;
    }

    /**
    * Reads current playback statistics.
    */
    bool  VideoPlayer::readStats(Player::Statistics *dst)
    {
        if (!m_initialized)
            return false;

        dst->framesDecoded = (int)m_framesDecoded;

        if (m_decodeBuffer)
            dst->videoBufferSize = (int)m_decodeBuffer->size();

        if (m_audioDecoder)
            dst->audioBufferSize = (int)m_audioDecoder->bufferSize();

        return true;
    }
}
