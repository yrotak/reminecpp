#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <memory>
#include "Texture.hpp"
#include "Game.hpp"

class Block
{
private:
    /* data */

    std::string m_identifier;
    glm::vec3 m_position;

    std::shared_ptr<Texture> m_texture;
public:

    static GLuint VBO, VAO;

    Block(std::string identifier, glm::vec3 position);
    ~Block();

    auto Render() -> void;

    static auto InitVertices() -> void;
    static auto ReleaseVertices() -> void;
};

#endif