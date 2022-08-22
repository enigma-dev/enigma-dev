#ifndef _UVPX_UTILS_H_
#define _UVPX_UTILS_H_

#define UVPX_THREAD_ERROR_DESC_SIZE         (512)

#include "player.hpp"

namespace uvpx
{
    template< class T > void SafeDelete(T*& pVal)
    {
        if (pVal)
        {
            delete pVal;
            pVal = nullptr;
        }
    }

    template< class T > void SafeDeleteArray(T*& pVal)
    {
        if (pVal)
        {
            delete[] pVal;
            pVal = nullptr;
        }
    }

    int getSystemThreadsCount();

    void setDebugLog(DebugLogFuncPtr func);
    void debugLog(const char *format, ...);
}

#endif  // _UVPX_UTILS_H_
