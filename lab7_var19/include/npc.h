#pragma once
#include <string>

enum class NpcType { Elf, Bear, Robber };

struct Npc 
{
    std::string name;
    NpcType type;
    int x{};
    int y{};
    bool alive{true};
    bool in_fight{false};

    Npc();
    Npc(NpcType t, const std::string& n, int x_, int y_);

    int move_distance() const;
    int kill_distance() const;
    int roll() const;
    const char* type_name() const;
};
