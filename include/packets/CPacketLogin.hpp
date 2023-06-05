#ifndef CPACKETLOGIN_HPP
#define CPACKETLOGIN_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"

class CPacketLogin : IPacket
{
public:
    auto GetData() -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet) -> void override;

    auto GetPacketId() -> int override;

    int eid;
    bool isHardcore;
    unsigned char gameMode;
    char previousGameMode;
    int dimensionCount;
    std::vector<std::string> dimensionNames;

    std::string dimensionType;
    std::string dimensionName;
    long hashSeed;
    int maxPlayers;
    int viewDistance;
    int simulationDistance;
    bool reducedDebugInfo;
    bool enableRespawnScreen;
    bool isDebug;
    bool isFlat;
    bool hasDeathLocation;
    
};

#endif