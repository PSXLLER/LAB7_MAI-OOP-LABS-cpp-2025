#pragma once
#include "npc.h"

struct NpcFactory 
{
    static Npc create_random(int id);
    static Npc create_fixed(NpcType type, const std::string& name, int x, int y);
};
