#include "Block.hpp"

Block::Block(std::string identifier, glm::vec3 position) : m_identifier(identifier), m_position(position)
{
    this->m_texture = Game::GetInstance()->m_render->textures[identifier];
}

Block::~Block()
{
    ReleaseVertices();
}

auto Block::Render() -> void
{
    if (m_isRendered)
    {
        m_texture->bind(0);
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        Game::GetInstance()->m_render->shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

auto Block::Update() -> void
{
    ReleaseVertices();

    std::vector<float> vertices = {};

    std::vector<std::vector<float>> primary = {
        {
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
        },
        {
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
        },
        {
            -0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            -0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
        },
        {
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
        },
        {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f},
        {
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
        }};

    /* float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f}; */
    m_isRendered = false;
    for (int i = 0; i < 6; i++)
    {
        if (shouldSideBeRendered(static_cast<EBlockSide>(i)))
        {
            m_isRendered = true;

            vertices.insert(
                vertices.end(),
                std::make_move_iterator(primary[i].begin()),
                std::make_move_iterator(primary[i].end()));
        }
    }
    if (m_isRendered)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
}

auto Block::ReleaseVertices() -> void
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

auto Block::shouldSideBeRendered(EBlockSide blockSide) -> bool
{
    glm::vec3 checkingSide(0, 0, 0);
    switch (blockSide)
    {
    case NORTH:
        std::cout << "north" << std::endl;
        checkingSide = m_position + glm::vec3(0, 0, 1);
        break;
    case SOUTH:
        std::cout << "SOUTH" << std::endl;
        checkingSide = m_position + glm::vec3(0, 0, -1);
        break;
    case EAST:
        std::cout << "EAST" << std::endl;
        checkingSide = m_position + glm::vec3(1, 0, 0);
        break;
    case WEST:
        std::cout << "WEST" << std::endl;
        checkingSide = m_position + glm::vec3(-1, 0, 0);
        break;
    case TOP:
        std::cout << "TOP" << std::endl;
        checkingSide = m_position + glm::vec3(0, 1, 0);
        break;
    case BOTTOM:
        std::cout << "BOTTOM" << std::endl;
        checkingSide = m_position + glm::vec3(0, -1, 0);
        break;
    default:
        break;
    }
    return !Game::GetInstance()->m_currentWorld->getBlockAt(checkingSide).has_value();
}