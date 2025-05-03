cmake_minimum_required(VERSION 3.30)
project(otus_cs C)

set(CMAKE_C_STANDARD 11)

add_executable(otus_cs_hw05 hw05-guess-number/main.c)
add_executable(otus_cs_hw06 hw06-labyrinth/main.c)
add_executable(otus_cs_hw07 hw07-lexer/main.c
        hw07-lexer/list.c
        hw07-lexer/list.h
        hw07-lexer/scanner.c
        hw07-lexer/scanner.h)
add_executable(otus_cs_hw08 hw08-interpreter/main.c
        hw08-interpreter/generated/lexer.h
        hw08-interpreter/generated/lexer.c
        hw08-interpreter/generated/parser.h
        hw08-interpreter/generated/parser.c
        hw08-interpreter/ast.h
        hw08-interpreter/ast.c
        hw08-interpreter/symbol_table.h
        hw08-interpreter/symbol_table.c
)
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
