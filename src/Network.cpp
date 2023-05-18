#include "Network.hpp"

Network::Network() : m_cryptography(std::make_unique<Cryptography>())
{
    /* if (SDLNet_Init() == -1)
    {
        std::cout << "SDLNet_Init error: " << SDLNet_GetError() << std::endl;
    } */
}

Network::~Network()
{
}

auto Network::ConnectToServer(std::string hostname, int port) -> void
{
    std::cout << "Connecting to " << hostname << std::endl;

    Game* gameInstance = Game::GetInstance();
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
    loginstart.username = gameInstance->m_auth->m_username;
    loginstart.hasUUID = true;
    loginstart.uuid = {0xe4, 0xcc, 0x4a, 0xcc, 0xf3, 0x9f, 0x4b, 0xf5, 0x9d, 0xfe, 0x88, 0x72, 0xaa, 0x84, 0x0a, 0x5d};

    Send(loginstart.GetData());

    while (true)
    {
        std::vector<unsigned char> data = Receive();
        PacketDecoder decoder = PacketDecoder(data.data(), data.size());
        int packetsize = decoder.ReadVarInt();
        int packetid = decoder.ReadVarInt();

        switch (packetid)
        {
        case EPacketNameLoginCB::Disconnect:
        {
            CPacketDisconnect disconnect;
            disconnect.FromPacketDecoder(decoder);

            std::cout << disconnect.reason.ToPlainText() << std::endl;
            break;
        }
        case EPacketNameLoginCB::EncryptionRequest:
        {
            CPacketEncryptionRequest encryptionreq;
            encryptionreq.FromPacketDecoder(decoder);

            m_cryptography->GenerateKey();

            m_cryptography->SetPublicKey(encryptionreq.pubkey);

            std::string hash = m_cryptography->GenerateAuthHash(encryptionreq.serverid);
            gameInstance->m_auth->JoinServer(hash);

            CPacketEncryptionResponse encryptionresp;
            encryptionresp.sharedsecretlength = 128;
            encryptionresp.sharedsecret = m_cryptography->GetEncryptedSharedSecret();
            std::vector<unsigned char> veriftoken = m_cryptography->EncryptRSA(encryptionreq.veriftoken);

            encryptionresp.veriftokenlength = 128;
            encryptionresp.veriftoken = veriftoken;

            Send(encryptionresp.GetData());
            m_isEncrypted = true;
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
}

auto Network::Send(std::vector<unsigned char> data) -> void
{
    SDLNet_TCP_Send(tcpsock, data.data(), data.size());
}
auto Network::Receive() -> std::vector<unsigned char>
{
    char buffer[1024];
    int res = SDLNet_TCP_Recv(tcpsock, buffer, sizeof(buffer));
    if (res == -1)
        std::cout << SDLNet_GetError() << std::endl;
    return std::vector<unsigned char>(buffer, buffer + sizeof(buffer));
}