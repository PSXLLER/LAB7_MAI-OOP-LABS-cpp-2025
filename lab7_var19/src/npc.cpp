#include "npc.h"
#include <cstdlib>

Npc::Npc() = default;

Npc::Npc(NpcType t, const std::string& n, int x_, int y_)
    : name(n), type(t), x(x_), y(y_) {}

int Npc::move_distance() const 
{
    if (type == NpcType::Elf) return 10;
    if (type == NpcType::Bear) return 5;
    return 10;
}

int Npc::kill_distance() const 
{
    if (type == NpcType::Elf) return 50;
    return 10;
}

int Npc::roll() const 
{
    return std::rand() % 6 + 1;
}

const char* Npc::type_name() const 
{
    if (type == NpcType::Elf) return "Elf";
    if (type == NpcType::Bear) return "Bear";
    return "Robber";
}
