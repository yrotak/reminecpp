#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <optional>
#include "Block.hpp"


class Block;

class World
{
private:
    std::unordered_map<glm::vec3, std::shared_ptr<Block>> m_blocks;

public:
    World();
    ~World();

    auto setBlock(glm::vec3 position, std::string identifier) -> std::shared_ptr<Block>;

    auto getBlockAt(glm::vec3 position) -> std::optional<std::shared_ptr<Block>>;

    auto getBlocks() -> std::unordered_map<glm::vec3, std::shared_ptr<Block>>;
};

#endif