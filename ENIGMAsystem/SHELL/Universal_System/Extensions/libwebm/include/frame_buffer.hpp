#ifndef _UVPX_FRAME_BUFFER_H_
#define _UVPX_FRAME_BUFFER_H_

#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <queue>

#include "thread_safe_queue.hpp"
#include "frame.hpp"
#include "dll_defines.hpp"

namespace uvpx
{
    class VideoPlayer;

    class FrameBuffer
    {
    protected:
        VideoPlayer  *m_parent;

        size_t m_frameCount;
        size_t m_width;
        size_t m_height;

        Frame  *m_readFrame;
        std::mutex m_readLock;
        std::mutex m_updateLock;

        ThreadSafeQueue<Frame*> m_readQueue;
        ThreadSafeQueue<Frame*> m_writeQueue;
        Frame *m_writeFrame;

        double  m_readTime;

    public:

        FrameBuffer(VideoPlayer *parent, size_t width, size_t height, size_t frameCount);
        ~FrameBuffer();

        void reset();

        UVPX_EXPORT Frame *lockRead();
        UVPX_EXPORT void unlockRead();

        Frame *lockWrite(double time);
        void unlockWrite();

        void update(double playTime, double frameTime);
        bool isFull();
    };
}
#endif  // _UVPX_FRAME_BUFFER_H_
