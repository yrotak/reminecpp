#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "stdafx.h"

#include "packets/CPacketHandshake.hpp"
#include "packets/CPacketLoginStart.hpp"
#include "packets/CPacketEncryptionRequest.hpp"
#include "packets/CPacketEncryptionResponse.hpp"
#include "packets/CPacketDisconnect.hpp"
#include "Cryptography.hpp"

#include "Game.hpp"

enum ENetworkStatus {
    Login,
    Play
};

class Network
{
private:

    IPaddress ip;
    TCPsocket tcpsock;

    std::unique_ptr<Cryptography> m_cryptography;

    int m_state = 0;
    bool m_isEncrypted = false;

public:
    Network();
    ~Network();

    auto ConnectToServer(std::string hostname, int port) -> void;

    auto Send(std::vector<unsigned char> data) -> void;
    auto Receive() -> std::vector<unsigned char>;
};


#endif