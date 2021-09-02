#include "frame_buffer.hpp"
#include "utils.hpp"
#include "video_player.hpp"

#include <cstring>

//#define LIMIT_READ_TO_FRAMERATE

namespace uvpx
{
    FrameBuffer::FrameBuffer(VideoPlayer *parent, size_t width, size_t height, size_t frameCount) :
        m_parent(parent),
        m_frameCount(frameCount),
        m_width(width),
        m_height(height),
        m_writeFrame(nullptr),
        m_readTime(0.0)
    {
        m_readFrame = new Frame(width, height);

        for (size_t i = 0; i < frameCount; i++)
            m_writeQueue.push(new Frame(width, height));
    }

    FrameBuffer::~FrameBuffer()
    {
        SafeDelete<Frame>(m_readFrame);

        while (m_writeQueue.size()>0)
        {
            Frame *frame = m_writeQueue.front();
            SafeDelete<Frame>(frame);
            m_writeQueue.pop();
        }

        while (m_readQueue.size()>0)
        {
            Frame *frame = m_readQueue.front();
            SafeDelete<Frame>(frame);
            m_readQueue.pop();
        }
    }

    Frame *FrameBuffer::lockRead()
    {
        double playTime = m_parent->playTime();
        double frameTime = 1.0 / m_parent->info().frameRate;

#ifdef LIMIT_READ_TO_FRAMERATE
        double diff = playTime - m_readTime;

        if (diff < frameTime)
            return nullptr;

        m_readTime = playTime - (diff - frameTime);
#endif

        update(playTime, frameTime);

        m_readLock.lock();

        return m_readFrame;
    }

    void FrameBuffer::unlockRead()
    {
        m_readLock.unlock();
    }

    Frame *FrameBuffer::lockWrite(double time)
    {
        m_writeFrame = m_writeQueue.front();
        m_writeQueue.pop();

        m_writeFrame->setTime(time);

        return m_writeFrame;
    }

    void FrameBuffer::unlockWrite()
    {
        m_readQueue.push(m_writeFrame);
        m_writeFrame = nullptr;
    }

    void FrameBuffer::reset()
    {
        while (m_readQueue.size() > 0)
        {
            m_writeQueue.push(m_readQueue.front());
            m_readQueue.pop();
        }

        m_writeFrame = nullptr;
        m_readTime = 0.0;
    }

    void FrameBuffer::update(double playTime, double frameTime)
    {
        if (m_readQueue.size() == 0)
            return;

        m_updateLock.lock();

        Frame *front = m_readQueue.front();

        if ((playTime - front->time()) >= frameTime)
        {
            while (m_readQueue.size() > 0)
            {
                m_readQueue.pop();
                m_writeQueue.push(front);

                if (m_readQueue.size() > 0)
                {
                    front = m_readQueue.front();

                    if ((playTime - front->time()) < frameTime)
                    {
                        m_readLock.lock();
                        front->copy(m_readFrame);
                        m_readLock.unlock();

                        break;
                    }
                }
            }
        }

        m_updateLock.unlock();
    }

    bool FrameBuffer::isFull()
    {
        return m_writeQueue.size() == 0;
    }
}
