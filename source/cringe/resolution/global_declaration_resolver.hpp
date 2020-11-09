// Copyright (C) 2020 luna_koly
//
// Registers global scope entities.


#pragma once

#include "../session.hpp"
#include "../ast/visitor.hpp"


namespace cringe {
    /**
     * Registers global scope entities.
     */
    void resolve_global_declarations(Session & session, AST::Node * node);
}
