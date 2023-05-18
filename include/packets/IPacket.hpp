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
    CloseWindowSB,
    PluginMessageSB,
    UseEntity,
    KeepAliveSB,
    Player,
    PlayerPosition,
    PlayerPositionAndLookSB,
    PlayerLook,
    VehicleMoveSB,
    SteerBoat,
    CraftRecipeRequest,
    PlayerAbilitiesSB,
    PlayerDigging,
    EntityAction,
    SteerVehicle,
    CraftingBookData,
    ResourcePackStatus,
    AdvancementTab,
    HeldItemChangeSB,
    CreativeInventoryAction,
    UpdateSign,
    AnimationSB,
    Spectate,
    PlayerBlockPlacement,
    UseItem,
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
    SpawnObject = 0x00,
    SpawnExperienceOrb,
    SpawnGlobalEntity,
    SpawnMob,
    SpawnPainting,
    SpawnPlayer,
    AnimationCB,
    Statistics,
    BlockBreakAnimation,
    UpdateBlockEntity,
    BlockAction,
    BlockChange,
    BossBar,
    ServerDifficulty,
    TabCompleteCB,
    ChatMessageCB,
    MultiBlockChange,
    ConfirmTransactionCB,
    CloseWindowCB,
    OpenWindow,
    WindowItems,
    WindowProperty,
    SetSlot,
    SetCooldown,
    PluginMessageCB,
    NamedSoundEffect,
    DisconnectPlay,
    EntityStatus,
    Explosion,
    UnloadChunk,
    ChangeGameState,
    KeepAliveCB,
    ChunkData,
    Effect,
    Particle,
    JoinGame,
    Map,
    EntityCB,
    EntityRelativeMove,
    EntityLookAndRelativeMove,
    EntityLook,
    VehicleMove,
    OpenSignEditor,
    CraftRecipeResponse,
    PlayerAbilitiesCB,
    CombatEvent,
    PlayerListItem,
    PlayerPositionAndLookCB,
    UseBed,
    UnlockRecipes,
    DestroyEntities,
    RemoveEntityEffect,
    ResourcePackSend,
    Respawn,
    EntityHeadLook,
    SelectAdvancementTab,
    WorldBorder,
    Camera,
    HeldItemChangeCB,
    DisplayScoreboard,
    EntityMetadata,
    AttachEntity,
    EntityVelocity,
    EntityEquipment,
    SetExperience,
    UpdateHealth,
    ScoreboardObjective,
    SetPassengers,
    Teams,
    UpdateScore,
    SpawnPosition,
    TimeUpdate,
    Title,
    SoundEffect,
    PlayerListHeaderAndFooter,
    CollectItem,
    EntityTeleport,
    Advancements,
    EntityProperties,
    EntityEffect,
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