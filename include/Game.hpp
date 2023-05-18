#ifndef GAME_HPP
#define GAME_HPP

#include "stdafx.h"

#include "Network.hpp"
#include "Render.hpp"
#include "World.hpp"
#include "ConfigManager.hpp"
#include "Authentification.hpp"

class Render;
class World;
class Network;
class Authentification;

class Game
{
protected:
    Game();

    static Game* singleton_;

public:

    std::unique_ptr<Render> m_render;
    std::unique_ptr<Network> m_network;
    std::unique_ptr<World> m_currentWorld;
    std::unique_ptr<ConfigManager> m_configManager;
    std::unique_ptr<Authentification> m_auth;


    bool m_isRunning = false;

    Game(Game &other) = delete;
    void operator=(const Game &) = delete;

    static auto GetInstance() -> Game*;
    static auto Run() -> void;

    auto PostInit() -> void;

    ~Game();
};

#endif