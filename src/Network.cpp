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

auto Network::HandleLogin(int packetid, PacketDecoder packetDecoder) -> void
{
    Game *gameInstance = Game::GetInstance();

    switch (packetid)
    {
    case EPacketNameLoginCB::Disconnect:
    {
        CPacketDisconnect disconnect;
        disconnect.FromPacketDecoder(packetDecoder);

        std::cout << "disconnect is compr; " << m_isCompressed << std::endl;

        std::cout << disconnect.reason.ToPlainText() << std::endl;
        break;
    }
    case EPacketNameLoginCB::EncryptionRequest:
    {
        CPacketEncryptionRequest encryptionreq;
        encryptionreq.FromPacketDecoder(packetDecoder);

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
    {
        std::cout << "Logged In" << std::endl;

        CPacketLoginSuccess loginSuccess;
        loginSuccess.FromPacketDecoder(packetDecoder);

        m_state = ENetworkState::PlayState;

        break;
    }
    case EPacketNameLoginCB::SetCompression:
        std::cout << "Compression setup" << std::endl;
        m_isCompressed = false;

        break;
    default:
        break;
    }
}
auto Network::HandlePlay(int packetid, PacketDecoder packetDecoder) -> void
{
    switch (packetid)
    {
    case EPacketNamePlayCB::Login:
    {
        CPacketLogin packetLogin;
        packetLogin.FromPacketDecoder(packetDecoder);

        CPacketClientInformation clientInfos;
        clientInfos.locale = "en_GB";
        clientInfos.viewDistance = (char)10;
        clientInfos.chatMode = 0;
        clientInfos.chatColors = true;
        clientInfos.displayedSkinPart = 0x01 | 0x02 | 0x04 | 0x08 | 0x10 | 0x20 | 0x40;
        clientInfos.mainHand = 1;
        clientInfos.textFiltering = false;
        clientInfos.serverListings = true;

        Send(clientInfos.GetData());
        break;
    }
    case EPacketNamePlayCB::PluginMessageCB:
    {
        CPacketPluginMessageCB packetPluginMessage;
        packetPluginMessage.FromPacketDecoder(packetDecoder);

        break;
    }
    case EPacketNamePlayCB::SetHeldItem:
    {
        CPacketSetHeldItem setHeldItem;
        setHeldItem.FromPacketDecoder(packetDecoder);

        break;
    }
    case EPacketNamePlayCB::PlayerAbilities:
    {
        CPacketPlayerAbilities playerAbilities;
        playerAbilities.FromPacketDecoder(packetDecoder);

        break;
    }
    default:
        std::cout << "packetid " << std::hex << packetid << std::endl;
        break;
    }
}

auto Network::ConnectToServer(std::string hostname, int port) -> void
{
    std::cout << "Connecting to " << hostname << std::endl;

    Game *gameInstance = Game::GetInstance();
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

        /* if (!m_isCompressed) */

        /* int size = decoder.ReadVarInt(); */
        int packetid = decoder.ReadVarInt();

        switch (m_state)
        {
        case ENetworkState::LoginState:
            HandleLogin(packetid, decoder);
            break;
        case ENetworkState::PlayState:
            HandlePlay(packetid, decoder);
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
    int size = PacketDecoder::ReadSizeFromSocket(tcpsock, m_cryptography, m_isEncrypted);

    unsigned char *buffer = new unsigned char[size];
    memset(buffer, 0, size);
    int res = SDLNet_TCP_Recv(tcpsock, buffer, size);
    if (res == -1)
        std::cout << SDLNet_GetError() << std::endl;

    std::vector<unsigned char> data(buffer, buffer + size);

    if (m_isEncrypted)
        data = m_cryptography->DecryptAES(data);

    if (m_isCompressed)
    {
        PacketDecoder packetCompress(data.data(), data.size());
        int packetsize = packetCompress.ReadVarInt();
        int uncrompressedsize = packetCompress.ReadVarInt();

        std::cout << "size last " << packetCompress.m_lastSize << std::endl;
        std::cout << "size " << packetsize - packetCompress.m_lastSize << std::endl;

        std::cout << "packetsize " << packetsize << std::endl;
        std::cout << "uncrompressedsize " << uncrompressedsize << std::endl;

        /* std::cout << "new packet " << std::endl;
        std::copy(
            data.begin(),
            data.end(),
            std::ostream_iterator<int>(std::cout, ", ")); */

        std::vector<unsigned char> compressedData = packetCompress.ReadByteArray(packetsize - packetCompress.m_lastSize);
        /* std::cout << "compressed " << std::endl;
        std::copy(
            compressedData.begin(),
            compressedData.end(),
            std::ostream_iterator<int>(std::cout, ", ")); */
        data = m_cryptography->Decompress(compressedData, uncrompressedsize);
        std::copy(
            data.begin(),
            data.end(),
            std::ostream_iterator<int>(std::cout, ", "));
    }

    return data;
}