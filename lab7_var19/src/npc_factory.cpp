#include "npc_factory.h"
#include <random>

Npc NpcFactory::create_random(int id) {
    static std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> type_dist(0, 2);
    std::uniform_int_distribution<int> pos_dist(0, 99);

    NpcType type = static_cast<NpcType>(type_dist(gen));
    return Npc(type, std::string(type == NpcType::Elf ? "Elf" : type == NpcType::Bear ? "Bear" : "Robber") + "_" + std::to_string(id), pos_dist(gen), pos_dist(gen));
}

Npc NpcFactory::create_fixed(NpcType type, const std::string& name, int x, int y) 
{
    return Npc(type, name, x, y);
}
