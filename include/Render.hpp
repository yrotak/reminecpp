#ifndef RENDER_HPP
#define RENDER_HPP

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <memory>
#include "Game.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <chrono>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Block.hpp"
#include "Camera.hpp"

typedef std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> SDLWindowPtr;
class Render
{
private:
    SDLWindowPtr m_sdlWindow;
    SDL_GLContext m_glContext;

    auto Init() -> void;

    int _fpsCount = 0;
    int fps = 0; // this will store the final fps for the last second

    std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();

    void CalculateFrameRate()
    {
        auto currentTime = std::chrono::steady_clock::now();

        const auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
        ++_fpsCount;

        if (elapsedTime > 1000000000)
        {
            lastTime = currentTime;
            fps = _fpsCount;
            _fpsCount = 0;

            std::cout << fps << std::endl;
        }
    }

public:
    std::map<std::string, std::shared_ptr<Texture>> textures;

    std::shared_ptr<Shader> shader;

    Render();
    ~Render();

    auto RenderThread() -> void;
};

#endif