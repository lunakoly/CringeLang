// Copyright (C) 2020 luna_koly
//
// The AST visitor and forward
// declarations of all the nodes.


#pragma once


namespace cringe {
    namespace AST {
        /**
         * Visitor for nodes.
         */
        struct Visitor;
        /**
         * Super TypeNode of all other AST nodes.
         */
        struct Node;
        /**
         * Super TypeNode of all other AST nodes.
         */
        template <typename T>
        struct DetailedNode;
        /**
         * [Node *, Node *, ...]
         */
        struct NodeList;
        /**
         * Error tokens.
         */
        struct ErrorNode;
        /**
         * [File1, File2, ...]
         */
        struct GlobalNode;
        /**
         * Filename + root node.
         */
        struct FileNode;
        /**
         * Safe way to cast nodes.
         */
        template <typename T>
        struct Extractor;
        /**
         * A safer way to cast a node.
         */
        template <typename T>
        DetailedNode<T> * extract(Node * node);

        /**
         * Declares `let x: Y = z`.
         */
        struct ConstantDeclarationNode;
        /**
         * Declares `TypeNode X = Y`.
         */
        struct TypealiasDeclarationNode;
        /**
         * Declares `var x: Y = z`.
         */
        struct VariableDeclarationNode;
        /**
         * Binary expressions.
         */
        struct BinaryExpressionNode;
        /**
         * Unary operators.
         */
        struct UnaryExpressionNode;
        /**
         * Expressions like `a.b.c`.
         */
        struct QualifiedAccessNode;
        /**
         * Leaf for characters.
         */
        struct CharacterLiteralNode;
        /**
         * Node that represents a name.
         */
        struct IdentifierNode;
        /**
         * Leaf for numbers.
         */
        struct NumberLiteralNode;
        /**
         * Leaf for strings.
         */
        struct StringLiteralNode;
        /**
         * Node that represents a TypeNode.
         */
        struct TypeNode;
        /**
         * Declares `fun ...(..., ): ...`.
         */
        struct FunctionStatementNode;
        /**
         * Declares `if ... ... else ...`.
         */
        struct IfStatementNode;
        /**
         * Declares `while ... ...`.
         */
        struct WhileStatementNode;
    }
}


struct cringe::AST::Visitor {
    virtual ~Visitor() {}

    virtual void visit(AST::Node * it) {}

    virtual void visit(AST::DetailedNode<AST::NodeList> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::ErrorNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::GlobalNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::FileNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::ConstantDeclarationNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::TypealiasDeclarationNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::VariableDeclarationNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::BinaryExpressionNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::UnaryExpressionNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::QualifiedAccessNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::CharacterLiteralNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::IdentifierNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::NumberLiteralNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::StringLiteralNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::TypeNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::FunctionStatementNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::IfStatementNode> * it) {
        visit((AST::Node *) it);
    }

    virtual void visit(AST::DetailedNode<AST::WhileStatementNode> * it) {
        visit((AST::Node *) it);
    }
};


#define __WITH_ACCEPT__ virtual void accept(cringe::AST::Visitor * visitor) { visitor->visit(this); }


template <typename T>
struct cringe::AST::Extractor : public cringe::AST::Visitor {
    cringe::AST::DetailedNode<T> * result = nullptr;

    virtual void visit(cringe::AST::Node * it) override {}

    virtual void visit(cringe::AST::DetailedNode<T> * it) override {
        result = it;
    }
};

template <typename T>
cringe::AST::DetailedNode<T> * cringe::AST::extract(Node * node) {
    Extractor<T> it;
    node->accept(&it);
    return it.result;
}
