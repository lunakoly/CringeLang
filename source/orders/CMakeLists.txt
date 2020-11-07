# Copyright (C) 2020 luna_koly


cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    OrdersProject
        VERSION 1.0
        DESCRIPTION "Boilerplace code for some parsing features."
)

configure_file(
    "about.hpp.in"
    "about.hpp"
)

add_library(
    Orders STATIC
        "util/printable.hpp"
        "util/printable.cpp"
        "streams/stream.hpp"
        "streams/buffered_stream.hpp"
        "streams/accumulator_stream.hpp"
        "streams/implementations/std_stream.hpp"
        "streams/implementations/std_stream.cpp"
        "streams/implementations/simple_buffered_stream.hpp"
        "streams/implementations/simple_buffered_stream.cpp"
        "streams/implementations/simple_text_stream.hpp"
        "streams/implementations/simple_text_stream.cpp"
        "streams/implementations/analyzable_stream.hpp"
        "streams/implementations/analyzable_stream.cpp"
        "parsing/diagnostics.hpp"
        "parsing/diagnostics.cpp"
)
