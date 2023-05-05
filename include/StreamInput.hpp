#ifndef STREAMINPUT_HPP
#define STREAMINPUT_HPP

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Stream.hpp"
#include "Slot.hpp"
#include "Utils.hpp"

class StreamInput : Stream
{
    virtual void ReadData(unsigned char *buffPtr, size_t buffLen) = 0;

public:
    virtual ~StreamInput() = default;
    bool ReadBool();
    signed char ReadByte();
    unsigned char ReadUByte();
    short ReadShort();
    unsigned short ReadUShort();
    int ReadInt();
    long long ReadLong();
    float ReadFloat();
    double ReadDouble();
    std::string ReadString();
    /* Chat ReadChat(); */
    int ReadVarInt();
    long long ReadVarLong();
    std::vector<unsigned char> ReadEntityMetadata();
    SlotDataType ReadSlot();
    std::vector<unsigned char> ReadNbtTag();
    glm::vec3 ReadPosition();
    unsigned char ReadAngle();
    Uuid ReadUuid();
    std::vector<unsigned char> ReadByteArray(size_t arrLength);
};

#endif