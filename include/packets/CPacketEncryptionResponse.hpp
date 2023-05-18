#ifndef CPACKETENCRYPTIONRESPONSE_HPP
#define CPACKETENCRYPTIONRESPONSE_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"

class CPacketEncryptionResponse : IPacket
{
public:

    auto GetData()                                  -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet)    -> void override;

    auto GetPacketId() -> int override;

    int sharedsecretlength;
    std::vector<unsigned char> sharedsecret;
    int veriftokenlength;
    std::vector<unsigned char> veriftoken;
};

#endif