#ifndef CPACKETCLIENTINFORMATION_HPP
#define CPACKETCLIENTINFORMATION_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"

class CPacketClientInformation : IPacket
{
public:

    auto GetData()                                  -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet)    -> void override;

    auto GetPacketId() -> int override;

    std::string locale;
    char viewDistance;
    int chatMode;
    bool chatColors;
    unsigned char displayedSkinPart;
    int mainHand;
    bool textFiltering;
    bool serverListings;
};

#endif