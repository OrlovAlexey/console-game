cmake_minimum_required(VERSION 3.17)
project(game)

set(CMAKE_CXX_STANDARD 17)

add_executable(game main.cpp Countries.cpp Countries.h include.h AbstractUnit.cpp AbstractUnit.h AbstractCountry.cpp AbstractCountry.h)