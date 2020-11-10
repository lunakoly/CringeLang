# Copyright (C) 2020 luna_koly


cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    ThreadingProject
        VERSION 1.0
        DESCRIPTION "Boilerplate code for threading things."
)

configure_file(
    "about.hpp.in"
    "about.hpp"
)

add_library(
    Threading STATIC
        "thread_pool.hpp"
        "thread_pool.cpp"
)
