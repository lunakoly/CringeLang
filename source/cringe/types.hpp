// Copyright (C) 2020 luna_koly
//
// Basic things about types and scopes


#pragma once

#include <string>
#include <map>

#include "ast/visitor.hpp"
#include "session.hpp"


namespace cringe {
    /**
     * Represents some possible type.
     */
    // struct Type {
    //     struct Builtins {
    //         static Type * Any;
    //         static Type * Int;
    //         static Type * Unit;
    //         static Type * Bool;
    //         static Type * Char;
    //         static Type * String;
    //     };

    //     /**
    //      * Not the fully qualified name, but
    //      * it's last part.
    //      */
    //     std::string name;
    //     /**
    //      * The type declaration.
    //      */
    //     AST::Node * source;
    //     /**
    //      * Inner declarations.
    //      */
    //     Scope * scope = new Scope();

    //     // Type(
    //     //     AST::Node * source,
    //     //     const std::string & name
    //     // );
    // };

    /**
     * Resolves types.
     */
    struct Scope {
    public:
        Scope(Scope * parent = nullptr);

        static Scope * create_global();

        // /**
        //  * Returns true if the given type
        //  * has already been registered within
        //  * the scope.
        //  */
        // bool contains(Type * type);

        // /**
        //  * Registers a new type.
        //  */
        // void add(Type * type);

        // /**
        //  * Returns the type that matches
        //  * the given name.
        //  */
        // Type * resolve(AST::Node * qualified_access);

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
        // /**
        //  * Just stores the mapping.
        //  */
        // std::map<std::string, Type *> types;
        /**
         * Just stores the mapping.
         */
        std::map<std::string, AST::Node *> declarations;
    };
}
