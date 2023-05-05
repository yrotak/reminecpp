#include "StreamInput.hpp"


bool StreamInput::ReadBool() {
	unsigned char value;
	ReadData(&value, 1);
	return value != 0;
}

signed char StreamInput::ReadByte() {
	signed char value;
	ReadData((unsigned char *) &value, 1);
	endswap(value);
	return value;
}

unsigned char StreamInput::ReadUByte() {
	unsigned char value;
	ReadData(&value, 1);
	endswap(value);
	return value;
}

short StreamInput::ReadShort() {
	unsigned short value;
	ReadData((unsigned char *) &value, 2);
	endswap(value);
	return value;
}

unsigned short StreamInput::ReadUShort() {
	unsigned short value;
	ReadData((unsigned char *) &value, 2);
	endswap(value);
	return value;
}

int StreamInput::ReadInt() {
	int value;
	ReadData((unsigned char *) &value, 4);
	endswap(value);
	return value;
}

long long StreamInput::ReadLong() {
	long long value;
	ReadData((unsigned char *) &value, 8);
	endswap(value);
	return value;
}

float StreamInput::ReadFloat() {
	float value;
	ReadData((unsigned char *) &value, 4);
	endswap(value);
	return value;
}

double StreamInput::ReadDouble() {
	double value;
	ReadData((unsigned char *) &value, 8);
	endswap(value);
	return value;
}

std::string StreamInput::ReadString() {
	int strLength = ReadVarInt();
	std::vector<unsigned char> buff(strLength + 1);
	ReadData(buff.data(), strLength);
	buff[strLength] = 0;
	std::string str((char *) buff.data());
	return str;
}

/* Chat StreamInput::ReadChat() {
	std::string str, jsonStr = ReadString();
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
	}
    Chat str(ReadString());
	return str;
} */

int StreamInput::ReadVarInt() {
	unsigned char data[MAX_VARINT_LENGTH] = {0};
	size_t dataLen = 0;
	do {
		ReadData(&data[dataLen], 1);
	} while ((data[dataLen++] & 0x80) != 0);

	int readed = 0;
	int result = 0;
	char read;
	do {
		read = data[readed];
		int value = (read & 0b01111111);
		result |= (value << (7 * readed));
		readed++;
	} while ((read & 0b10000000) != 0);

	return result;
}

long long StreamInput::ReadVarLong() {
	return 0;
}

std::vector<unsigned char> StreamInput::ReadEntityMetadata() {
    std::cout << "Reading EntityMetadata is not implemented" << std::endl;
	return std::vector<unsigned char>();
}

SlotDataType StreamInput::ReadSlot() {
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

std::vector<unsigned char> StreamInput::ReadNbtTag() {
    std::cout << "Reading NBT is not implemented" << std::endl;
	return std::vector<unsigned char>();
}

glm::vec3 StreamInput::ReadPosition() {
	unsigned long long t = ReadLong();
	int x = t >> 38;
	int y = (t >> 26) & 0xFFF;
	int z = t << 38 >> 38;
	if (x >= pow(2, 25)) {
		x -= pow(2, 26);
	}
	if (y >= pow(2, 11)) {
		y -= pow(2, 12);
	}
	if (z >= pow(2, 25)) {
		z -= pow(2, 26);
	}
	return glm::vec3(x, y, z);
}

unsigned char StreamInput::ReadAngle() {
	return ReadUByte();
}

Uuid StreamInput::ReadUuid() {
	unsigned char buff[16];
	ReadData(buff, 16);
	endswap(buff, 16);
    return Uuid(buff,buff+16);
}

std::vector<unsigned char> StreamInput::ReadByteArray(size_t arrLength) {
	std::vector<unsigned char> buffer(arrLength);
	ReadData(buffer.data(), arrLength);
	return buffer;

}