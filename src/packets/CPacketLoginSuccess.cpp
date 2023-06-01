#include "packets/CPacketLoginSuccess.hpp"

auto CPacketLoginSuccess::GetData() -> std::vector<unsigned char>
{
    return std::vector<unsigned char>();
}
auto CPacketLoginSuccess::FromPacketDecoder(PacketDecoder packet) -> void
{
    uuid = packet.ReadUuid();
    username = packet.ReadString();
    numOfProperties = packet.ReadVarInt();

    for(int i = 0; i < numOfProperties; i++) {
        SLoginSuccessProperty property;
        property.Name = packet.ReadString();
        property.Value = packet.ReadString();
        property.isSigned = packet.ReadBool();
        if(property.isSigned)
            property.Signature = packet.ReadString();
        properties.push_back(property);
    }
}

auto CPacketLoginSuccess::GetPacketId() -> int
{
    return EPacketNameLoginSB::LoginStart;
}