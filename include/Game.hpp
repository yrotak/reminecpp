#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <iostream>
#include <SDL2/SDL.h>
#include "Render.hpp"

class Render;

class Game
{
protected:
    Game();

    static Game* singleton_;

public:

    std::unique_ptr<Render> m_render;
    bool m_isRunning = false;

    Game(Game &other) = delete;
    void operator=(const Game &) = delete;

    static auto GetInstance() -> Game*;
    static auto Run() -> void;

    auto PostInit() -> void;

    ~Game();
};

#endif