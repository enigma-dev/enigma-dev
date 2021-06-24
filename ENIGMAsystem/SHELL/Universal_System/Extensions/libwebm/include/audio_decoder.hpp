#ifndef _UVPX_AUDIO_DECODER_H_
#define _UVPX_AUDIO_DECODER_H_

#include "buffer.hpp"

#include <vorbis/codec.h>

#include "utils.hpp"
#include <cstdint>

namespace uvpx
{
    class VideoPlayer;

    class AudioData
    {
    protected:
        float   *mValues;
        size_t  mSize;

    public:
        AudioData(float *values, size_t size) :
            mValues(values),
            mSize(size)
        {
        }

        ~AudioData()
        {
            delete[] mValues;
        }

        const float *values() { return mValues; }
        size_t  size() { return mSize; }
    };

    class AudioDecoder
    {
    protected:
        vorbis_info         m_vorbisInfo;
        vorbis_comment      m_vorbisComment;
        vorbis_dsp_state    m_vorbisDsp;
        vorbis_block        m_vorbisBlock;
        int64_t             m_packetCount;
        VideoPlayer         *m_parent;
        uint64_t            m_framesDecoded;
        Buffer<float>       *m_buffer;

    public:
        AudioDecoder(VideoPlayer* parent, size_t bufferSize);
        ~AudioDecoder();

        void    init();
        bool    initHeader(unsigned char *data, size_t size);
        bool    postInit();

        void    resetDecode();
        bool    decodeHeader(const unsigned char *data, size_t length);
        int     decode(const unsigned char* aData, size_t aLength,
                    int64_t aOffset, uint64_t aTstampUsecs, uint64_t aTstampDuration,
                    int64_t aDiscardPadding, int32_t* aTotalFrames);
        long    rate();
        int     channels();

        float   decodedTime();
        size_t  bufferSize() const;
    };
}
#endif  // _UVPX_AUDIO_DECODER_H_
