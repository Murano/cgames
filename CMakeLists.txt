cmake_minimum_required(VERSION 3.12)
project(linux_system_programming C)

set(CMAKE_C_STANDARD 11)

add_executable(linux_system_programming main.c chapter3/run.h chapter3/run.c)