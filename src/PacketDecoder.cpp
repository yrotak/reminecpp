#include "PacketDecoder.hpp"

const int MAX_VARINT_LENGTH = 5;

PacketDecoder::PacketDecoder()
{
}
PacketDecoder::PacketDecoder(unsigned char *data, size_t size)
{
    m_data = std::vector<unsigned char>(data, data + size);
}

PacketDecoder::~PacketDecoder()
{
}

auto PacketDecoder::ReadSizeFromSocket(TCPsocket tcpsock, std::shared_ptr<Cryptography> crypto, bool isEncrypted) -> int
{
    unsigned char data[MAX_VARINT_LENGTH] = {0};
    size_t dataLen = 0;
    do
    {
        unsigned char tempdata[1];
        int res = SDLNet_TCP_Recv(tcpsock, tempdata, 1);
        if (res == -1)
            std::cout << SDLNet_GetError() << std::endl;
        std::vector<unsigned char> tempvec(tempdata, tempdata+sizeof(tempdata));
        if(isEncrypted)
            tempvec = crypto->DecryptAES(tempvec);
        memcpy(&data[dataLen], tempvec.data(), 1);
    } while ((data[dataLen++] & 0x80) != 0);

    /* std::cout << "new packet " << std::endl;
        std::copy(
            data,
            data+sizeof(data),
            std::ostream_iterator<int>(std::cout, ", "));
 */
    int readed = 0;
    int result = 0;
    char read;
    do
    {
        read = data[readed];
        int value = (read & 0b01111111);
        result |= (value << (7 * readed));
        readed++;
    } while ((read & 0b10000000) != 0);

    return result;
}

auto PacketDecoder::ReadData(unsigned char *data, size_t size) -> void
{
    memcpy(data, m_data.data() + m_readIndex, size);
    m_lastSize = size;
    m_readIndex += size;
}
auto PacketDecoder::WriteData(unsigned char *data, size_t size) -> void
{
    m_data.insert(m_data.end(),
                  data,
                  data + size);
    m_lastSize = size;
    m_totalSize += size;
}
auto PacketDecoder::GetDataLengthed() -> std::vector<unsigned char>
{
    PacketDecoder p = PacketDecoder();
    p.WriteVarInt(m_totalSize);
    std::vector<unsigned char> v = p.m_data;
    v.insert(v.end(), m_data.begin(), m_data.end());
    return v;
}
auto PacketDecoder::GetSize() -> int
{
    return m_totalSize;
}
auto PacketDecoder::ReadBool() -> bool
{
    unsigned char value;
    ReadData(&value, 1);
    return value != 0;
}

auto PacketDecoder::ReadByte() -> signed char
{
    signed char value;
    ReadData((unsigned char *)&value, 1);
    endswap(value);
    return value;
}

auto PacketDecoder::ReadUByte() -> unsigned char
{
    unsigned char value;
    ReadData(&value, 1);
    endswap(value);
    return value;
}

auto PacketDecoder::ReadShort() -> short
{
    unsigned short value;
    ReadData((unsigned char *)&value, 2);
    endswap(value);
    return value;
}

auto PacketDecoder::ReadUShort() -> unsigned short
{
    unsigned short value;
    ReadData((unsigned char *)&value, 2);
    endswap(value);
    return value;
}

auto PacketDecoder::ReadInt() -> int
{
    int value;
    ReadData((unsigned char *)&value, 4);
    endswap(value);
    return value;
}

auto PacketDecoder::ReadLong() -> long long
{
    long long value;
    ReadData((unsigned char *)&value, 8);
    endswap(value);
    return value;
}

auto PacketDecoder::ReadFloat() -> float
{
    float value;
    ReadData((unsigned char *)&value, 4);
    endswap(value);
    return value;
}

auto PacketDecoder::ReadDouble() -> double
{
    double value;
    ReadData((unsigned char *)&value, 8);
    endswap(value);
    return value;
}

auto PacketDecoder::ReadString() -> std::string
{
    int strLength = ReadVarInt();
    std::vector<unsigned char> buff(strLength + 1);
    ReadData(buff.data(), strLength);
    buff[strLength] = 0;
    std::string str((char *)buff.data());
    return str;
}

auto PacketDecoder::ReadChat() -> Chat
{
    /* std::string str, jsonStr = ReadString();
    nlohmann::json json;
    try {
        json = nlohmann::json::parse(jsonStr);
    } catch (std::exception &e) {
        LOG(WARNING) << "Chat json parsing failed: " << e.what();
        LOG(WARNING) << "Corrupted json: " << jsonStr;
        return "";
    }
    if (json.find("translate") != json.end())
        if (json["translate"].get<std::string>() == "multiplayer.disconnect.kicked")
            return "kicked by operator";
    for (auto &it:json["extra"]) {
        str += it["text"].get<std::string>();
    } */
    Chat str(ReadString());
    return str;
}

auto PacketDecoder::ReadVarInt() -> int
{
    unsigned char data[MAX_VARINT_LENGTH] = {0};
    size_t dataLen = 0;
    do
    {
        ReadData(&data[dataLen], 1);
    } while ((data[dataLen++] & 0x80) != 0);

    int readed = 0;
    int result = 0;
    char read;
    do
    {
        read = data[readed];
        int value = (read & 0b01111111);
        result |= (value << (7 * readed));
        readed++;
    } while ((read & 0b10000000) != 0);

    return result;
}

auto PacketDecoder::ReadVarLong() -> long long
{
    return 0;
}

