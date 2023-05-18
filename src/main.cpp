#include "Game.hpp"

auto main(void) -> int
{
    /* if (SDL_Init(0) == -1)
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

    CPacketHandshake packet;

    packet.protocolVersion = 762;
    packet.serverAddress = "127.0.0.1";
    packet.serverPort = 25565;
    packet.nextState = 2;

    std::vector<unsigned char> v = packet.GetData();

    int result = SDLNet_TCP_Send(tcpsock, v.data(), v.size());

    CPacketLoginStart packet2;
    packet2.username = "DrayNeur";
    packet2.hasUUID = true;
    packet2.uuid = {0xe4, 0xcc, 0x4a, 0xcc, 0xf3, 0x9f, 0x4b, 0xf5, 0x9d, 0xfe, 0x88, 0x72, 0xaa, 0x84, 0x0a, 0x5d};

    v=packet2.GetData();

    result = SDLNet_TCP_Send(tcpsock, v.data(), v.size());


    while (true)
    {
        unsigned char buffer[1024];
        int a = SDLNet_TCP_Recv(tcpsock, buffer, 1024);
        
        PacketDecoder packetdec = PacketDecoder(buffer, sizeof(buffer));
        int packetsize = packetdec.ReadVarInt();
        int packetid = packetdec.ReadVarInt();
        CPacketEncryptionRequest packet3;
        packet3.FromPacketDecoder(packetdec);

        std::cout << packet3.pubkeylength << std::endl;

        std::cout << std::hex;
        std:copy(packet3.pubkey.begin(), packet3.pubkey.end(), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;
    }
     */
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

    /* SDLNet_TCP_Close(tcpsock); */

    Game::Run();

    return 0;
}