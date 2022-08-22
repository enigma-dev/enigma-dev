#include "file_reader.hpp"
#include "utils.hpp"

#ifdef _WIN32
#include <clocale>
#include "strings_util.h"
#endif

#include <algorithm>
#include <cstring>

namespace uvpx
{
    FileReader::FileReader() :
        m_length(0),
        m_file(nullptr),
        m_preloaded(false),
        m_data(nullptr)
    {
    }

    FileReader::~FileReader()
    {
        close();
    }

    int FileReader::open(const char *fileName, bool preload)
    {
        if (fileName == nullptr)
            return -1;

        if (m_file)
            return -1;

#ifdef _WIN32
        setlocale(LC_ALL, "");
        std::wstring wstrFileName = strings_util::widen(fileName);
        const errno_t e = _wfopen_s(&m_file, wstrFileName.c_str(), L"rb");

        if (e)
            return -1;  // error
#else
        m_file = fopen(fileName, "rb");

        if (m_file == NULL)
            return -1;
#endif

        bool ret = !getFileSize();

        if (preload)
        {
            m_data = new unsigned char[m_length];
            Read(0, m_length, m_data);
        }

        m_preloaded = preload;

        return ret;
    }

    void FileReader::close()
    {
        SafeDeleteArray<unsigned char>(m_data);

        if (m_file != nullptr)
        {
            fclose(m_file);
            m_file = nullptr;
        }
    }

    int FileReader::Read(long long offset, long len, unsigned char* buffer)
    {
        if (m_file == nullptr)
            return -1;

        if (offset < 0)
            return -1;

        if (len < 0)
            return -1;

        if (len == 0)
            return 0;

        if (offset >= m_length)
            return -1;

        if (!m_preloaded)
        {
#ifdef _WIN32
            const int status = _fseeki64(m_file, offset, SEEK_SET);

            if (status)
                return -1;  // error
#else
            fseek(m_file, offset, SEEK_SET);
#endif

            const size_t size = fread(buffer, 1, len, m_file);

            if (size < size_t(len))
                return -1;  // error
        }
        // file preloaded
        else
        {
            size_t size = len;

            if ((offset + len) > m_length)
                size = m_length - offset;

            memcpy(buffer, m_data + offset, size);

            if (size < size_t(len))
                return -1;  // error
        }

        return 0;  // success
    }

    int FileReader::Length(long long* total, long long* available)
    {
        if (m_file == nullptr)
            return -1;

        if (total)
            *total = m_length;

        if (available)
            *available = m_length;

        return 0;
    }

    bool  FileReader::getFileSize()
    {
        if (m_file == NULL)
            return false;
#ifdef _WIN32
        int status = _fseeki64(m_file, 0L, SEEK_END);

        if (status)
            return false;  // error

        m_length = _ftelli64(m_file);
#else
        fseek(m_file, 0L, SEEK_END);
        m_length = ftell(m_file);
#endif

        if (m_length < 0)
            return false;

#ifdef _WIN32
        status = _fseeki64(m_file, 0L, SEEK_SET);

        if (status)
            return false;  // error
#else
        fseek(m_file, 0L, SEEK_SET);
#endif

        return true;
    }
}
