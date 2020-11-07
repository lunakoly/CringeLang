#include "global_declaration_resolver.hpp"
#include "ast/nodes.hpp"
#include "types.hpp"

#include <stack>
#include <iostream>


using namespace cringe;
using namespace cringe::AST;


struct GlobalDeclarationResolver : public Visitor {
    /**
     * A shorthand.
     */
    Scope * global_scope = nullptr;

    virtual void visit(DetailedNode<GlobalNode> * it) override {
        global_scope = it->details.scope;
        it->details.files->accept(this);
    }

    virtual void visit(DetailedNode<FunctionStatementNode> * it) override {
        auto name = extract<IdentifierNode>(it->details.name);

        if (name != nullptr) {
            global_scope->add(name->details.value, it);
        } else {
            // session.reporter <<
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
                // session.reporter <<
            }
        }
    }

    virtual void visit(DetailedNode<TypealiasDeclarationNode> * it) override {
        auto name = extract<IdentifierNode>(it->details.type);

        if (name != nullptr) {
            global_scope->add(name->details.value, it);
        } else {
            // session.reporter <<
        }
    }

    virtual void visit(DetailedNode<VariableDeclarationNode> * it) override {
        auto names = it->details.variables->details.values;

        for (size_t that = 0; that < names.size(); that++) {
            auto name = extract<IdentifierNode>(names[that]);

            if (name != nullptr) {
                global_scope->add(name->details.value, it);
            } else {
                // session.reporter <<
            }
        }
    }
};


void cringe::resolve_global_declarations(AST::Node * node) {
    GlobalDeclarationResolver resolver;
    node->accept(&resolver);
}
