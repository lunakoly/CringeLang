#include "global_declaration_resolver.hpp"

#include "../ast/nodes.hpp"
#include "../ast/scopes.hpp"

#include <stack>
#include <iostream>


using namespace cringe;
using namespace cringe::AST;


struct GlobalDeclarationResolver : public Visitor {
    /**
     * Common things, u know.
     */
    Session & session;
    /**
     * A shorthand.
     */
    Scope * global_scope = nullptr;


    // I wish u knew how much I hate the need to
    // manually write such stupid constructors...
    GlobalDeclarationResolver(Session & session) : session(session) {}


    virtual void visit(AST::DetailedNode<AST::NodeList> * it) override {
        for (auto that : it->details.values) {
            that->accept(this);
        }
    }

    virtual void visit(DetailedNode<GlobalNode> * it) override {
        global_scope = it->details.scope;
        it->details.files->accept(this);
    }

    virtual void visit(DetailedNode<FileNode> * it) override {
        it->details.root->accept(this);
    }

    virtual void visit(DetailedNode<FunctionStatementNode> * it) override {
        auto name = extract<IdentifierNode>(it->details.name);

        if (name != nullptr) {
            global_scope->add(name->details.value, it);
        } else {
            std::cout << "!!GlobalDeclarationResolver encountered a non-identifier as a function name: `" << *it->details.name << "`!!" << std::endl;
        }
    }

    virtual void visit(DetailedNode<ConstantDeclarationNode> * it) override {
        auto names = it->details.constants->details.values;

        for (size_t that = 0; that < names.size(); that++) {
            auto name = extract<IdentifierNode>(names[that]);

            if (name != nullptr) {
                // if (it->details.values != nullptr && it->details.values->details.values.size() == names.size()) {
                //     global_scope->add(name->details.value, it->details.values->details.values[that]);
                // } else {
                    global_scope->add(name->details.value, it);
                // }
            } else {
                std::cout << "!!GlobalDeclarationResolver encountered a non-identifier as a constant name name: `" << *names[that] << "`!!" << std::endl;
            }
        }
    }

    virtual void visit(DetailedNode<TypealiasDeclarationNode> * it) override {
        auto name = extract<IdentifierNode>(it->details.type);

        if (name != nullptr) {
            global_scope->add(name->details.value, it);
        } else {
            std::cout << "!!GlobalDeclarationResolver has no implementation for non-identifier type aliases names: `" << *it->details.type << "`!!" << std::endl;
        }
    }

    virtual void visit(DetailedNode<VariableDeclarationNode> * it) override {
        auto names = it->details.variables->details.values;

        for (size_t that = 0; that < names.size(); that++) {
            auto name = extract<IdentifierNode>(names[that]);

            if (name != nullptr) {
                global_scope->add(name->details.value, it);
            } else {
                std::cout << "!!GlobalDeclarationResolver encountered a non-identifier as a variable name name: `" << *names[that] << "`!!" << std::endl;
            }
        }
    }
};


void cringe::resolve_global_declarations(Session & session, AST::Node * node) {
    GlobalDeclarationResolver resolver{session};
    node->accept(&resolver);
}
