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
        "parsing/parser.hpp"
        "parsing/parser.cpp"
        "ast/scopes.hpp"
        "ast/scopes.cpp"
        "resolution/scope_resolver.hpp"
        "resolution/scope_resolver.cpp"
        "resolution/global_declaration_resolver.hpp"
        "resolution/global_declaration_resolver.cpp"
        "resolution/deep_declaration_resolver.hpp"
        "resolution/deep_declaration_resolver.cpp"
)

target_link_libraries(Cringe Orders Threading)
