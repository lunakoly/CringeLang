# Copyright (C) 2020 luna_koly


cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
    CringeLang
        "main.cpp"
)

target_link_libraries(CringeLang Orders Cringe)
