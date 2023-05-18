#include "packets/CPacketDisconnect.hpp"

auto CPacketDisconnect::GetData() -> std::vector<unsigned char>
{
    return std::vector<unsigned char>();
}
auto CPacketDisconnect::FromPacketDecoder(PacketDecoder packet) -> void
{
    reason = packet.ReadChat();
}

auto CPacketDisconnect::GetPacketId() -> int
{
    return EPacketNameLoginCB::EncryptionRequest;
}