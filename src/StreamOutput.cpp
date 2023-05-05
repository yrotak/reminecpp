#include "StreamOutput.hpp"


void StreamOutput::WriteBool(bool value) {
	unsigned char val = value ? 1 : 0;
	endswap(val);
	WriteData(&val, 1);
}

void StreamOutput::WriteByte(signed char value) {
	endswap(value);
	WriteData((unsigned char *) &value, 1);
}

void StreamOutput::WriteUByte(unsigned char value) {
	endswap(value);
	WriteData(&value, 1);
}

void StreamOutput::WriteShort(short value) {
	endswap(value);
	WriteData((unsigned char *) &value, 2);
}

void StreamOutput::WriteUShort(unsigned short value) {
	endswap(value);
	WriteData((unsigned char *) &value, 2);
}

void StreamOutput::WriteInt(int value) {
	endswap(value);
	WriteData((unsigned char *) &value, 4);
}

void StreamOutput::WriteLong(long long value) {
	endswap(value);
	WriteData((unsigned char *) &value, 8);
}

void StreamOutput::WriteFloat(float value) {
	endswap(value);
	WriteData((unsigned char *) &value, 4);
}

void StreamOutput::WriteDouble(double value) {
	endswap(value);
	WriteData((unsigned char *) &value, 8);
}

void StreamOutput::WriteString(const std::string &value) {
	WriteVarInt(value.size());
	WriteData((unsigned char *) value.data(), value.size());
}

/* void StreamOutput::WriteChat(const Chat &value) {
	WriteString(value.ToJson());
} */

void StreamOutput::WriteVarInt(int value) {
	unsigned char buff[5];
	size_t len = 0;
	do {
		unsigned char temp = (unsigned char) (value & 0b01111111);
		value >>= 7;
		if (value != 0) {
			temp |= 0b10000000;
		}
		buff[len] = temp;
		len++;
	} while (value != 0);
	WriteData(buff, len);
}

void StreamOutput::WriteVarLong(long long value) {
	unsigned char buff[10];
	size_t len = 0;
	do {
		unsigned char temp = (unsigned char) (value & 0b01111111);
		value >>= 7;
		if (value != 0) {
			temp |= 0b10000000;
		}
		buff[len] = temp;
		len++;
	} while (value != 0);
	WriteData(buff, len);
}

void StreamOutput::WriteEntityMetadata(const std::vector<unsigned char> &value) {
    std::cout << "Used unimplemented WriteEntityMetadata: " << value.size() << std::endl;
}

void StreamOutput::WriteSlot(const SlotDataType &value) {
    WriteShort(value.BlockId);
    if (value.BlockId == -1)
        return;
    WriteByte(value.ItemCount);
    WriteShort(value.ItemDamage);
    WriteByte(0);
}

void StreamOutput::WriteNbtTag(const std::vector<unsigned char> &value) {
    std::cout << "Used unimplemented WriteNbtTag " << value.size() << std::endl;

}

void StreamOutput::WritePosition(const glm::vec3 value) {
	unsigned long long pos = (((int)value.x & 0x3FFFFFF) << 38) | (((int)value.y & 0xFFF) << 26) | ((int)value.z & 0x3FFFFFF);
	WriteLong(pos);
}

void StreamOutput::WriteAngle(unsigned char value) {
	WriteUByte(value);
}

void StreamOutput::WriteUuid(const Uuid &value) {
	WriteByteArray(value);
}

void StreamOutput::WriteByteArray(const std::vector<unsigned char> &value) {
    auto& val = const_cast<std::vector<unsigned char>&>(value);
	WriteData(val.data(), val.size());
}