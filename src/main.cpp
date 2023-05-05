#pragma execution_character_set("utf-8")

#include <iostream>
#include "Game.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

auto main(void) -> int
{
    if (SDL_Init(0) == -1)
    {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    if (SDLNet_Init() == -1)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }

    IPaddress ip;
    TCPsocket tcpsock;

    if (SDLNet_ResolveHost(&ip, "127.0.0.1", 25565) == -1)
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    tcpsock = SDLNet_TCP_Open(&ip);
    if (!tcpsock)
    {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }


    unsigned char data[(5*sizeof(__int32_t))+sizeof(unsigned short)+254] = {};
    __int32_t size = (4*sizeof(__int32_t))+sizeof(unsigned short)+254;
    __int32_t packetid = 0;
    __int32_t sizeofstring = 255;
    char address[254] = "127.0.0.1";
    __int32_t protocolver = 762;
    unsigned short port = 25565;
    __int32_t nextstate = 1;
    

    memcpy(data, &size, sizeof(__int32_t));
    memcpy(data, &packetid, sizeof(__int32_t));
    memcpy(data, &sizeofstring, sizeof(__int32_t));
    memcpy(data, &address, sizeof(address));
    memcpy(data, &protocolver, sizeof(__int32_t));
    memcpy(data, &port, sizeof(unsigned short));
    memcpy(data, &nextstate, sizeof(__int32_t));

    int result = SDLNet_TCP_Send(tcpsock, data, sizeof(data));


    while (true)
    {
        char buffer[1024];
        int a = SDLNet_TCP_Recv(tcpsock, buffer, 1024);
        if(a > 0)
            std::cout << buffer << std::endl;
    }
    
/* 
    while (1)
    {
        printf("message: ");
        char message[1024];
        fgets(message, 1024, stdin);
        int len = strlen(message);

        message[len - 1] = '\0';

        if (len)
        {
            int result;

            printf("Sending: %.*s\n", len, message);

            result = SDLNet_TCP_Send(tcpsock, message, len);
            if (result < len)
                printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        }

        if (len == 2 && tolower(message[0]) == 'q')
        {
            break;
        }
    } */

    SDLNet_TCP_Close(tcpsock);

    /* Game::Run(); */

    return 0;
}