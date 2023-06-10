#ifndef CPACKETSETHELDITEM_HPP
#define CPACKETSETHELDITEM_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"


class CPacketSetHeldItem: IPacket
{
public:
    auto GetData() -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet) -> void override;

    auto GetPacketId() -> int override;

    char slot;
};

#endif