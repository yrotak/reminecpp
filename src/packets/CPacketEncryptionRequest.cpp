#include "packets/CPacketEncryptionRequest.hpp"

auto CPacketEncryptionRequest::GetData() -> std::vector<unsigned char>
{
    return std::vector<unsigned char>();
}
auto CPacketEncryptionRequest::FromPacketDecoder(PacketDecoder packet) -> void
{
    serverid = packet.ReadString();
    pubkeylength = packet.ReadVarInt();
    pubkey = packet.ReadByteArray(pubkeylength);
    veriftokenlength = packet.ReadVarInt();
    veriftoken = packet.ReadByteArray(veriftokenlength);
}

auto CPacketEncryptionRequest::GetPacketId() -> int
{
    return EPacketNameLoginCB::EncryptionRequest;
}