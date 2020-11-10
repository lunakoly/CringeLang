// Copyright (C) 2020 luna_koly
//
// The parser for the cringe language.


#pragma once


#include "../session.hpp"
#include "../ast/nodes.hpp"


namespace cringe {
    /**
     * Builds an abstract syntax tree.
     */
    AST::Node * parse(Session & session, const std::string & filename);
}
