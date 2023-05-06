#ifndef CPACKETHANDSHAKE_HPP
#define CPACKETHANDSHAKE_HPP

#include <iostream>
#include "IPacket.hpp"
#include "PacketDecoder.hpp"

class CPacketHandshake : IPacket
{
public:

    auto GetData()                                  -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet)    -> void override;

    auto GetPacketId() -> int override;

    int protocolVersion;
    std::string serverAddress;
    unsigned short serverPort;
    int nextState;
};

#endif