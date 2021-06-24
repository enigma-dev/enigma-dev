#ifndef _UVPX_THREAD_SAFE_QUEUE_H_
#define _UVPX_THREAD_SAFE_QUEUE_H_

#include <queue>
#include <mutex>

namespace uvpx
{
    template<typename T>
    class ThreadSafeQueue
    {
    protected:
        std::queue<T>  m_queue;
        mutable std::mutex  m_mutex;

    public:

        ThreadSafeQueue() {}
        ~ThreadSafeQueue() {}

        void  push(T val)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(val);
        }

        void  pop()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.pop();
        }

        T  front() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.front();
        }

        size_t  size() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.size();
        }
    };
}
#endif  // _UVPX_THREAD_SAFE_QUEUE_H_
