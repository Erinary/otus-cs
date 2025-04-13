cmake_minimum_required(VERSION 3.30)
project(otus_cs C)

set(CMAKE_C_STANDARD 11)

add_executable(otus_cs_hw05 hw05-guess-number/main.c
        hw12-course-work/server/main.c)
add_executable(otus_cs_hw06 hw06-labyrinth/main.c)
add_executable(otus_cs_hw11-client hw11-online-chat/client/main.c)
add_executable(otus_cs_hw11-server hw11-online-chat/server/main.c)
add_executable(otus_cs_hw12-client
        hw12-course-work/client/main.c
        hw12-course-work/libs/msg_codec.c
        hw12-course-work/libs/msg_codec.h
        hw12-course-work/libs/msg_types.h)
add_executable(otus_cs_hw12-server
        hw12-course-work/server/main.c
        hw12-course-work/libs/msg_codec.c
        hw12-course-work/libs/msg_codec.h
        hw12-course-work/libs/msg_types.h
        hw12-course-work/server/server.c
        hw12-course-work/server/server.h)
