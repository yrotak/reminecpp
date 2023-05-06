#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "packets/CPacketHandshake.hpp"
#include "packets/CPacketLoginStart.hpp"
#include "packets/CPacketEncryptionRequest.hpp"

class Network
{
private:

    IPaddress ip;
    TCPsocket tcpsock;

public:
    Network(/* args */);
    ~Network();

    auto ConnectToServer(std::string hostname, int port) -> void;

    auto Send(std::vector<unsigned char> data) -> void;
    auto Receive() -> std::vector<unsigned char>;
};


#endif