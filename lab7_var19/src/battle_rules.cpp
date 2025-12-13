#include "battle_rules.h"

bool can_kill(const Npc& a, const Npc& b) 
{
    if (!a.alive || !b.alive) return false;

    if (a.type == NpcType::Elf && b.type == NpcType::Robber) return true;
    if (a.type == NpcType::Bear && b.type == NpcType::Elf) return true;
    if (a.type == NpcType::Robber && b.type == NpcType::Robber) return true;

    return false;
}
