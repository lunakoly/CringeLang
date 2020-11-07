// Copyright (C) 2020 luna_koly
//
// Visitor tha visits everything.


#pragma once

#include "nodes.hpp"


namespace cringe {
    namespace AST {
        /**
         * Visitor tha visits everything
         * by default.
         */
        struct Explorer;
    }
}


struct cringe::AST::Explorer : public cringe::AST::Visitor {
    virtual void visit(AST::Node * it) override {

    }

    virtual void visit(AST::DetailedNode<AST::NodeList> * it) override {
        for (auto that : it->details.values) {
            that->accept(this);
        }
    }

    virtual void visit(AST::DetailedNode<AST::ErrorNode> * it) override {

    }

    virtual void visit(AST::DetailedNode<AST::GlobalNode> * it) override {
        it->details.files->accept(this);
    }

    virtual void visit(AST::DetailedNode<AST::ConstantDeclarationNode> * it) override {
        it->details.constants->accept(this);

        if (it->details.values != nullptr) {
            it->details.values->accept(this);
        }

        if (it->details.type != nullptr) {
            it->details.type->accept(this);
        }
    }

    virtual void visit(AST::DetailedNode<AST::TypealiasDeclarationNode> * it) override {
        it->details.type->accept(this);
        it->details.value->accept(this);
    }

    virtual void visit(AST::DetailedNode<AST::VariableDeclarationNode> * it) override {
        it->details.variables->accept(this);

        if (it->details.values != nullptr) {
            it->details.values->accept(this);
        }

        if (it->details.type != nullptr) {
            it->details.type->accept(this);
        }
    }

    virtual void visit(AST::DetailedNode<AST::BinaryExpressionNode> * it) override {
        it->details.left->accept(this);
        it->details.right->accept(this);
    }

    virtual void visit(AST::DetailedNode<AST::UnaryExpressionNode> * it) override {
        it->details.target->accept(this);
    }

    virtual void visit(AST::DetailedNode<AST::QualifiedAccessNode> * it) override {
        it->details.identifiers->accept(this);
    }

    virtual void visit(AST::DetailedNode<AST::CharacterLiteralNode> * it) override {

    }

    virtual void visit(AST::DetailedNode<AST::IdentifierNode> * it) override {

    }

    virtual void visit(AST::DetailedNode<AST::NumberLiteralNode> * it) override {

    }

    virtual void visit(AST::DetailedNode<AST::StringLiteralNode> * it) override {

    }

    virtual void visit(AST::DetailedNode<AST::TypeNode> * it) override {
        it->details.identifier->accept(this);
        it->details.subtypes->accept(this);
    }

    virtual void visit(AST::DetailedNode<AST::FunctionStatementNode> * it) override {
        it->details.name->accept(this);
        it->details.value_parameters->accept(this);
        it->details.body->accept(this);

        if (it->details.return_type != nullptr) {
            it->details.return_type->accept(this);
        }
    }

    virtual void visit(AST::DetailedNode<AST::IfStatementNode> * it) override {
        it->details.condition->accept(this);
        it->details.on_true->accept(this);

        if (it->details.on_else != nullptr) {
            it->details.on_else->accept(this);
        }
    }

    virtual void visit(AST::DetailedNode<AST::WhileStatementNode> * it) override {
        it->details.condition->accept(this);
        it->details.on_true->accept(this);
    }
};
