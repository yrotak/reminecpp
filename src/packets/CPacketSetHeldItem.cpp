#include "packets/CPacketSetHeldItem.hpp"

auto CPacketSetHeldItem::GetData() -> std::vector<unsigned char>
{
    return std::vector<unsigned char>();
}
auto CPacketSetHeldItem::FromPacketDecoder(PacketDecoder packet) -> void
{
    slot = packet.ReadByte();
}

auto CPacketSetHeldItem::GetPacketId() -> int
{
    return EPacketNamePlayCB::SetHeldItem;
}