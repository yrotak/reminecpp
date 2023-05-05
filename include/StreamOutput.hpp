#ifndef STREAMOUTPUT_HPP
#define STREAMOUTPUT_HPP

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Stream.hpp"
#include "Slot.hpp"
#include "Utils.hpp"

class StreamOutput : Stream
{
    virtual void WriteData(unsigned char *buffPtr, size_t buffLen) = 0;

public:
    virtual ~StreamOutput() = default;
    void WriteBool(bool value);
    void WriteByte(signed char value);
    void WriteUByte(unsigned char value);
    void WriteShort(short value);
    void WriteUShort(unsigned short value);
    void WriteInt(int value);
    void WriteLong(long long value);
    void WriteFloat(float value);
    void WriteDouble(double value);
    void WriteString(const std::string &value);
    /* void WriteChat(const Chat &value); */
    void WriteVarInt(int value);
    void WriteVarLong(long long value);
    void WriteEntityMetadata(const std::vector<unsigned char> &value);
    void WriteSlot(const SlotDataType &value);
    void WriteNbtTag(const std::vector<unsigned char> &value);
    void WritePosition(const glm::vec3 value);
    void WriteAngle(unsigned char value);
    void WriteUuid(const Uuid &value);
    void WriteByteArray(const std::vector<unsigned char> &value);
};

#endif