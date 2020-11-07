# Copyright (C) 2020 luna_koly


cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    ArrrghProject
        VERSION 1.1
        DESCRIPTION "Boilerplace code for parsing CLI arguments."
)

configure_file(
    "about.hpp.in"
    "about.hpp"
)

add_library(Arrrgh INTERFACE)

target_sources(Arrrgh INTERFACE "arrrgh.hpp")
