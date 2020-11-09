#include "scopes.hpp"
#include "nodes.hpp"


using namespace cringe;
using namespace cringe::AST;


// Type::Type(
//     AST::Node * source,
//     const std::string & name
// )
//     : source(source)
//     , name(name)
//     , scope(new Scope()) {}

// Type * Type::Builtins::Any    = new Type{"Any",    nullptr};
// Type * Type::Builtins::Int    = new Type{"Int",    nullptr};
// Type * Type::Builtins::Unit   = new Type{"Unit",   nullptr};
// Type * Type::Builtins::Bool   = new Type{"Bool",   nullptr};
// Type * Type::Builtins::Char   = new Type{"Char",   nullptr};
// Type * Type::Builtins::String = new Type{"String", nullptr};


Scope::Scope(Scope * parent) : parent(parent) {}

Scope * Scope::create_global() {
    auto global = new Scope();
    // global.add(Type::Builtins::Any);
    // global.add(Type::Builtins::Int);
    // global.add(Type::Builtins::Unit);
    // global.add(Type::Builtins::Bool);
    // global.add(Type::Builtins::Char);
    // global.add(Type::Builtins::String);

    global->add("Int", new DetailedNode{TypeNode{
        .identifier = new DetailedNode{IdentifierNode{"Int"}},
        .subtypes = new DetailedNode{NodeList()}
    }});

    global->add("Real", new DetailedNode{TypeNode{
        .identifier = new DetailedNode{IdentifierNode{"Real"}},
        .subtypes = new DetailedNode{NodeList()}
    }});

    global->add("Char", new DetailedNode{TypeNode{
        .identifier = new DetailedNode{IdentifierNode{"Char"}},
        .subtypes = new DetailedNode{NodeList()}
    }});

    global->add("String", new DetailedNode{TypeNode{
        .identifier = new DetailedNode{IdentifierNode{"String"}},
        .subtypes = new DetailedNode{NodeList()}
    }});

    return global;
}

// bool Scope::contains(Type * type) {
//     auto it = types.find(type->name);
//     return it != types.end();
// }

// void Scope::add(Type * type) {
//     types[type->name] = type;
// }

// Type * Scope::resolve(AST::Identifier * identifier) {
//     auto it = types.find(identifier->value);

//     if (it != types.end()) {
//         if (identifier->further != nullptr) {
//             return it->second->scope->resolve(identifier->further);
//         }

//         return it->second;
//     }

//     if (parent != nullptr) {
//         return parent->resolve(identifier);
//     }

//     return nullptr;
// }


void Scope::add(const std::string & name, AST::Node * declaration) {
    declarations[name] = declaration;
}


struct ScopeExtractor : public Visitor {
    Scope * result = nullptr;

    virtual void visit(Node * it) override {}

    virtual void visit(DetailedNode<GlobalNode> * it) override {
        result = it->details.scope;
    }

    virtual void visit(DetailedNode<FunctionStatementNode> * it) override {
        result = it->details.scope;
    }

    virtual void visit(DetailedNode<IfStatementNode> * it) override {
        result = it->details.scope;
    }

    virtual void visit(DetailedNode<WhileStatementNode> * it) override {
        result = it->details.scope;
    }
};

Scope * extract_scope(Node * node) {
    ScopeExtractor it;
    node->accept(&it);
    return it.result;
}


Node * Scope::resolve(Session & session, Node * node) {
    auto qualified_access = extract<QualifiedAccessNode>(node);

    if (qualified_access != nullptr) {
        return resolve(session, qualified_access);
    }

    return nullptr;
}

Node * Scope::resolve(Session & session, DetailedNode<QualifiedAccessNode> * qualified_access) {
    auto names = qualified_access->details.identifiers->details.values;
    Node * declaration = nullptr;
    Scope * scope = this;

    for (size_t it = 0; it < names.size(); it++) {
        auto name = extract<IdentifierNode>(names[it]);

        if (name == nullptr) {
            // session.reporter <<
            return nullptr;
        }

        if (scope == nullptr) {
            // session.reporter <<
            return nullptr;
        }

        auto that = scope->declarations.find(name->details.value);

        if (that != scope->declarations.end()) {
            declaration = that->second;
            scope = extract_scope(declaration);
        } else {
            declaration = nullptr;
            break;
        }
    }

    if (declaration == nullptr && parent != nullptr) {
        return parent->resolve(session, qualified_access);
    }

    return declaration;
}

AST::Node * Scope::resolve(Session & session, AST::DetailedNode<AST::IdentifierNode> * node) {
    auto that = declarations.find(node->details.value);

    if (that != declarations.end()) {
        return that->second;
    }

    if (parent != nullptr) {
        return parent->resolve(session, node);
    }

    return nullptr;
}
