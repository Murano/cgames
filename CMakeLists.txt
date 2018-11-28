cmake_minimum_required(VERSION 3.12)
project(linux_system_programming C)

set(CMAKE_C_STANDARD 11)

add_executable(linux_system_programming main.c chapter2/run.h chapter2/run.c chapter2/sandbox.c)