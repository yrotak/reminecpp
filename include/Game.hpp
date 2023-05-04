#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <iostream>
#include <SDL2/SDL.h>
#include "Render.hpp"
#include "World.hpp"

class Render;
class World;

class Game
{
protected:
    Game();

    static Game* singleton_;

public:

    std::unique_ptr<Render> m_render;
    std::unique_ptr<World> m_currentWorld;


    bool m_isRunning = false;

    Game(Game &other) = delete;
    void operator=(const Game &) = delete;

    static auto GetInstance() -> Game*;
    static auto Run() -> void;

    auto PostInit() -> void;

    ~Game();
};

#endif