# Copyright (C) 2020 luna_koly


cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    CringeLangProject
        VERSION 1.1
        DESCRIPTION "Having fun"
        LANGUAGES CXX
)

add_subdirectory("source")

target_include_directories(
    Cringe PUBLIC
        "source"
)

target_include_directories(
    CringeLang PUBLIC
        "source"
        "${PROJECT_BINARY_DIR}/source"
)
