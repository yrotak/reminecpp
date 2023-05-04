#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <memory>
#include "Texture.hpp"
#include "Game.hpp"

enum EBlockSide {
    NORTH,
    SOUTH,
    WEST,
    EAST,
    TOP,
    BOTTOM
};

class Block
{
private:
    /* data */

    std::string m_identifier;
    glm::vec3 m_position;

    std::shared_ptr<Texture> m_texture;

    bool m_isRendered;

    GLuint VBO, VAO;
public:

    Block(std::string identifier, glm::vec3 position);
    ~Block();

    auto Render() -> void;

    auto Update() -> void;

    auto ReleaseVertices() -> void;

    auto shouldSideBeRendered(EBlockSide blockSide) -> bool;
};

#endif