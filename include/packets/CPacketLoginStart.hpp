#ifndef CPACKETLOGINSTART_HPP
#define CPACKETLOGINSTART_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"


class CPacketLoginStart : IPacket
{
public:

    auto GetData()                                  -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet)    -> void override;

    auto GetPacketId() -> int override;

    std::string username;
    bool hasUUID;
    std::vector<unsigned char> uuid;
};

#endif