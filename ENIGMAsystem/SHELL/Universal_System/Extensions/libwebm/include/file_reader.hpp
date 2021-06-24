#ifndef _UVPX_FILE_READER_H_
#define _UVPX_FILE_READER_H_

#include "mkvreader.hpp"

namespace uvpx
{
    class FileReader : public mkvparser::IMkvReader
    {
    public:
    protected:
        long long m_length;
        FILE* m_file;

        bool  getFileSize();

        // preload
        bool  m_preloaded;
        unsigned char  *m_data;

    public:

        FileReader();
        virtual ~FileReader();

        int open(const char *fileName, bool preload);
        void close();

        virtual int Read(long long position, long length, unsigned char* buffer);
        virtual int Length(long long* total, long long* available);
    };
}
#endif  // _UVPX_FILE_READER_H_
