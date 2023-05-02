#ifndef RENDER_HPP
#define RENDER_HPP

#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#include <iostream>
#include <memory>
#include "Game.hpp"

typedef std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> SDLWindowPtr;
class Render
{
private:
    SDLWindowPtr m_sdlWindow;
    SDL_GLContext m_glContext;

    auto Init() -> void;

public:
    Render();
    ~Render();

    auto RenderThread() -> void;
};

#endif