#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include "stdafx.h"

class ConfigManager
{
private:
    
public:

    std::string refreshToken;

    ConfigManager();
    ~ConfigManager();

    auto ReadConfigFile() -> void;
    auto WriteConfigFile() -> void;
};


#endif