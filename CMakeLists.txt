cmake_minimum_required(VERSION 3.16)
project(projet_c C)

set(CMAKE_C_STANDARD 99)

add_executable(projet_c main.c functions/verifyDtd.c functions/xmlFunctions.c)