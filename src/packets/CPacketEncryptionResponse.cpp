#include "packets/CPacketEncryptionResponse.hpp"

auto CPacketEncryptionResponse::GetData() -> std::vector<unsigned char>
{
    PacketDecoder packet = PacketDecoder();

    packet.WriteVarInt(GetPacketId());
    packet.WriteVarInt(sharedsecretlength);
    packet.WriteByteArray(sharedsecret);
    packet.WriteVarInt(veriftokenlength);
    packet.WriteByteArray(veriftoken);
    return packet.GetDataLengthed();
}
auto CPacketEncryptionResponse::FromPacketDecoder(PacketDecoder packet) -> void
{
}

auto CPacketEncryptionResponse::GetPacketId() -> int
{
    return EPacketNameLoginSB::EncryptionResponse;
}