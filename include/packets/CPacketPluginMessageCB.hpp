#ifndef CPACKETPLUGINMESSAGECB_HPP
#define CPACKETPLUGINMESSAGECB_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"


class CPacketPluginMessageCB: IPacket
{
public:
    auto GetData() -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet) -> void override;

    auto GetPacketId() -> int override;

    std::string channel;
    std::vector<unsigned char> data;
};

#endif