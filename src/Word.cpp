#include "World.hpp"


World::World()
{
}

World::~World()
{
}

auto World::setBlock(glm::vec3 position, std::string identifier) -> std::shared_ptr<Block>
{
    std::shared_ptr<Block> block = std::make_shared<Block>(identifier, position);
    m_blocks[position] = block;
    return block;
}

auto World::getBlockAt(glm::vec3 position) -> std::optional<std::shared_ptr<Block>>
{
    if(m_blocks.count(position) == 0)
    {
        return std::nullopt;
    } else {
        std::optional<std::shared_ptr<Block>> result = std::make_optional(m_blocks[position]);
        return result;
    }
}


auto World::getBlocks() -> std::unordered_map<glm::vec3, std::shared_ptr<Block>>
{
    return m_blocks;
}