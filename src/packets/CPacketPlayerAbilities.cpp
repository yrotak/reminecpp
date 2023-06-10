#include "packets/CPacketPlayerAbilities.hpp"

auto CPacketPlayerAbilities::GetData() -> std::vector<unsigned char>
{
    return std::vector<unsigned char>();
}
auto CPacketPlayerAbilities::FromPacketDecoder(PacketDecoder packet) -> void
{
    flags = packet.ReadByte();
    flyingSpeed = packet.ReadFloat();
    fovModifier = packet.ReadFloat();
}

auto CPacketPlayerAbilities::GetPacketId() -> int
{
    return EPacketNamePlayCB::PlayerAbilities;
}