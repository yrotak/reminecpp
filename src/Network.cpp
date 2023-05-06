#include "Network.hpp"

Network::Network(/* args */)
{
    if (SDLNet_Init() == -1)
    {
        std::cout << "SDLNet_Init error: " << SDLNet_GetError() << std::endl;
    }
}

Network::~Network()
{
}

auto Network::ConnectToServer(std::string hostname, int port) -> void
{
    if (SDLNet_ResolveHost(&ip, hostname.c_str(), port) == -1)
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

    CPacketHandshake handshake;

    handshake.protocolVersion = 762;
    handshake.serverAddress = hostname;
    handshake.serverPort = port;
    handshake.nextState = 2;

    Send(handshake.GetData());

    CPacketLoginStart loginstart;
    loginstart.username = "DrayNeur";
    loginstart.hasUUID = true;
    loginstart.uuid = {0xe4, 0xcc, 0x4a, 0xcc, 0xf3, 0x9f, 0x4b, 0xf5, 0x9d, 0xfe, 0x88, 0x72, 0xaa, 0x84, 0x0a, 0x5d};

    Send(loginstart.GetData());

    std::vector<unsigned char> data = Receive();
    PacketDecoder decoder = PacketDecoder(data.data(), data.size());
    int packetsize = decoder.ReadVarInt();
    int packetid = decoder.ReadVarInt();

    switch (packetid)
    {
    case EPacketNameLoginCB::Disconnect:
        /* code */
        break;
    case EPacketNameLoginCB::EncryptionRequest:
    {
        CPacketEncryptionRequest encryptionreq;
        encryptionreq.FromPacketDecoder(decoder);

        std::cout << "Network done" << std::endl;

        break;
    }
    case EPacketNameLoginCB::LoginSuccess:
        /* code */
        break;
    case EPacketNameLoginCB::SetCompression:
        /* code */
        break;
    default:
        break;
    }
}

auto Network::Send(std::vector<unsigned char> data) -> void
{
    SDLNet_TCP_Send(tcpsock, data.data(), data.size());
}
auto Network::Receive() -> std::vector<unsigned char>
{
    std::vector<unsigned char> data;
    SDLNet_TCP_Recv(tcpsock, data.data(), 1024);
    return data;
}