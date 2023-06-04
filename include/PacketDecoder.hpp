#ifndef PACKET_HPP
#define PACKET_HPP

#include "stdafx.h"

#include "Slot.hpp"
#include "Utils.hpp"
#include "Chat.hpp"
#include "Cryptography.hpp"

#include "Nbt.hpp"


class PacketDecoder
{
private:

    size_t m_totalSize = 0;

    auto ReadData(unsigned char *data, size_t size) -> void;
    auto WriteData(unsigned char *data, size_t size) -> void;

public:
    std::vector<unsigned char> m_data;
    int m_readIndex = 0;

    size_t m_lastSize = 0;

    PacketDecoder();
    PacketDecoder(unsigned char *data, size_t size);
    ~PacketDecoder();
    

    static auto ReadSizeFromSocket(TCPsocket tcpsock, std::shared_ptr<Cryptography> crypto, bool isEncrypted) -> int;

    auto ReadBool()                    -> bool;
    auto ReadByte()                    -> signed char;
    auto ReadUByte()                   -> unsigned char;
    auto ReadShort()                   -> short;
    auto ReadUShort()                  -> unsigned short;
    auto ReadInt()                     -> int;
    auto ReadLong()                    -> long long;
    auto ReadFloat()                   -> float;
    auto ReadDouble()                  -> double;
    auto ReadString()                  -> std::string;
    auto ReadChat()                    -> Chat;
    auto ReadVarInt()                  -> int;
    auto ReadVarLong()                 -> long long;
    auto ReadEntityMetadata()          -> std::vector<unsigned char>;
    auto ReadSlot()                    -> SlotDataType;
    auto ReadNbtTag()                  -> std::vector<unsigned char>;
    auto ReadPosition()                -> glm::vec3;
    auto ReadAngle()                   -> unsigned char;
    auto ReadUuid()                    -> std::vector<unsigned char>;
    auto ReadByteArray(size_t size)    -> std::vector<unsigned char>;

    auto WriteBool(bool value)                                          -> void;
    auto WriteByte(signed char value)                                   -> void;
    auto WriteUByte(unsigned char value)                                -> void;
    auto WriteShort(short value)                                        -> void;
    auto WriteUShort(unsigned short value)                              -> void;
    auto WriteInt(int value)                                            -> void;
    auto WriteLong(long long value)                                     -> void;
    auto WriteDouble(double value)                                      -> void;
    auto WriteFloat(float value)                                        -> void;
    auto WriteString(const std::string &value)                          -> void;
    auto WriteChat(const Chat &value)                                   -> void;
    auto WriteVarInt(int value)                                         -> void;
    auto WriteVarLong(long long value)                                  -> void;
    auto WriteEntityMetadata(const std::vector<unsigned char> &value)   -> void;
    auto WriteSlot(const SlotDataType &value)                           -> void;
    auto WriteNbtTag(const std::vector<unsigned char> &value)           -> void;
    auto WritePosition(const glm::vec3 value)                           -> void;
    auto WriteAngle(unsigned char value)                                -> void;
    auto WriteUuid(const std::vector<unsigned char> &value)             -> void;
    auto WriteByteArray(const std::vector<unsigned char> &value)        -> void;

    auto GetDataLengthed()  -> std::vector<unsigned char>;
    auto GetSize()          -> int;
};

#endif