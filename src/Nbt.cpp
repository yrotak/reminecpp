#include "Nbt.hpp"

const int sizes[13] = {0, 1, 2, 4, 8, 4, 8, 1, 1, 1, 0, 4, 8};
const char *tagNames[13] = {"TAG_End", "TAG_Byte", "TAG_Short", "TAG_Int", "TAG_Long", "TAG_Float", "TAG_Double", "TAG_Byte_Array",
                            "TAG_String", "TAG_List", "TAG_Compound", "TAG_Int_Array", "TAG_Long_Array"};

Nbt::Nbt(PacketDecoder *packetDecoder, std::optional<unsigned char> typeChild)
{
    if (typeChild.has_value())
    {

        type = typeChild.value();

        name = "None";
    }
    else
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
    }

    if (type == TAG_String)
    {
        size = (int)packetDecoder->ReadUShort();
        std::vector<unsigned char> data = packetDecoder->ReadByteArray(size);
    }

    if (type != TAG_Compound && type != TAG_End && type != TAG_List && type != TAG_String)
    {
        size = sizes[type];
        data = packetDecoder->ReadByteArray(size);
    }
    else if (type == TAG_Compound || type == TAG_List)
    {
        unsigned char childType;
        int sizeoflist = 100;
        if (type == TAG_List)
        {
            childType = packetDecoder->ReadUByte();
            sizeoflist = packetDecoder->ReadInt();
        }
        for (int i = 0; i < sizeoflist; i++)
        {
            std::shared_ptr<Nbt> child = std::make_shared<Nbt>(packetDecoder,
                                                               type == TAG_List ? std::make_optional(childType) : std::nullopt);

            if (child->type == TAG_End)
                break;

            childs.push_back(child);
        }
    }
}

Nbt::~Nbt()
{
}

auto Nbt::ToString() -> std::string
{
    std::stringstream s;
    s << std::hex << tagNames[type] << "(" << name << ")";
    if (type == TAG_Compound)
    {
        s << " {" << std::endl;
    }
    else if (type == TAG_List)
    {
        s << " (" << listSize << ") "
          << "{" << std::endl;
    }
    else if (type == TAG_End)
    {
        s << "}" << std::endl;
    }
    else
    {
        s << ": ";
        std::copy(
            data.begin(),
            data.end(),
            std::ostream_iterator<int>(s, ", "));
        s << std::endl;
    }
    if(type == TAG_List || type == TAG_Compound) {
        for(std::shared_ptr<Nbt> nbt : childs)
            s << nbt->ToString();
    }

    return s.str();
}