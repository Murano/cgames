cmake_minimum_required(VERSION 3.12)
project(linux_system_programming C)

set(CMAKE_C_STANDARD 11)

add_executable(linux_system_programming main.c chapter4/run.c chapter4/run.h utils.c utils.h chapter4/cursor.c chapter4/cursor.h)