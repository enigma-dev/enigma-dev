#include "frame.hpp"
#include "utils.hpp"

#include <cstring>

namespace uvpx
{
    Frame::Frame(size_t width, size_t height) :
        m_width(0), m_height(0),
        m_displayWidth(width), m_displayHeight(height),
        m_time(0.0)
    {
        // vpx size is +64 aligned to 16
        m_width = (((width + 64) - 1) & ~15) + 16;
        m_height = (((height + 64) - 1) & ~15) + 16;

        m_ySize = m_width * m_height;
        m_uvSize = (m_width / 2) * (m_height / 2);

        m_y = new unsigned char[m_ySize];
        m_u = new unsigned char[m_uvSize];
        m_v = new unsigned char[m_uvSize];

        // - initially black
        std::memset(m_y, 0, m_ySize);
        std::memset(m_u, 128, m_uvSize);
        std::memset(m_v, 128, m_uvSize);
    }

    Frame::~Frame()
    {
        SafeDeleteArray<unsigned char>(m_y);
        SafeDeleteArray<unsigned char>(m_u);
        SafeDeleteArray<unsigned char>(m_v);
    }

    unsigned char *Frame::y() const
    {
        return m_y;
    }

    unsigned char *Frame::u() const
    {
        return m_u;
    }

    unsigned char *Frame::v() const
    {
        return m_v;
    }

    size_t Frame::ySize() const
    {
        return m_ySize;
    }

    size_t Frame::uvSize() const
    {
        return m_uvSize;
    }

    size_t Frame::yPitch() const
    {
        return m_width;
    }

    size_t Frame::uvPitch() const
    {
        return m_width / 2;
    }

    size_t Frame::width() const
    {
        return m_width;
    }

    size_t Frame::height() const
    {
        return m_height;
    }

    size_t Frame::displayWidth() const
    {
        return m_displayWidth;
    }

    size_t Frame::displayHeight() const
    {
        return m_displayHeight;
    }

    void  Frame::setTime(double time)
    {
        m_time = time;
    }

    double  Frame::time() const
    {
        return m_time;
    }

    void Frame::copy(Frame *dst)
    {
        std::memcpy(dst->y(), m_y, m_ySize);
        std::memcpy(dst->u(), m_u, m_uvSize);
        std::memcpy(dst->v(), m_v, m_uvSize);
    }
}
