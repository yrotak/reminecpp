#include "Render.hpp"

Render::Render() : m_sdlWindow(nullptr, SDL_DestroyWindow)
{
    std::cout << "Rendering engine" << std::endl;
    this->Init();
}

Render::~Render()
{
    SDL_GL_DeleteContext(this->m_glContext);
    SDL_DestroyWindow(this->m_sdlWindow.get());
    SDL_Quit();
}

auto Render::Init() -> void
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        exit(-1);
    }

#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char *glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    const char *glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    this->m_sdlWindow.reset(SDL_CreateWindow("ReMineCPP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags));
    this->m_glContext = SDL_GL_CreateContext(this->m_sdlWindow.get());

    SDL_GL_MakeCurrent(this->m_sdlWindow.get(), this->m_glContext);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Set VSYNC swap interval
    SDL_GL_SetSwapInterval(1);

    // glEnable( GL_MULTISAMPLE );
    //  glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    std::cout << "Inits" << std::endl;

    glewInit();

    shader = std::make_shared<Shader>("../shader/vertex.glsl", "../shader/fragment.glsl");

    textures["minecraft:grass"] = std::make_shared<Texture>("../texture/missing.jpg", GL_TEXTURE_2D);
}

auto Render::RenderThread() -> void
{
    std::cout << "RenderThread" << std::endl;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            for (int k = 0; k < 16; k++)
            {
                Game::GetInstance()->m_currentWorld->setBlock(glm::vec3(i, k, j), "minecraft:grass");
            }
        }
    }
/* 
    Camera *camera = new Camera(); */

    for (const auto &[pos, block] : Game::GetInstance()->m_currentWorld->getBlocks())
    {
        block->Update();
    }

    while (Game::GetInstance()->m_isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                 event.window.windowID == SDL_GetWindowID(this->m_sdlWindow.get())))
                delete Game::GetInstance();
            /* camera->HandleMouse(event);
            if (event.type == SDL_KEYDOWN)
            {
                camera->MoveCamera(event.key.keysym.scancode);
            } */
        }
        glClearColor(0.f, 0.f, 0.f, 1.f);

        int w = 0, h = 0;
        SDL_GetWindowSize(this->m_sdlWindow.get(), &w, &h);
        /* camera->UpdateProjection(w, h); */
        /*
        std::cout << w << " " << h << std::endl;
        glViewport(0, 0, (int)w, (int)h); */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CalculateFrameRate();
        shader->use();

        glm::mat4 view = glm::mat4(0)/* camera->GetViewMatrix() */;
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::mat4(0)/* camera->GetProjectionMatrix() */;
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(w) / static_cast<float>(h), 0.1f, 100.0f);

        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        /*         for (Block *block : blocks)
                    block->Render(); */
        for (const auto &[pos, block] : Game::GetInstance()->m_currentWorld->getBlocks())
        {
            if(block != nullptr)
                block->Render();
        }

        SDL_GL_SwapWindow(this->m_sdlWindow.get());
    }
}