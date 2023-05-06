#include "packets/CPacketLoginStart.hpp"

auto CPacketLoginStart::GetData() -> std::vector<unsigned char>
{
    PacketDecoder packet = PacketDecoder();
    packet.WriteVarInt(GetPacketId());
    packet.WriteString(username);
    packet.WriteBool(hasUUID);
    packet.WriteUuid(uuid);

    return packet.GetDataLengthed();
}
auto CPacketLoginStart::FromPacketDecoder(PacketDecoder packet) -> void
{

}

auto CPacketLoginStart::GetPacketId() -> int
{
    return EPacketNameLoginSB::LoginStart;
}