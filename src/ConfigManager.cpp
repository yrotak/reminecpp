#include "ConfigManager.hpp"

using nlohmann::json;

ConfigManager::ConfigManager()
{

    std::ifstream configfile("config.json");
    bool doFileExist = configfile.good();
    configfile.close();
    if (!doFileExist)
    {
        std::cout << "Config file doesn't exist, creating one." << std::endl;
        WriteConfigFile();
    }
}

ConfigManager::~ConfigManager()
{
}

auto ConfigManager::ReadConfigFile() -> void
{
    std::ifstream configfile("config.json");

    std::string data;
    std::string line;
    while (std::getline(configfile, line))
    {
        data+=line;
    }

    json jsondata = json::parse(data);

    refreshToken = jsondata["refreshToken"];
}
auto ConfigManager::WriteConfigFile() -> void
{
    std::ofstream configfile("config.json");
    json jsondata = {
        {"refreshToken", refreshToken},
    };

    configfile << jsondata.dump();

    configfile.close();
}