auto PacketDecoder::ReadEntityMetadata() -> std::vector<unsigned char>
{
    std::cout << "Reading EntityMetadata is not implemented" << std::endl;
    return std::vector<unsigned char>();
}

auto PacketDecoder::ReadSlot() -> SlotDataType
{
    SlotDataType slot;
    slot.BlockId = ReadShort();

    if (slot.BlockId == -1)
        return slot;

    slot.ItemCount = ReadByte();
    slot.ItemDamage = ReadShort();

    if (ReadByte() != 0)
        throw std::runtime_error("Slot data with NBT not supported");

    return slot;
}

auto PacketDecoder::ReadNbtTag() -> std::vector<unsigned char>
{
    std::cout << "Reading NBT is not implemented" << std::endl;
    return std::vector<unsigned char>();
}

auto PacketDecoder::ReadPosition() -> glm::vec3
{
    unsigned long long t = ReadLong();
    int x = t >> 38;
    int y = (t >> 26) & 0xFFF;
    int z = t << 38 >> 38;
    if (x >= pow(2, 25))
    {
        x -= pow(2, 26);
    }
    if (y >= pow(2, 11))
    {
        y -= pow(2, 12);
    }
    if (z >= pow(2, 25))
    {
        z -= pow(2, 26);
    }
    return glm::vec3(x, y, z);
}

auto PacketDecoder::ReadAngle() -> unsigned char
{
    return ReadUByte();
}

auto PacketDecoder::ReadUuid() -> std::vector<unsigned char>
{
    unsigned char buff[16];
    ReadData(buff, 16);
    endswap(buff, 16);
    return std::vector<unsigned char>(buff, buff + 16);
}

auto PacketDecoder::ReadByteArray(size_t arrLength) -> std::vector<unsigned char>
{
    std::vector<unsigned char> buffer(arrLength);
    ReadData(buffer.data(), arrLength);
    return buffer;
}

auto PacketDecoder::WriteBool(bool value) -> void
{
    unsigned char val = value ? 1 : 0;
    endswap(val);
    WriteData(&val, 1);
}

auto PacketDecoder::WriteByte(signed char value) -> void
{
    endswap(value);
    WriteData((unsigned char *)&value, 1);
}

auto PacketDecoder::WriteUByte(unsigned char value) -> void
{
    endswap(value);
    WriteData(&value, 1);
}

auto PacketDecoder::WriteShort(short value) -> void
{
    endswap(value);
    WriteData((unsigned char *)&value, 2);
}

auto PacketDecoder::WriteUShort(unsigned short value) -> void
{
    endswap(value);
    WriteData((unsigned char *)&value, 2);
}

auto PacketDecoder::WriteInt(int value) -> void
{
    endswap(value);
    WriteData((unsigned char *)&value, 4);
}

auto PacketDecoder::WriteLong(long long value) -> void
{
    endswap(value);
    WriteData((unsigned char *)&value, 8);
}

auto PacketDecoder::WriteFloat(float value) -> void
{
    endswap(value);
    WriteData((unsigned char *)&value, 4);
}

auto PacketDecoder::WriteDouble(double value) -> void
{
    endswap(value);
    WriteData((unsigned char *)&value, 8);
}

auto PacketDecoder::WriteString(const std::string &value) -> void
{
    WriteVarInt(value.size());
    WriteData((unsigned char *)value.data(), value.size());
}

auto PacketDecoder::WriteChat(const Chat &value) -> void
{
    WriteString(value.ToJson());
}

auto PacketDecoder::WriteVarInt(int value) -> void
{
    unsigned char buff[5];
    size_t len = 0;
    do
    {
        unsigned char temp = (unsigned char)(value & 0b01111111);
        value >>= 7;
        if (value != 0)
        {
            temp |= 0b10000000;
        }
        buff[len] = temp;
        len++;
    } while (value != 0);
    WriteData(buff, len);
}

auto PacketDecoder::WriteVarLong(long long value) -> void
{
    unsigned char buff[10];
    size_t len = 0;
    do
    {
        unsigned char temp = (unsigned char)(value & 0b01111111);
        value >>= 7;
        if (value != 0)
        {
            temp |= 0b10000000;
        }
        buff[len] = temp;
        len++;
    } while (value != 0);
    WriteData(buff, len);
}

auto PacketDecoder::WriteEntityMetadata(const std::vector<unsigned char> &value) -> void
{
    std::cout << "Used unimplemented WriteEntityMetadata: " << value.size() << std::endl;
}

auto PacketDecoder::WriteSlot(const SlotDataType &value) -> void
{
    WriteShort(value.BlockId);
    if (value.BlockId == -1)
        return;
    WriteByte(value.ItemCount);
    WriteShort(value.ItemDamage);
    WriteByte(0);
}

auto PacketDecoder::WriteNbtTag(const std::vector<unsigned char> &value) -> void
{
    std::cout << "Used unimplemented WriteNbtTag " << value.size() << std::endl;
}

auto PacketDecoder::WritePosition(const glm::vec3 value) -> void
{
    unsigned long long pos = (((int)value.x & 0x3FFFFFF) << 38) | (((int)value.y & 0xFFF) << 26) | ((int)value.z & 0x3FFFFFF);
    WriteLong(pos);
}

auto PacketDecoder::WriteAngle(unsigned char value) -> void
{
    WriteUByte(value);
}

auto PacketDecoder::WriteUuid(const std::vector<unsigned char> &value) -> void
{
    WriteByteArray(value);
}

auto PacketDecoder::WriteByteArray(const std::vector<unsigned char> &value) -> void
{
    auto &val = const_cast<std::vector<unsigned char> &>(value);
    WriteData(val.data(), val.size());
}