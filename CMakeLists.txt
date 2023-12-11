cmake_minimum_required(VERSION 3.19)
project(Projeto2final C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")

add_executable(Projeto2final Projeto2_main.c Additional_functions_1.c Additional_functions_2.c)