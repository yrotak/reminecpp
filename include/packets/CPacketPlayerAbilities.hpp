#ifndef CPACKETPLAYERABILITIES_HPP
#define CPACKETPLAYERABILITIES_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"


class CPacketPlayerAbilities: IPacket
{
public:
    auto GetData() -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet) -> void override;

    auto GetPacketId() -> int override;

    char flags;
    float flyingSpeed;
    float fovModifier;
};

#endif