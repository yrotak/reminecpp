#ifndef CPACKETENCRYPTIONREQUEST_HPP
#define CPACKETENCRYPTIONREQUEST_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"


class CPacketEncryptionRequest : IPacket
{
public:

    auto GetData()                                  -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet)    -> void override;

    auto GetPacketId() -> int override;

    std::string serverid;
    int pubkeylength;
    std::vector<unsigned char> pubkey;
    int veriftokenlength;
    std::vector<unsigned char> veriftoken;

};

#endif