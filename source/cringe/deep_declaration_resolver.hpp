// Copyright (C) 2020 luna_koly
//
// Resolves the types.


#pragma once

#include "ast/visitor.hpp"
#include "session.hpp"


namespace cringe {
    /**
     * Resolves the types.
     */
    void resolve_deep_declarations(Session & session, AST::Node * node);
}
