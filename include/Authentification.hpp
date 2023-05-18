#ifndef AUTHENTIFICATION_HPP
#define AUTHENTIFICATION_HPP

#include "stdafx.h"

#include "Game.hpp"

struct SXstsResponse {
    std::string token;
    std::string uhs;
};

class Authentification
{
private:

    std::string m_accesstoken;

    std::string m_appId = "c6cd7b0f-077d-4fcf-ab5c-9659576e38cb";
    std::string m_appSecret = "vI87Q~GkhVHJSLN5WKBbEKbK0TJc9YRDyOYc5";

    auto PostData(std::string url, std::string contentType, std::string postData) -> std::string;

    auto LiveAuth(std::string authCode, bool refresh) -> std::string;
    auto XBoxAuth(std::string accessToken) -> std::string;
    auto XStsAuth(std::string xblToken) -> SXstsResponse;
    auto MinecraftAuth(std::string xblToken, std::string uhs) -> std::string;
    auto GetAccountProfile() -> void;

public:

    std::string m_uuid;
    std::string m_username;

    Authentification();
    ~Authentification();

    auto LoginWithMicrosoft() -> void;

    auto JoinServer(std::string hash) -> void;
};


#endif