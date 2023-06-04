#include "Nbt.hpp"

const int sizes[13] = {0, 1, 2, 4, 8, 4, 8, 1, 1, 1, 0, 4, 8};

Nbt::Nbt(PacketDecoder *packetDecoder)
{
    type = packetDecoder->ReadUByte();
    name = packetDecoder->ReadString();

    unsigned short strLength = packetDecoder->ReadUShort();
    std::vector<unsigned char> buff;
    buff = packetDecoder->ReadByteArray((int)(strLength) + 1);

    name = std::string(buff.begin(), buff.end());
    std::cout << (int)type << std::endl;

    if (type != TAG_Compound)
    {
        size = packetDecoder->ReadInt() * sizes[type];
        std::vector<unsigned char> data = packetDecoder->ReadByteArray(size);
    }
    else
    {
        for (;;)
        {
            std::shared_ptr<Nbt> child = std::make_shared<Nbt>(packetDecoder);

            if (child->type == TAG_End)
                break;

            childs.push_back(child);
        }
    }
    /* std::cout << name << std::endl;
    std::cout << size << std::endl; */
}

Nbt::~Nbt()
{
}