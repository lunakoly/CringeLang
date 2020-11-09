// Copyright (C) 2020 luna_koly
//
// All AST Nodes live here.


#pragma once

#include "visitor.hpp"
#include "../types.hpp"

#include <orders/util/printable.hpp>

#include <vector>
#include <string>
#include <variant>


#define __EASILY_CONVERTIBLE__(T)           \
    operator DetailedNode<T> * () {         \
        return new DetailedNode{*this};     \
    }                                       \


struct cringe::AST::Node : public orders::Printable {
    __WITH_ACCEPT__
    __WITH_PURE_PRINT__

    virtual ~Node() {}

    /**
     * Only the ErrorNode shows true here.
     */
    virtual bool is_error() const {
        return false;
    }
};


template <typename T>
struct cringe::AST::DetailedNode : public cringe::AST::Node {
    __WITH_ACCEPT__
    __WITH_CUSTOM_PRINT__

    T details;

    DetailedNode(T node) : details(node) {}
};


struct cringe::AST::NodeList {
    std::vector<Node *> values;
};


struct cringe::AST::ErrorNode {
    // __EASILY_CONVERTIBLE__(ErrorNode)

    std::string value;
};


template <>
struct cringe::AST::DetailedNode<cringe::AST::ErrorNode> : public cringe::AST::Node {
    __WITH_ACCEPT__
    __WITH_CUSTOM_PRINT__

    ErrorNode details;

    DetailedNode(ErrorNode node) : details(node) {}

    virtual bool is_error() const override {
        return true;
    }
};


struct cringe::AST::GlobalNode {
    DetailedNode<NodeList> * files;
    Scope * scope = nullptr;
};


struct cringe::AST::FileNode {
    std::string filename;
    Node * root;
};


struct cringe::AST::ConstantDeclarationNode {
    DetailedNode<NodeList> * constants;
    DetailedNode<NodeList> * values;
    Node * type;
};


struct cringe::AST::TypealiasDeclarationNode {
    Node * type;
    Node * value;
};


struct cringe::AST::VariableDeclarationNode {
    DetailedNode<NodeList> * variables;
    DetailedNode<NodeList> * values;
    Node * type;
};


struct cringe::AST::BinaryExpressionNode {
    Node * left;
    Node * right;
    std::string operator_token;
};


struct cringe::AST::UnaryExpressionNode {
    Node * target;
    std::string operator_token;
};


struct cringe::AST::QualifiedAccessNode {
    DetailedNode<NodeList> * identifiers;
};


struct cringe::AST::CharacterLiteralNode {
    std::string value;
};


struct cringe::AST::IdentifierNode {
    std::string value;
};


struct cringe::AST::NumberLiteralNode {
    std::string value;
    std::variant<int, double> calculated;
};


struct cringe::AST::StringLiteralNode {
    std::string value;
};


struct cringe::AST::TypeNode {
    Node * identifier;
    DetailedNode<NodeList> * subtypes;
    Node * declaration = nullptr;
};


struct cringe::AST::FunctionStatementNode {
    Node * name;
    Node * return_type;
    DetailedNode<NodeList> * value_parameters;
    Node * body;
    Scope * scope = nullptr;
};


struct cringe::AST::IfStatementNode {
    Node * condition;
    Node * on_true;
    Node * on_else;
    Scope * scope = nullptr;
};


struct cringe::AST::WhileStatementNode {
    Node * condition;
    Node * on_true;
    Scope * scope = nullptr;
};
