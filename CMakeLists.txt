cmake_minimum_required(VERSION 3.27)

project(AdventOfCode-2021 VERSION 0.0.1 LANGUAGES CXX)

set(CXX_FLAGS "-Wall -Wextra -Werror -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

add_executable(day01 ./day_01/day01.cpp)
add_executable(day02 ./day_02/day02.cpp)
add_executable(day03 ./day_03/day03.cpp)
add_executable(day04 ./day_04/day04.cpp)
add_executable(day05 ./day_05/day05.cpp)
add_executable(day06 ./day_06/day06.cpp)
