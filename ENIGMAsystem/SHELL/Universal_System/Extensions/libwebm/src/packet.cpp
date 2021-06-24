#include "packet.hpp"

namespace uvpx
{
    Packet::Packet(const mkvparser::Block *block, Packet::Type type, double time) :
        m_block(block),
        m_type(type),
        m_time(time)
    {
    }

    Packet::~Packet()
    {
    }

    Packet::Type  Packet::type() const
    {
        return m_type;
    }

    const mkvparser::Block *Packet::block() const
    {
        return m_block;
    }

    double  Packet::time() const
    {
        return m_time;
    }
}
