cmake_minimum_required(VERSION 4.2)
project(cccc C)

set(CMAKE_C_STANDARD 17)

add_executable(cccc main.c
        common.h
        tokenizer.c
        parse.c
        codegen.c
        common.c)
