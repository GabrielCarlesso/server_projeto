cmake_minimum_required(VERSION 3.26)
project(server_projeto C)

set(CMAKE_C_STANDARD 11)

add_executable(server_projeto main.c
        include/cercaVirtual.h)

target_link_libraries(server_projeto ulfius jansson)