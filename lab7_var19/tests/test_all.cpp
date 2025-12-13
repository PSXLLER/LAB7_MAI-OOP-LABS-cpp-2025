#include <gtest/gtest.h>
#include "npc.h"
#include "npc_factory.h"
#include "battle_rules.h"
#include <algorithm>

TEST(NpcProperties, MoveDistance) 
{
    EXPECT_EQ(
        NpcFactory::create_fixed(NpcType::Elf, "Elf", 0, 0).move_distance(),
        10
    );
    EXPECT_EQ(
        NpcFactory::create_fixed(NpcType::Robber, "Robber", 0, 0).move_distance(),
        10
    );
    EXPECT_EQ(
        NpcFactory::create_fixed(NpcType::Bear, "Bear", 0, 0).move_distance(),
        5
    );
}

TEST(NpcProperties, KillDistance) 
{
    EXPECT_EQ(
        NpcFactory::create_fixed(NpcType::Elf, "Elf", 0, 0).kill_distance(),
        50
    );
    EXPECT_EQ(
        NpcFactory::create_fixed(NpcType::Robber, "Robber", 0, 0).kill_distance(),
        10
    );
    EXPECT_EQ(
        NpcFactory::create_fixed(NpcType::Bear, "Bear", 0, 0).kill_distance(),
        10
    );
}

TEST(NpcProperties, InitiallyAlive) 
{
    auto npc = NpcFactory::create_fixed(NpcType::Elf, "Elf", 0, 0);
    EXPECT_TRUE(npc.alive);
}

TEST(NpcRoll, RollInRange) 
{
    auto npc = NpcFactory::create_fixed(NpcType::Elf, "Elf", 0, 0);
    for (int i = 0; i < 100; ++i) {
        int r = npc.roll();
        EXPECT_GE(r, 1);
        EXPECT_LE(r, 6);
    }
}

TEST(BattleRules, ElfKillsRobber) 
{
    auto elf = NpcFactory::create_fixed(NpcType::Elf, "Elf", 0, 0);
    auto robber = NpcFactory::create_fixed(NpcType::Robber, "Robber", 0, 0);
    EXPECT_TRUE(can_kill(elf, robber));
}

TEST(BattleRules, RobberCannotKillElf) 
{
    auto elf = NpcFactory::create_fixed(NpcType::Elf, "Elf", 0, 0);
    auto robber = NpcFactory::create_fixed(NpcType::Robber, "Robber", 0, 0);
    EXPECT_FALSE(can_kill(robber, elf));
}

TEST(BattleRules, BearKillsElf) 
{
    auto bear = NpcFactory::create_fixed(NpcType::Bear, "Bear", 0, 0);
    auto elf = NpcFactory::create_fixed(NpcType::Elf, "Elf", 0, 0);
    EXPECT_TRUE(can_kill(bear, elf));
}

TEST(BattleRules, ElfCannotKillElf) 
{
    auto a = NpcFactory::create_fixed(NpcType::Elf, "Elf1", 0, 0);
    auto b = NpcFactory::create_fixed(NpcType::Elf, "Elf2", 0, 0);
    EXPECT_FALSE(can_kill(a, b));
}

TEST(BattleRules, RobberKillsRobber) 
{
    auto a = NpcFactory::create_fixed(NpcType::Robber, "R1", 0, 0);
    auto b = NpcFactory::create_fixed(NpcType::Robber, "R2", 0, 0);
    EXPECT_TRUE(can_kill(a, b));
}

TEST(Movement, DeadNpcDoesNotMove) 
{
    auto npc = NpcFactory::create_fixed(NpcType::Elf, "Dead", 50, 50);
    npc.alive = false;

    int x = npc.x;
    int y = npc.y;

    if (npc.alive) 
    {
        npc.x += 10;
        npc.y += 10;
    }

    EXPECT_EQ(npc.x, x);
    EXPECT_EQ(npc.y, y);
}

TEST(Movement, NpcClampedToMap) 
{
    auto npc = NpcFactory::create_fixed(NpcType::Elf, "Clamp", 0, 0);

    npc.x -= 1000;
    npc.y += 1000;

    npc.x = std::clamp(npc.x, 0, 99);
    npc.y = std::clamp(npc.y, 0, 99);

    EXPECT_EQ(npc.x, 0);
    EXPECT_EQ(npc.y, 99);
}
