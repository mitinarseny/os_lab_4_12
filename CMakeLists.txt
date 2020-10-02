cmake_minimum_required(VERSION 3.18)

project(LAB3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(shared shared.cpp shared.h)

add_executable(server server.cpp)
target_link_libraries(server shared)

add_executable(client client.cpp)
target_link_libraries(client shared)
