#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <condition_variable>
#include "npc.h"

struct FightTask 
{
    size_t a;
    size_t b;
};

class World 
{
public:
    World();
    void run();

private:
    std::vector<Npc> npcs;

    std::shared_mutex npc_mutex;
    std::mutex fight_mutex;
    std::mutex cout_mutex;

    std::queue<FightTask> fight_queue;
    std::condition_variable fight_cv;

    std::atomic<bool> running{true};

    void movement_thread();
    void fight_thread();
    void print_thread();
};
