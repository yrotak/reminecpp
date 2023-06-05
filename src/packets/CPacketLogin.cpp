#include "packets/CPacketLogin.hpp"

auto CPacketLogin::GetData() -> std::vector<unsigned char>
{
    return std::vector<unsigned char>();
}
auto CPacketLogin::FromPacketDecoder(PacketDecoder packet) -> void
{
    eid = packet.ReadInt();
    isHardcore = packet.ReadBool();
    gameMode = packet.ReadUByte();
    previousGameMode = packet.ReadByte();
    dimensionCount = packet.ReadVarInt();

    for (int i = 0; i < dimensionCount; i++)
    {
        dimensionNames.push_back(packet.ReadString());
    }

    packet.ReadNbtTag();

    dimensionType = packet.ReadString();
    dimensionName = packet.ReadString();

    hashSeed = packet.ReadLong();
    maxPlayers = packet.ReadVarInt();
    viewDistance = packet.ReadVarInt();
    simulationDistance = packet.ReadVarInt();
    reducedDebugInfo = packet.ReadBool();
    enableRespawnScreen = packet.ReadBool();
    isDebug = packet.ReadBool();
    isFlat = packet.ReadBool();
    hasDeathLocation = packet.ReadBool();

}

auto CPacketLogin::GetPacketId() -> int
{
    return EPacketNamePlayCB::Login;
}