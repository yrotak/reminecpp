#include "Nbt.hpp"

const int sizes[13] = {0, 1, 2, 4, 8, 4, 8, 1, 1, 1, 0, 4, 8};
const char *tagNames[13] = {"TAG_End", "TAG_Byte", "TAG_Short", "TAG_Int", "TAG_Long", "TAG_Float", "TAG_Double", "TAG_Byte_Array",
                            "TAG_String", "TAG_List", "TAG_Compound", "TAG_Int_Array", "TAG_Long_Array"};

Nbt::Nbt(PacketDecoder *packetDecoder, std::optional<std::string> parrentName)
{
    type = packetDecoder->ReadUByte();

    if (type != TAG_End)
    {
        unsigned short strLength = packetDecoder->ReadUShort();

        if (strLength > 0)
        {
            std::vector<unsigned char> buff;
            buff = packetDecoder->ReadByteArray((int)(strLength));

            name = std::string(buff.begin(), buff.end());
        }
        else
        {
            name = "None";
        }
    }

    std::ofstream f("test.txt", std::ios_base::app);
    f << std::hex << tagNames[type] << "(" << name << ") : ";

    if(type == TAG_End) {
        f << parrentName.value() << std::endl;
        f.close();
    }

    if (type == TAG_String)
    {
        size = (int)packetDecoder->ReadUShort();
        std::vector<unsigned char> data = packetDecoder->ReadByteArray(size);
        f << std::string(data.begin(), data.end()) << std::endl;
        f.close();
    }

    if (type != TAG_Compound && type != TAG_End && type != TAG_List && type != TAG_String)
    {
        size = packetDecoder->ReadInt() * sizes[type];
        std::vector<unsigned char> data = packetDecoder->ReadByteArray(size);

        std::copy(
            data.begin(),
            data.end(),
            std::ostream_iterator<int>(f, ", "));
        f.close();
    }
    else if (type == TAG_Compound || type == TAG_List)
    {
        f << std::endl;
        f.close();
        for (;;)
        {
            std::shared_ptr<Nbt> child = std::make_shared<Nbt>(packetDecoder, std::make_optional(name));

            if (child->type == TAG_End)
                break;

            childs.push_back(child);
        }
    }
    std::cout << packetDecoder->m_readIndex << std::endl;
    /* std::cout << name << std::endl;
    std::cout << size << std::endl; */
}

Nbt::~Nbt()
{
}