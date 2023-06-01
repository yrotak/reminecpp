#ifndef CPACKETDISCONNECT_HPP
#define CPACKETDISCONNECT_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"
#include "Chat.hpp"


class CPacketDisconnect : IPacket
{
public:

    auto GetData()                                  -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet)    -> void override;

    auto GetPacketId() -> int override;

    Chat reason;

};

#endif