#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "stdafx.h"

#include "packets/CPacketHandshake.hpp"
#include "packets/CPacketLoginStart.hpp"
#include "packets/CPacketEncryptionRequest.hpp"
#include "packets/CPacketEncryptionResponse.hpp"
#include "packets/CPacketDisconnect.hpp"
#include "packets/CPacketLoginSuccess.hpp"
#include "packets/CPacketLogin.hpp"
#include "packets/CPacketPluginMessageCB.hpp"
#include "packets/CPacketClientInformation.hpp"
#include "packets/CPacketSetHeldItem.hpp"
#include "packets/CPacketPlayerAbilities.hpp"
#include "Cryptography.hpp"

#include "Game.hpp"

enum ENetworkState
{
    LoginState = 0,
    PlayState,
};

class Network
{
private:
    IPaddress ip;
    TCPsocket tcpsock;

    int m_state = ENetworkState::LoginState;
    bool m_isEncrypted = false;
    bool m_isCompressed = false;

    auto HandleLogin(int packetid, PacketDecoder packetDecoder) -> void;
    auto HandlePlay(int packetid, PacketDecoder packetDecoder) -> void;

public:
    std::shared_ptr<Cryptography> m_cryptography;
    Network();
    ~Network();

    auto ConnectToServer(std::string hostname, int port) -> void;

    auto Send(std::vector<unsigned char> data) -> void;
    auto Receive() -> std::optional<std::vector<unsigned char>>;
};

#endif