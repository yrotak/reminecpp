#include "Game.hpp"

Game *Game::singleton_ = nullptr;

Game::Game() :  m_render(std::make_unique<Render>()),
                m_network(std::make_unique<Network>()),
                m_configManager(std::make_unique<ConfigManager>()),
                m_auth(std::make_unique<Authentification>())
{
    std::cout << "Game constructor" << std::endl;
    this->m_isRunning = true;
}

auto Game::GetInstance() -> Game *
{
    if (singleton_ == nullptr)
    {
        singleton_ = new Game();
    }
    return singleton_;
}

auto Game::Run() -> void
{
    std::cout << "Running game" << std::endl;
    singleton_ = new Game();
    singleton_->PostInit();
}

auto Game::PostInit() -> void
{
    m_currentWorld = std::make_unique<World>();

    this->m_configManager->ReadConfigFile();
    this->m_auth->LoginWithMicrosoft();
    this->m_network->ConnectToServer("127.0.0.1", 25565);
    /* std::thread([this] {
        this->m_network->ConnectToServer("127.0.0.1", 25565);
    }).detach(); */
    this->m_render->RenderThread();
}

Game::~Game()
{
    this->m_isRunning = false;
    std::cout << "deleted" << std::endl;
}
