#ifndef CPACKETCHUNKDATA_HPP
#define CPACKETCHUNKDATA_HPP

#include "IPacket.hpp"
#include "PacketDecoder.hpp"

struct SBlockEntity {
    char packedXZ;
    short Y;
    int type;
    Nbt Data;
};

class CPacketChunkData: IPacket
{
public:
    auto GetData() -> std::vector<unsigned char> override;
    auto FromPacketDecoder(PacketDecoder packet) -> void override;

    auto GetPacketId() -> int override;

    int x;
    int y;

    Nbt heightMaps;
    int dataSize;
    std::vector<unsigned char> data;

    int numberOfBlockEntity;
    std::vector<SBlockEntity> blockEntities;
    
};

#endif