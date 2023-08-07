cmake_minimum_required(VERSION 3.10)

project(EchoServer C)

set(CMAKE_C_STANDARD 11)

add_executable(EchoServer src/main.c src/server.c)
