cmake_minimum_required(VERSION 3.16)
project(lab7)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab7
    src/main.cpp
    src/world.cpp
    src/npc.cpp
    src/npc_factory.cpp
    src/battle_rules.cpp
)

target_include_directories(lab7 PRIVATE include)
target_link_libraries(lab7 pthread)

# ===== TESTS =====
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(lab7_tests
    tests/test_all.cpp
    src/world.cpp
    src/npc.cpp
    src/npc_factory.cpp
    src/battle_rules.cpp
)

target_include_directories(lab7_tests PRIVATE include)
target_link_libraries(lab7_tests gtest_main pthread)