#ifndef _UVPX_PACKET_QUEUE_H_
#define _UVPX_PACKET_QUEUE_H_

#include <queue>
#include "utils.hpp"

namespace uvpx
{
    class Packet;

    class PacketQueue
    {
    protected:
        std::queue<Packet*>   m_queue;

    public:

        PacketQueue() {}
        ~PacketQueue() {}

        void  enqueue(Packet *val)
        {
            m_queue.push(val);
        }

        void  pop()
        {
            m_queue.pop();
        }

        void  destroy()
        {
            while (!m_queue.empty())
                m_queue.pop();
        }

        size_t  size() const
        {
            return m_queue.size();
        }

        bool  empty() const
        {
            return size() == 0;
        }

        Packet  *first()
        {
            if (m_queue.size() > 0)
                return m_queue.front();

            return nullptr;
        }
    };
}
#endif  // _UVPX_PACKET_QUEUE_H_
