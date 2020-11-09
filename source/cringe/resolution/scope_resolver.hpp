// Copyright (C) 2020 luna_koly
//
// Builds scopes for AST nodes.


#pragma once

#include "../ast/visitor.hpp"


namespace cringe {
    /**
     * Substitutes scopes instead of `nullptr`s.
     */
    void resolve_scopes(AST::Node * node);
}
