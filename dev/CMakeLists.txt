cmake_minimum_required(VERSION 3.17)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lncurses")

add_executable(game
        include.h
        main.cpp
        Countries.cpp
        Countries.h
        AbstractUnit.cpp
        AbstractUnit.h
        AbstractCountry.cpp
        AbstractCountry.h
        )