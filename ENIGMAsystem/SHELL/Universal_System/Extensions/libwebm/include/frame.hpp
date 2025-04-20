#ifndef _UVPX_FRAME_H_
#define _UVPX_FRAME_H_

#include <cstdio>
#include <memory>

#include "dll_defines.hpp"

namespace uvpx
{
    class UVPX_EXPORT Frame
    {
    private:
        unsigned char *m_y;
        unsigned char *m_u;
        unsigned char *m_v;

        size_t  m_ySize;
        size_t  m_uvSize;

        size_t  m_width;
        size_t  m_height;
        size_t  m_displayWidth;
        size_t  m_displayHeight;

        double  m_time;

    public:
        Frame(size_t width, size_t height);
        ~Frame();

        void copy(Frame *dst);

        unsigned char *y() const;
        unsigned char *u() const;
        unsigned char *v() const;

        size_t ySize() const;
        size_t uvSize() const;

        size_t yPitch() const;
        size_t uvPitch() const;

        size_t width() const;
        size_t height() const;
        size_t displayWidth() const;
        size_t displayHeight() const;

        void  setTime(double time);
        double  time() const;
    };
}
#endif  // _UVPX_FRAME_H_
