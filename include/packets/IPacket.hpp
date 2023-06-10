#ifndef IPACKET_HPP
#define IPACKET_HPP

#include "PacketDecoder.hpp"


enum EPacketNameLoginSB {
    LoginStart = 0x00,
    EncryptionResponse = 0x01,
};
enum EPacketNamePlaySB {
    TeleportConfirm = 0x00,
    TabCompleteSB,
    ChatMessageSB,
    ClientStatus,
    ClientSettings,
    ConfirmTransactionSB,
    EnchantItem,
    ClickWindow,
    ClientInformation = 0x08,
};

enum EPacketNameHandshakingCB {
    Handshake = 0x00,
};
enum EPacketNameLoginCB {
    Disconnect = 0x00,
    EncryptionRequest = 0x01,
    LoginSuccess = 0x02,
    SetCompression = 0x03,
};
enum EPacketNamePlayCB {
    BundleDelimiter = 0x00,
    SpawnEntity,
    SpawnExperienceOrb,
    SpawnPlayer,
    EntityAnimation,
    PluginMessageCB = 0x17,
    ChunkData = 0x24,
    Login = 0x28,
    PlayerAbilities = 0x34,
    SetHeldItem = 0x4D,
};

class IPacket
{
public:
    virtual ~IPacket() = default;
    virtual auto GetData()                                  -> std::vector<unsigned char> = 0;
    virtual auto FromPacketDecoder(PacketDecoder packet)    -> void = 0;
    virtual auto GetPacketId()                              -> int = 0;
};
#endif