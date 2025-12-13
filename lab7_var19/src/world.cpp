#include "world.h"
#include "battle_rules.h"
#include "npc_factory.h"

#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <iostream>

static int distance(const Npc& a, const Npc& b) 
{
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}

World::World() 
{
    for (int i = 0; i < 50; ++i) 
    {
        npcs.push_back(NpcFactory::create_random(i));
    }
}

void World::movement_thread() 
{
    while (running) 
    {
        {
            std::lock_guard lock(npc_mutex);

            for (auto& n : npcs) 
            {
                if (!n.alive) continue;

                int dx = (rand() % 3 - 1) * n.move_distance();
                int dy = (rand() % 3 - 1) * n.move_distance();

                n.x = std::clamp(n.x + dx, 0, 99);
                n.y = std::clamp(n.y + dy, 0, 99);
            }

            for (size_t i = 0; i < npcs.size(); ++i) 
            {
                for (size_t j = i + 1; j < npcs.size(); ++j) 
                {
                    auto& a = npcs[i];
                    auto& b = npcs[j];

                    if (!a.alive || !b.alive) continue;
                    if (a.in_fight || b.in_fight) continue;
                    if (!can_kill(a, b)) continue;
                    if (distance(a, b) > a.kill_distance()) continue;

                    a.in_fight = true;
                    b.in_fight = true;

                    {
                        std::lock_guard qlock(fight_mutex);
                        fight_queue.push({i, j});
                    }
                    fight_cv.notify_one();
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void World::fight_thread() 
{
    while (running) {
        FightTask task;

        {
            std::unique_lock lock(fight_mutex);
            fight_cv.wait(lock, [&] 
            {
                return !fight_queue.empty() || !running;
            });

            if (!running) break;

            task = fight_queue.front();
            fight_queue.pop();
        }

        std::lock_guard npc_lock(npc_mutex);

        auto& a = npcs[task.a];
        auto& b = npcs[task.b];

        if (!a.alive || !b.alive) 
        {
            a.in_fight = false;
            b.in_fight = false;
            continue;
        }

        {
            std::lock_guard cout_lock(cout_mutex);
            std::cout << "[Fight] " << a.name << " vs " << b.name << "\n";
        }

        int attack = a.roll();
        int defense = b.roll();

        if (attack > defense) 
        {
            b.alive = false;

            std::lock_guard cout_lock(cout_mutex);
            std::cout << "[Kill] " << a.name << " killed " << b.name << " (attack=" << attack << ", defense=" << defense << ")\n";
        }

        a.in_fight = false;
        b.in_fight = false;
    }
}

void World::print_thread() 
{
    while (running) 
    {
        {
            std::shared_lock lock(npc_mutex);
            std::lock_guard cout_lock(cout_mutex);

            std::cout << "\n===== MAP =====\n";
            for (const auto& n : npcs) 
            {
                if (n.alive) {
                    std::cout << n.name << " (" << n.x << ", " << n.y << ")\n";
                }
            }
            std::cout << "===============\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void World::run() 
{
    std::thread t_move(&World::movement_thread, this);
    std::thread t_fight(&World::fight_thread, this);
    std::thread t_print(&World::print_thread, this);

    std::this_thread::sleep_for(std::chrono::seconds(30));
    running = false;

    fight_cv.notify_all();

    t_move.join();
    t_fight.join();
    t_print.join();

    std::cout << "\nSURVIVORS:\n";
    for (const auto& n : npcs) 
    {
        if (n.alive) 
        {
            std::cout << n.name << "\n";
        }
    }
}
