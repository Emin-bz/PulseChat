cmake_minimum_required(VERSION 3.10)

project(PulseChat C)

set(CMAKE_C_STANDARD 11)

add_executable(PulseChat src/main.c src/server.c)
