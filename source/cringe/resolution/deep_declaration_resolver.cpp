#include "deep_declaration_resolver.hpp"

#include "../ast/explorer.hpp"
#include "../ast/scopes.hpp"

#include "../diagnostics.hpp"

#include <stack>
#include <iostream>


using namespace cringe;
using namespace cringe::AST;


/**
 * Helps to convert individual T's
 * to their corresponding DetailedNode's.
 */
struct DetailsConverter {};

/**
 * Use `DETAILS << MyNode{}`.
 */
static DetailsConverter DETAILS;

/**
 * Even shorter variant...
 */
#define $ DETAILS <<
// or the more readable one...
#define detailed DETAILS <<

template <typename T>
DetailedNode<T> * operator << (const DetailsConverter & converter, T it) {
    return new DetailedNode{it};
}


struct TypeNodeExtractor : public Visitor {
    DetailedNode<TypeNode> * result = nullptr;

    virtual void visit(Node * it) override {}

    virtual void visit(DetailedNode<FunctionStatementNode> * it) override {
        auto return_type = it->details.return_type;

        if (return_type != nullptr) {
            result = extract<TypeNode>(return_type);
        }
    }

    virtual void visit(DetailedNode<ConstantDeclarationNode> * it) override {
        auto type = it->details.type;

        if (type != nullptr) {
            result = extract<TypeNode>(type);
        }
    }

    virtual void visit(DetailedNode<TypealiasDeclarationNode> * it) override {
        auto type = it->details.type;

        if (type != nullptr) {
            result = extract<TypeNode>(type);
        }
    }

    virtual void visit(DetailedNode<VariableDeclarationNode> * it) override {
        auto type = it->details.type;

        if (type != nullptr) {
            result = extract<TypeNode>(type);
        }
    }

    virtual void visit(DetailedNode<TypeNode> * it) override {
        result = it;
    }
};

DetailedNode<TypeNode> * extract_type_node(Node * node) {
    TypeNodeExtractor it;
    node->accept(&it);
    return it.result;
}


struct DeepDeclarationResolver : public Explorer {
    /**
     * Common things, u know.
     */
    Session & session;
    /**
     * The stack of scopes.
     */
    std::stack<Scope *> scopes;
    /**
     * The stack of the return values
     * because in C++ we can't use templates
     * with the visitor pattern.
     */
    std::stack<DetailedNode<TypeNode> *> declarations;


    DeepDeclarationResolver(Session & session) : session(session) {}


