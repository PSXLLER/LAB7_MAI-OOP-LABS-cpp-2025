#include "world.h"
#include <cstdlib>
#include <ctime>

int main() 
{
    std::srand(std::time(nullptr));
    World w;
    w.run();
}
