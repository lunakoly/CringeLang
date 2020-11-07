#include "scope_resolver.hpp"
#include "ast/explorer.hpp"
#include "types.hpp"

#include <stack>
#include <iostream>


using namespace cringe;
using namespace cringe::AST;


struct ScopeResolver : public Explorer {
    /**
     * The stack of scopes.
     */
    std::stack<Scope *> scopes;

    virtual void visit(Node * it) override {
        std::cout << "UNIMPLEMENTED FOR " << *it;
    }

    virtual void visit(DetailedNode<GlobalNode> * it) override {
        it->details.scope = Scope::create_global();
        scopes.push(it->details.scope);

        it->details.files->accept(this);

        scopes.pop();
    }

    virtual void visit(DetailedNode<FunctionStatementNode> * it) override {
        it->details.scope = new Scope(scopes.top());
        scopes.push(it->details.scope);

        it->details.name->accept(this);
        it->details.value_parameters->accept(this);
        it->details.body->accept(this);

        if (it->details.return_type != nullptr) {
            it->details.return_type->accept(this);
        }

        scopes.pop();
    }

    virtual void visit(DetailedNode<IfStatementNode> * it) override {
        it->details.scope = new Scope(scopes.top());
        scopes.push(it->details.scope);

        it->details.condition->accept(this);
        it->details.on_true->accept(this);

        if (it->details.on_else != nullptr) {
            it->details.on_else->accept(this);
        }

        scopes.pop();
    }

    virtual void visit(DetailedNode<WhileStatementNode> * it) override {
        it->details.scope = new Scope(scopes.top());
        scopes.push(it->details.scope);

        it->details.condition->accept(this);
        it->details.on_true->accept(this);

        scopes.pop();
    }
};


void cringe::resolve_scopes(AST::Node * node) {
    ScopeResolver resolver;
    node->accept(&resolver);
}