    virtual void visit(Node * it) override {
        std::cout << "!!DeepDeclarationResolver wasn't implemented for `" << *it << "`!!" << std::endl;

        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"[UNIMPLEMENTED]"},
            .subtypes = $ NodeList()
        });
    }

    virtual void visit(AST::DetailedNode<AST::NodeList> * it) override {
        DetailedNode<TypeNode> * last = nullptr;

        for (auto that : it->details.values) {
            that->accept(this);
            last = declarations.top();
            declarations.pop();
        }

        if (last == nullptr) {
            last = $ TypeNode {
                .identifier = $ IdentifierNode{"Unit"},
                .subtypes = $ NodeList()
            };
        }

        declarations.push(last);
    }

    virtual void visit(AST::DetailedNode<AST::ErrorNode> * it) override {
        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"[ERROR]"},
            .subtypes = $ NodeList()
        });
    }

    virtual void visit(DetailedNode<GlobalNode> * it) override {
        scopes.push(it->details.scope);

        it->details.files->accept(this);

        scopes.pop();
    }

    virtual void visit(AST::DetailedNode<AST::ConstantDeclarationNode> * it) override {
        DetailedNode<TypeNode> * type = nullptr;

        if (it->details.values != nullptr) {
            it->details.values->accept(this);
            type = declarations.top();
            declarations.pop();
        }

        if (it->details.type != nullptr) {
            it->details.type->accept(this);
            type = declarations.top();
            declarations.pop();
        } else if (type == nullptr) {
            type = $ TypeNode{
                .identifier = $ IdentifierNode{"Unit"},
                .subtypes = $ NodeList()
            };
        }

        it->details.type = type;

        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"Unit"},
            .subtypes = $ NodeList()
        });

        // REGISTER

        auto names = it->details.constants->details.values;

        for (size_t that = 0; that < names.size(); that++) {
            auto name = extract<IdentifierNode>(names[that]);

            if (name != nullptr) {
                scopes.top()->add(name->details.value, it);
            } else {
                std::cout << "!!DeepDeclarationResolver encountered a non-identifier as a constant name name: `" << *names[that] << "`!!" << std::endl;
            }
        }
    }

    virtual void visit(AST::DetailedNode<AST::TypealiasDeclarationNode> * it) override {
        it->details.value->accept(this);
        auto new_value = declarations.top();
        declarations.pop();
        it->details.value = new_value;

        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"Unit"},
            .subtypes = $ NodeList()
        });

        // REGISTER

        auto name = extract<IdentifierNode>(it->details.type);

        if (name != nullptr) {
            scopes.top()->add(name->details.value, it);
        } else {
            std::cout << "!!DeepDeclarationResolver has no implementation for non-identifier type aliases names: `" << *it->details.type << "`!!" << std::endl;
        }
    }

    virtual void visit(AST::DetailedNode<AST::VariableDeclarationNode> * it) override {
        DetailedNode<TypeNode> * type = nullptr;

        if (it->details.values != nullptr) {
            it->details.values->accept(this);
            type = declarations.top();
            declarations.pop();
        }

        if (it->details.type != nullptr) {
            it->details.type->accept(this);
            type = declarations.top();
            declarations.pop();
        } else if (type == nullptr) {
            type = $ TypeNode{
                .identifier = $ IdentifierNode{"Unit"},
                .subtypes = $ NodeList()
            };
        }

        it->details.type = type;

        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"Unit"},
            .subtypes = $ NodeList()
        });

        // REGISTER

        auto names = it->details.variables->details.values;

        for (size_t that = 0; that < names.size(); that++) {
            auto name = extract<IdentifierNode>(names[that]);

            if (name != nullptr) {
                scopes.top()->add(name->details.value, it);
            } else {
                std::cout << "!!DeepDeclarationResolver encountered a non-identifier as a variable name name: `" << *names[that] << "`!!" << std::endl;
            }
        }
    }

    virtual void visit(AST::DetailedNode<AST::BinaryExpressionNode> * it) override {
        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"[BINARY]"},
            .subtypes = $ NodeList()
        });
    }

    virtual void visit(AST::DetailedNode<AST::UnaryExpressionNode> * it) override {
        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"[UNARY]"},
            .subtypes = $ NodeList()
        });
    }

    virtual void visit(AST::DetailedNode<AST::QualifiedAccessNode> * it) override {
        auto that = scopes.top()->resolve(session, it);

        if (that != nullptr) {
            auto type = extract_type_node(that);

            if (type != nullptr) {
                declarations.push($ TypeNode{
                    .identifier = type->details.identifier,
                    .declaration = that
                });
            } else {
                std::stringstream rendered;
                it->print(rendered);

                session.reporter << InaccessibleTypeInformationDiagnostic{
                    .filename = "[MISSING_FILENAME]",
                    .line_number = 0,
                    .range = {0, 0},
                    .visualization = "[MISSING_VISUALIZATION]",
                    .accessor = rendered.str()
                };

                declarations.push($ TypeNode{
                    .identifier = $ IdentifierNode{"[DECLARATION_WITHOUT_TYPE]"},
                    .subtypes = $ NodeList()
                });
            }
        } else {
            std::stringstream rendered;
            it->print(rendered);

            session.reporter << UnresolvedReferenceDiagnostic{
                .filename = "[MISSING_FILENAME]",
                .line_number = 0,
                .range = {0, 0},
                .visualization = "[MISSING_VISUALIZATION]",
                .accessor = rendered.str()
            };

            declarations.push($ TypeNode{
                .identifier = $ IdentifierNode{"[UNRESOLVED_REFERENCE]"},
                .subtypes = $ NodeList()
            });
        }
    }

    virtual void visit(AST::DetailedNode<AST::CharacterLiteralNode> * it) override {
        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"Char"},
            .subtypes = $ NodeList()
        });
    }

    virtual void visit(AST::DetailedNode<AST::IdentifierNode> * it) override {
        auto that = scopes.top()->resolve(session, it);

        if (that != nullptr) {
            auto type = extract_type_node(that);

            if (type != nullptr) {
                declarations.push($ TypeNode{
                    .identifier = type->details.identifier,
                    .subtypes = type->details.subtypes,
                    .declaration = that
                });
            } else {
                std::stringstream rendered;
                it->print(rendered);

                session.reporter << InaccessibleTypeInformationDiagnostic{
                    .filename = "[MISSING_FILENAME]",
                    .line_number = 0,
                    .range = {0, 0},
                    .visualization = "[MISSING_VISUALIZATION]",
                    .accessor = rendered.str()
                };

                declarations.push($ TypeNode{
                    .identifier = $ IdentifierNode{"[DECLARATION_WITHOUT_TYPE]"},
                    .subtypes = $ NodeList()
                });
            }
        } else {
            std::stringstream rendered;
            it->print(rendered);

            session.reporter << UnresolvedReferenceDiagnostic{
                .filename = "[MISSING_FILENAME]",
                .line_number = 0,
                .range = {0, 0},
                .visualization = "[MISSING_VISUALIZATION]",
                .accessor = rendered.str()
            };

            declarations.push($ TypeNode{
                .identifier = $ IdentifierNode{"[UNRESOLVED_REFERENCE]"},
                .subtypes = $ NodeList()
            });
        }
    }

    virtual void visit(AST::DetailedNode<AST::NumberLiteralNode> * it) override {
        if (std::holds_alternative<int>(it->details.calculated)) {
            declarations.push($ TypeNode{
                .identifier = $ IdentifierNode{"Int"},
                .subtypes = $ NodeList()
            });
        } else {
            declarations.push($ TypeNode{
                .identifier = $ IdentifierNode{"Real"},
                .subtypes = $ NodeList()
            });
        }
    }

    virtual void visit(AST::DetailedNode<AST::StringLiteralNode> * it) override {
        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"String"},
            .subtypes = $ NodeList()
        });
    }

    virtual void visit(AST::DetailedNode<AST::TypeNode> * it) override {
        auto that = extract<IdentifierNode>(it->details.identifier);

        if (it->details.subtypes->details.values.empty() && that != nullptr) {
            // delegate calculations
            that->accept(this);
        } else {
            std::cout << "!!DeepDeclarationResolver has no implementation for non-identifier type nodes names: `" << *it->details.identifier << "`!!" << std::endl;

            declarations.push($ TypeNode{
                .identifier = $ IdentifierNode{"[DIFFICULT_TYPE]"},
                .subtypes = $ NodeList()
            });
        }
    }

    virtual void visit(DetailedNode<FunctionStatementNode> * it) override {
        it->details.value_parameters->accept(this);
        declarations.pop();

        scopes.push(it->details.scope);
        it->details.body->accept(this);
        scopes.pop();

        if (it->details.return_type != nullptr) {
            declarations.pop();
            it->details.return_type->accept(this);
        }

        it->details.return_type = declarations.top();

        // REGISTER

        auto name = extract<IdentifierNode>(it->details.name);

        if (name != nullptr) {
            scopes.top()->add(name->details.value, it);
        } else {
            std::cout << "!!DeepDeclarationResolver encountered a non-identifier as a function name: `" << *it->details.name << "`!!" << std::endl;
        }
    }

    virtual void visit(DetailedNode<IfStatementNode> * it) override {
        it->details.condition->accept(this);
        declarations.pop();

        scopes.push(it->details.scope);

        it->details.on_true->accept(this);
        declarations.pop();

        if (it->details.on_else != nullptr) {
            it->details.on_else->accept(this);
            declarations.pop();
        }

        scopes.pop();

        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"Unit"},
            .subtypes = $ NodeList()
        });
    }

    virtual void visit(DetailedNode<WhileStatementNode> * it) override {
        it->details.condition->accept(this);

        scopes.push(it->details.scope);

        it->details.on_true->accept(this);

        scopes.pop();

        declarations.push($ TypeNode{
            .identifier = $ IdentifierNode{"Unit"},
            .subtypes = $ NodeList()
        });
    }
};


void cringe::resolve_deep_declarations(Session & session, AST::Node * node) {
    DeepDeclarationResolver resolver{session};
    node->accept(&resolver);
}
