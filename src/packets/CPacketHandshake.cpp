#include "packets/CPacketHandshake.hpp"

auto CPacketHandshake::GetData() -> std::vector<unsigned char>
{
    PacketDecoder packet = PacketDecoder();
    packet.WriteVarInt(GetPacketId());
    packet.WriteVarInt(protocolVersion);
    packet.WriteString(serverAddress);
    packet.WriteUShort(serverPort);
    packet.WriteVarInt(nextState);

    return packet.GetDataLengthed();
}
auto CPacketHandshake::FromPacketDecoder(PacketDecoder packet) -> void
{

}

auto CPacketHandshake::GetPacketId() -> int
{
    return EPacketNameHandshakingCB::Handshake;
}