// Copyright (C) 2020 luna_koly
//
// Registers global scope entities.


#pragma once

#include "../ast/visitor.hpp"


namespace cringe {
    /**
     * Registers global scope entities.
     */
    void resolve_global_declarations(AST::Node * node);
}
