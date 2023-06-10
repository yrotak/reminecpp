#include "packets/CPacketClientInformation.hpp"

auto CPacketClientInformation::GetData() -> std::vector<unsigned char>
{
    PacketDecoder packet = PacketDecoder();
    packet.WriteVarInt(GetPacketId());
    packet.WriteString(locale);
    packet.WriteByte(viewDistance);
    packet.WriteVarInt(chatMode);
    packet.WriteBool(chatColors);
    packet.WriteUByte(displayedSkinPart);
    packet.WriteVarInt(mainHand);
    packet.WriteBool(textFiltering);
    packet.WriteBool(serverListings);

    return packet.GetDataLengthed();
}
auto CPacketClientInformation::FromPacketDecoder(PacketDecoder packet) -> void
{

}

auto CPacketClientInformation::GetPacketId() -> int
{
    return EPacketNamePlaySB::ClientInformation;
}