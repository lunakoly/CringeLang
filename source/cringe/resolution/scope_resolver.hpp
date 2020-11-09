// Copyright (C) 2020 luna_koly
//
// Builds scopes for AST nodes.


#pragma once

#include "../session.hpp"
#include "../ast/visitor.hpp"


namespace cringe {
    /**
     * Substitutes scopes instead of `nullptr`s.
     */
    void resolve_scopes(Session & session, AST::Node * node);
}
