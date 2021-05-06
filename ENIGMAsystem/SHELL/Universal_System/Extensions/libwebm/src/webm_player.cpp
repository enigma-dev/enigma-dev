/*
*  Copyright (c) 2016 Stefan Sincak. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree.
*/

#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "webm_player.hpp"

namespace wp
{
    // called by libWebmPlayer when new PCM data is available
    void OnAudioData(void *userPtr, float *pcm, size_t count)
    {
        WebmPlayer *player = static_cast<WebmPlayer*>(userPtr);

        player->insertAudioData(pcm, count);
    }

    // called by SDL when audio device wants PCM data
    void sdlAudioCallback(void *userPtr, Uint8 *stream, int len)
    {
        WebmPlayer *player = static_cast<WebmPlayer*>(userPtr);

        player->readAudioData((float*)stream, len / sizeof(float));
    }

    WebmPlayer::WebmPlayer()
    {
    }

    WebmPlayer::~WebmPlayer()
    {
    }

    bool  WebmPlayer::load(const char *fileName)
    {
        if (!loadVideo(fileName))
            return false;

        if (!initSDL())
            return false;

        if (!initAudio())
            return false;

        return true;
    }

    bool WebmPlayer::loadVideo(const char *fileName)
    {
        m_video = new uvpx::Player(uvpx::Player::defaultConfig());

        uvpx::Player::LoadResult res = m_video->load(fileName, 0, false);

        switch (res)
        {
        case uvpx::Player::LoadResult::FileNotExists:
            return false;

        case uvpx::Player::LoadResult::UnsupportedVideoCodec:
            return false;

        case uvpx::Player::LoadResult::NotInitialized:
            return false;

        case uvpx::Player::LoadResult::Success:
            break;

        default:
            return false;
        }

        m_video->setOnAudioData(OnAudioData, this);

        return true;
    }

    bool WebmPlayer::initSDL()
    {
        if (SDL_Init(SDL_INIT_AUDIO) != 0)
        {
            return false;
        }

        return true;
    }

    void  WebmPlayer::destroy()
    {
        if (m_video)
            delete m_video;

        if(m_audioDevice != 0)
        {
            SDL_CloseAudioDevice(m_audioDevice);
            m_audioDevice = 0;
        }

        SDL_Quit();
    }

    bool WebmPlayer::initAudio()
    {
        uvpx::Player::VideoInfo info = m_video->info();

        if (!info.hasAudio)
            return true;

        SDL_AudioSpec wanted_spec, obtained_spec;

        wanted_spec.freq = info.audioFrequency;
        wanted_spec.format = AUDIO_F32;
        wanted_spec.channels = info.audioChannels;
        wanted_spec.silence = 0;
        wanted_spec.samples = 4096;
        wanted_spec.callback = sdlAudioCallback;
        wanted_spec.userdata = this;

        m_audioDevice = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &obtained_spec, 0);
        if (m_audioDevice == 0)
        {
            return false;
        }
        else if (wanted_spec.format != obtained_spec.format)
        {
            return false;
        }

        SDL_PauseAudioDevice(m_audioDevice, 0);

        return true;
    }

    void  WebmPlayer::insertAudioData(float *src, size_t count)
    {
        std::lock_guard<std::mutex> lock(m_audioMutex);

        m_audioBuffer.resize(m_audioBuffer.size() + count);
        memcpy(&m_audioBuffer[m_audioBuffer.size() - count], src, count * sizeof(float));
    }

    bool  WebmPlayer::readAudioData(float *dst, size_t count)
    {
        std::lock_guard<std::mutex> lock(m_audioMutex);

        if (m_audioBuffer.size() < count)
            return false;

        memcpy(dst, &m_audioBuffer[0], count * sizeof(float));

        m_audioBuffer.erase(m_audioBuffer.begin(), m_audioBuffer.begin() + count);

        return true;
    }

    void  WebmPlayer::clearAudioData()
    {
        std::lock_guard<std::mutex> lock(m_audioMutex);
        m_audioBuffer.clear();
    }

    void  WebmPlayer::playback(PlaybackCommand cmd)
    {
        switch (cmd)
        {
        case PlaybackCommand::Play:
            m_video->play();
            SDL_PauseAudioDevice(m_audioDevice, 0);
            break;

        case PlaybackCommand::Pause:
            m_video->pause();
            SDL_PauseAudioDevice(m_audioDevice, 1);
            break;

        case PlaybackCommand::Resume:
            m_video->play();
            SDL_PauseAudioDevice(m_audioDevice, 0);
            break;

        case PlaybackCommand::Stop:
            m_video->stop();
            SDL_PauseAudioDevice(m_audioDevice, 1);
            SDL_ClearQueuedAudio(m_audioDevice);
            clearAudioData();
            break;
        }
    }
}
