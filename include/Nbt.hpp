#ifndef NBT_HPP
#define NBT_HPP

#include "stdafx.h"

#include "PacketDecoder.hpp"

class PacketDecoder;

enum ETagType {
    TAG_End,
    TAG_Byte,
    TAG_Short,
    TAG_Int,
    TAG_Long,
    TAG_Float,
    TAG_Double,
    TAG_Byte_Array,
    TAG_String,
    TAG_List,
    TAG_Compound,
    TAG_Int_Array,
    TAG_Long_Array,
};

class Nbt
{
private:
    std::vector<std::shared_ptr<Nbt>> childs;

    std::string name;
    size_t size;
public:
    unsigned char type;

    Nbt(PacketDecoder* packetDecoder, std::optional<std::string> parrentName = std::nullopt);
    ~Nbt();


};


#endif