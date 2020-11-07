// Copyright (C) 2020 luna_koly
//
// The parser for the cringe language.


#pragma once

// #include <orders/parsing/analyzer.hpp>

#include "session.hpp"
#include "ast/nodes.hpp"
// #include "lexer.hpp"


namespace cringe {
    /**
     * Things the parser needs to save.
     */
    struct ParsingContext {
        /**
         * The single place where all
         * available compilation information
         * lives.
         */
        Session & session;

        /**
         * A single file input.
         */
        orders::AnalyzableStream input;

        /**
         * Builds an abstract syntax tree.
         */
        AST::Node * to_ast();
    };
}
