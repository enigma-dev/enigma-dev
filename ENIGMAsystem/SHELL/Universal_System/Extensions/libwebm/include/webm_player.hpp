/*
*  Copyright (c) 2016 Stefan Sincak. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree.
*/

#ifndef _WEBM_PLAYER_HPP_
#define _WEBM_PLAYER_HPP_

#include <SDL2/SDL.h>
#include <player.hpp>

#include <vector>
#include <mutex>

namespace wp
{
    class WebmPlayer
    {
    public:
        enum PlaybackCommand
        {
            Play,
            Pause,
            Stop,
            Resume
        };

    public:
        WebmPlayer();
        virtual ~WebmPlayer();

        bool  load(const char *fileName);
        void  destroy();

        void  playback(PlaybackCommand cmd);

        void  insertAudioData(float *src, size_t count);
        bool  readAudioData(float *dst, size_t count);
        void  clearAudioData();

    private:
        uvpx::Player  *m_video = nullptr;

        SDL_Window  *m_window = nullptr;
        SDL_AudioDeviceID  m_audioDevice = 0;

        void log(const char *format, ...);
        void printInfo();

        bool loadVideo(const char *fileName);
        bool initSDL();

        bool initAudio();

        std::vector<float> m_audioBuffer;
        std::mutex  m_audioMutex;
    };
}

#endif
