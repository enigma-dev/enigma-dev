#include "timer.hpp"
#include "utils.hpp"

#if _WIN32
#include <Windows.h>
#endif

#include <ctime>
#include <chrono>

namespace uvpx
{
#ifdef _WIN32
    LARGE_INTEGER s_timerFrequency;
#endif

    Timer::Timer() :
        m_active(false),
        m_pauseDuration(0.0)
    {
#if _WIN32
        QueryPerformanceFrequency(&s_timerFrequency);
#endif
    }

    Timer::~Timer()
    {
    }

    void  Timer::start()
    {
        now(m_start);

        m_active = true;
        m_pauseDuration = 0.0;
    }

    void  Timer::stop()
    {
        m_active = false;
    }

    void  Timer::pause()
    {
        now(m_pauseStart);
    }

    void  Timer::resume()
    {
        TimePoint end;
        now(end);

        m_pauseDuration += secondsElapsed(m_pauseStart, end);
    }

    double Timer::elapsedSeconds()
    {
        if (!m_active)
            return 0.0;

        TimePoint end;
        now(end);

        return secondsElapsed(m_start, end) - m_pauseDuration;
    }

    void Timer::now(TimePoint & tp)
    {
#if _WIN32
        QueryPerformanceCounter(&tp);
#else
        tp = std::chrono::steady_clock::now();
#endif
    }

    double Timer::secondsElapsed(const TimePoint& start, const TimePoint& end)
    {
#if _WIN32
        return (end.QuadPart - start.QuadPart) / (double)s_timerFrequency.QuadPart;
#else
        return ((end - start).count()) * std::chrono::steady_clock::period::num / static_cast<double>(std::chrono::steady_clock::period::den);
#endif
    }
}
