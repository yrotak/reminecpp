#include "packets/CPacketPluginMessageCB.hpp"

auto CPacketPluginMessageCB::GetData() -> std::vector<unsigned char>
{
    return std::vector<unsigned char>();
}
auto CPacketPluginMessageCB::FromPacketDecoder(PacketDecoder packet) -> void
{
    channel = packet.ReadString();

    data = packet.ReadByteArray(packet.m_data.size()-channel.size());  
}

auto CPacketPluginMessageCB::GetPacketId() -> int
{
    return EPacketNamePlayCB::PluginMessageCB;
}