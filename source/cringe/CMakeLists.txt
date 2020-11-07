# Copyright (C) 2020 luna_koly


cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    CringeProject
        VERSION 1.0
        DESCRIPTION "`Cringe` is the name of the hypothetical language."
)

configure_file(
    "about.hpp.in"
    "about.hpp"
)

add_library(
    Cringe STATIC
        "diagnostics.hpp"
        "diagnostics.cpp"
        "session.hpp"
        "ast/visitor.hpp"
        "ast/nodes.hpp"
        "ast/nodes.cpp"
        "ast/probably.hpp"
        "ast/explorer.hpp"
        "parser.hpp"
        "parser.cpp"
        "types.hpp"
        "types.cpp"
        "scope_resolver.hpp"
        "scope_resolver.cpp"
        "global_declaration_resolver.hpp"
        "global_declaration_resolver.cpp"
        "deep_declaration_resolver.hpp"
        "deep_declaration_resolver.cpp"
)

target_link_libraries(Cringe Orders)
