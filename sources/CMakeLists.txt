# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
cmake_minimum_required(VERSION 2.9)

project (FASM-Z80)

set(CMAKE_UNITY_BUILD ON)

set (TARGET_NAME FASM-Z80)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

if(WIN32)

else()
    add_definitions(-m64 -fmax-errors=1 -Wall)
endif()

add_subdirectory(TAPview)
