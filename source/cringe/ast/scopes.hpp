// Copyright (C) 2020 luna_koly
//
// Basic things about types and scopes


#pragma once

#include <string>
#include <map>

#include "visitor.hpp"
#include "../session.hpp"


namespace cringe {
    namespace AST {
        /**
         * Resolves types.
         */
        struct Scope {
        public:
            Scope(Scope * parent = nullptr);

            /**
             * Creates a new scope and sets up
             * the builtins.
             */
            static Scope * create_global();

            /**
             * Registers a new local declaration.
             */
            void add(const std::string & name, AST::Node * declaration);

            /**
             * Returns the declaration that matches
             * the given name.
             */
            AST::Node * resolve(Session & session, AST::DetailedNode<AST::QualifiedAccessNode> * qualified_access);

            /**
             * Returns the declaration that matches
             * the given name.
             */
            AST::Node * resolve(Session & session, AST::DetailedNode<AST::IdentifierNode> * node);

            /**
             * Returns the declaration that matches
             * the given name.
             */
            AST::Node * resolve(Session & session, AST::Node * node);

        private:
            /**
             * Scope to search if the type wasn't
             * found here.
             */
            Scope * parent;

            /**
             * Just stores the mapping.
             */
            std::map<std::string, AST::Node *> declarations;
        };
    }
}
