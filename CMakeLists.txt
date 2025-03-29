cmake_minimum_required(VERSION 3.30)
project(otus_cs C)

set(CMAKE_C_STANDARD 11)

add_executable(otus_cs_hw05 hw05-guess-number/main.c)
add_executable(otus_cs_hw06 hw06-labyrinth/main.c)
add_executable(otus_cs_hw11-client hw11-online-chat/client/main.c)
add_executable(otus_cs_hw11-server hw11-online-chat/server/main.c)
