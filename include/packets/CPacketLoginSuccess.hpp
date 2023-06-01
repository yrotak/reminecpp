#ifndef CPACKETLOGINSUCCESS_HPP
#define CPACKETLOGINSUCCESS_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"


struct SLoginSuccessProperty {
    std::string Name;
    std::string Value;
    bool isSigned;
    std::string Signature;
};

class CPacketLoginSuccess : IPacket
{
public:
    auto GetData() -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet) -> void override;

    auto GetPacketId() -> int override;

    std::vector<unsigned char> uuid;
    std::string username;
    int numOfProperties;

    std::vector<SLoginSuccessProperty> properties;
};

#endif