#include "Authentification.hpp"

using nlohmann::json;

Authentification::Authentification(/* args */)
{
}

Authentification::~Authentification()
{
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

auto Authentification::PostData(std::string url, std::string contentType, std::string postData) -> std::string
{
    CURL *curl = curl_easy_init();
    if (!curl)
        std::cout << "curl error" << std::endl;

    std::string to_return;
    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, std::string("Content-Type: " + contentType).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &to_return);

    auto code = curl_easy_perform(curl);

    if (code != CURLE_OK)
        std::cout << "curl error " << curl_easy_strerror(code) << std::endl;

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);


    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);


    if(response_code == 204) {
        return "";
    }

    return to_return;
}

auto Authentification::LiveAuth(std::string authCode, bool refresh) -> std::string
{
    std::string postdata;
    if (refresh)
    {
        postdata = "client_id=" + m_appId +
                   "&refresh_token=" + authCode +
                   "&grant_type=refresh_token&redirect_uri=http://localhost:1919/login&scope=XboxLive.signin offline_access&client_secret=" +
                   m_appSecret;
    }
    else
    {
        postdata = "client_id=" + m_appId +
                   "&code=" + authCode +
                   "&grant_type=authorization_code&redirect_uri=http://localhost:1919/login&scope=XboxLive.signin offline_access&client_secret=" +
                   m_appSecret;
    }
    auto to_return = PostData(
        "https://login.live.com/oauth20_token.srf",
        "application/x-www-form-urlencoded",
        postdata);

    auto jsonresp = json::parse(to_return);
    Game *gameInstance = Game::GetInstance();
    gameInstance->m_configManager->refreshToken = jsonresp["refresh_token"];
    gameInstance->m_configManager->WriteConfigFile();
    return jsonresp["access_token"];
}
auto Authentification::XBoxAuth(std::string accessToken) -> std::string
{

    json jsondata = {
        {"Properties", {
                           {"AuthMethod", "RPS"},
                           {"SiteName", "user.auth.xboxlive.com"},
                           {"RpsTicket", "d=" + accessToken},
                       }},
        {"RelyingParty", "http://auth.xboxlive.com"},
        {"TokenType", "JWT"},
    };

    auto to_return = PostData(
        "https://user.auth.xboxlive.com/user/authenticate",
        "application/json",

        jsondata.dump());
    auto jsonresp = json::parse(to_return);
    return jsonresp["Token"];
}
auto Authentification::XStsAuth(std::string xblToken) -> SXstsResponse
{
    json jsondata = {
        {"Properties", {
                           {"SandboxId", "RETAIL"},
                           {"UserTokens", json::array({xblToken})},
                       }},
        {"RelyingParty", "rp://api.minecraftservices.com/"},
        {"TokenType", "JWT"},
    };
    auto to_return = PostData(
        "https://xsts.auth.xboxlive.com/xsts/authorize",
        "application/json",

        jsondata.dump());
    auto jsonresp = json::parse(to_return);
    return {jsonresp["Token"], jsonresp["DisplayClaims"]["xui"][0]["uhs"]};
}
auto Authentification::MinecraftAuth(std::string xblToken, std::string uhs) -> std::string
{
    json jsondata = {
        {"identityToken", "XBL3.0 x=" + uhs + ";" + xblToken},
        {"ensureLegacyEnabled", true},
    };
    auto to_return = PostData(
        "https://api.minecraftservices.com/authentication/login_with_xbox",
        "application/json",

        jsondata.dump());

    auto jsonresp = json::parse(to_return);
    return jsonresp["access_token"];
}
auto Authentification::GetAccountProfile() -> void
{
    CURL *curl = curl_easy_init();
    if (!curl)
        std::cout << "curl error" << std::endl;

    std::string to_return;
    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, std::string("Authorization: Bearer " + m_accesstoken).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.minecraftservices.com/minecraft/profile");

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &to_return);

    auto code = curl_easy_perform(curl);

    if (code != CURLE_OK)
        std::cout << "curl error " << curl_easy_strerror(code) << std::endl;

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    auto jsonresp = json::parse(to_return);

    m_uuid = jsonresp["id"];
    m_username = jsonresp["name"];
}

auto Authentification::LoginWithMicrosoft() -> void
{

    Game *gameInstance = Game::GetInstance();

    bool hasRefreshToken = gameInstance->m_configManager->refreshToken.size() > 0;

    std::string code;

    if (!hasRefreshToken)
    {
        std::string url = "https://login.live.com/oauth20_authorize.srf?response_type=code&client_id=" + m_appId + "&redirect_uri=http://localhost:1919/login&scope=XboxLive.signin+offline_access";
#ifdef _WIN32
        ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif __APPLE__
        std::string command = "open \"" + url + "\"";
        system(command.c_str());
#elif __linux
        std::string command = "xdg-open \"" + url + "\"";
        system(command.c_str());
#endif

        TCPsocket server, client;
        IPaddress ip;
        if (SDLNet_ResolveHost(&ip, NULL, 1919) == -1)
        {
            printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
            exit(1);
        }
        server = SDLNet_TCP_Open(&ip);
        if (!server)
        {
            printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
            exit(2);
        }
        bool done = false;
        while (!done)
        {
            client = SDLNet_TCP_Accept(server);
            if (!client)
            {
                SDL_Delay(100);
                continue;
            }

            IPaddress *remoteip;
            remoteip = SDLNet_TCP_GetPeerAddress(client);
            if (!remoteip)
            {
                continue;
            }

            Uint32 ipaddr;
            ipaddr = SDL_SwapBE32(remoteip->host);
            while (1)
            {
                char message[1024];
                std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Login successful</h1></body></html>";

                int len = SDLNet_TCP_Recv(client, message, 1024);
                if (!len)
                {
                    printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                    break;
                }
                std::string request(message, message + len);

                std::size_t code_start = request.find("code=") + 5;
                std::size_t code_end = request.find("HTTP/1.1") - 1;

                code = request.substr(code_start, code_end - code_start);

                SDLNet_TCP_Send(client, response.data(), response.size());
                done = true;
                break;
            }
            SDLNet_TCP_Close(client);
        }
    }

    std::string liveAccessToken = LiveAuth(hasRefreshToken ? gameInstance->m_configManager->refreshToken : code, hasRefreshToken);
    std::string XBoxToken = XBoxAuth(liveAccessToken);
    SXstsResponse XstsToken = XStsAuth(XBoxToken);
    m_accesstoken = MinecraftAuth(XstsToken.token, XstsToken.uhs);
    GetAccountProfile();
}

auto Authentification::JoinServer(std::string hash) -> void
{
    std::cout << "Setting up profile for " << m_username << " " << m_uuid << std::endl;
    json jsondata = {
        {"accessToken", m_accesstoken},
        {"selectedProfile", m_uuid},
        {"serverId", hash},
    };


    PostData(
    "https://sessionserver.mojang.com/session/minecraft/join",
    "application/json",

    jsondata.dump());
}