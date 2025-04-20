#ifndef _UVPX_BUFFER_H_
#define _UVPX_BUFFER_H_

#include <cstdio>

namespace uvpx
{
    template<typename T>
    class Buffer
    {
    protected:
        T       *m_data;
        size_t  m_size;

        void  resize(size_t newSize)
        {
            if (m_data != nullptr)
                delete[] m_data;

            m_data = new T[newSize];
            m_size = newSize;
        }

    public:

        Buffer(size_t initialSize) :
            m_data(nullptr),
            m_size(0)
        {
            resize(initialSize);
        }

        ~Buffer()
        {
            if (m_data != nullptr)
                delete[] m_data;
        }

        T *get(size_t size)
        {
            if (size > m_size)
                resize(size);

            return m_data;
        }

        size_t  size() const
        {
            return m_size;
        }
    };
}
#endif  // _UVPX_BUFFER_H_
