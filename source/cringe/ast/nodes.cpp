#include "nodes.hpp"


using namespace cringe;
using namespace cringe::AST;


#define __PRINT_NODE__(T) __IMPLEMENT_PRINT__(DetailedNode<T>)


__PRINT_NODE__(NodeList) {
    output << "[";

    if (details.values.size() > 0) {
        for (size_t it = 0; it < details.values.size() - 1; it++) {
            output << *details.values[it] << ", ";
        }

        output << *details.values.back();
    }

    return output << "]";
}


__PRINT_NODE__(ErrorNode) {
    return output << "<!ERROR!>" << details.value << "<!>";
}


__PRINT_NODE__(GlobalNode) {
    auto files = details.files->details.values;

    if (!files.empty()) {
        for (size_t it = 0; it < files.size() - 1; it++) {
            output << *files[it] << std::endl;
        }

        return output << *files.back();
    }

    return output << "<!NO_FILES!><!>";
}


__PRINT_NODE__(FileNode) {
    return output
        << "*** FILE " << details.filename << " ***" << std::endl
        << *details.root;
}


__PRINT_NODE__(ConstantDeclarationNode) {
    output << "let ";

    if (details.constants != nullptr) {
        output << *details.constants;
    } else {
        output << "<!MISSING CONSTANTS LIST!><!>";
    }

    if (details.type != nullptr) {
        output << ": " << *details.type;
    } else {
        output << ": <!MISSING TYPE!><!>";
    }

    if (details.values != nullptr) {
        output << " = " << *details.values;
    }

    return output;
}


__PRINT_NODE__(TypealiasDeclarationNode) {
    output << "typealias ";

    if (details.type != nullptr) {
        output << *details.type;
    } else {
        output << "<!MISSING TYPE!><!>";
    }

    output << " = ";

    if (details.value != nullptr) {
        output << *details.value;
    } else {
        output << "<!MISSING VALUE!><!>";
    }

    return output;
}


__PRINT_NODE__(VariableDeclarationNode) {
    output << "var ";

    if (details.variables != nullptr) {
        output << *details.variables;
    } else {
        output << "<!MISSING VARIABLES LIST!><!>";
    }

    if (details.type != nullptr) {
        output << ": " << *details.type;
    } else {
        output << ": <!MISSING TYPE!><!>";
    }

    if (details.values != nullptr) {
        output << " = " << *details.values;
    }

    return output;
}


__PRINT_NODE__(BinaryExpressionNode) {
    output << '(';

    if (details.left != nullptr) {
        output << *details.left;
    } else {
        output << "<!MISSING EXPRESSION!><!>";
    }

    output << ' ' << details.operator_token << ' ';

    if (details.right != nullptr) {
        output << *details.right;
    } else {
        output << "<!MISSING EXPRESSION!><!>";
    }

    return output << ')';
}


__PRINT_NODE__(UnaryExpressionNode) {
    output << '(' << details.operator_token;

    if (details.target != nullptr) {
        output << *details.target;
    } else {
        output << "<!MISSING EXPRESSION!><!>";
    }

    return output << ')';
}


__PRINT_NODE__(QualifiedAccessNode) {
    auto & identifiers = details.identifiers->details.values;

    if (identifiers.size() > 0) {
        for (size_t it = 0; it < identifiers.size() - 1; it++) {
            output << *identifiers[it] << ".";
        }

        return output << *identifiers.back();
    }

    return output << "<!MISSING_QUALIFIED_ACCESS!><!>";
}


__PRINT_NODE__(CharacterLiteralNode) {
    return output << details.value;
}


__PRINT_NODE__(IdentifierNode) {
    return output << details.value;
}


__PRINT_NODE__(NumberLiteralNode) {
    if (std::holds_alternative<int>(details.calculated)) {
        return output << std::get<int>(details.calculated);
    }

    return output << std::get<double>(details.calculated);
}


__PRINT_NODE__(StringLiteralNode) {
    return output << "\"" << details.value << "\"";
}


__PRINT_NODE__(TypeNode) {
    if (details.identifier != nullptr) {
        output << *details.identifier;
    } else {
        output << "<!MISSING TYPE!><!>";
    }

    if (details.subtypes != nullptr) {
        if (details.subtypes->details.values.size() > 0) {
            output << '<' << *details.subtypes << '>';
        }
    } else {
        output << "<!MISSING SUBTYPE LIST!><!>";
    }

    return output;
}


__PRINT_NODE__(FunctionStatementNode) {
    output << "fun ";

    if (details.name != nullptr) {
        output << *details.name << ' ';
    } else {
        output << "<!MISSING NAME!><!>";
    }

    if (details.value_parameters != nullptr) {
        output << '(' << *details.value_parameters << ')';
    } else {
        output << "(<!MISSING VALUE PARAMETERS!><!>)";
    }

    if (details.return_type != nullptr) {
        output << ": " << *details.return_type;
    } else {
        output << ": <!MISSING RETURN TYPE!><!>";
    }

    if (details.body != nullptr) {
        output << " " << *details.body;
    } else {
        output << " <!MISSING BODY!><!>";
    }

    return output;
}


__PRINT_NODE__(IfStatementNode) {
    output << "if ";

    if (details.condition != nullptr) {
        output << *details.condition << ' ';
    } else {
        output << "<!MISSING CONDITION!><!>";
    }

    if (details.on_true != nullptr) {
        output << *details.on_true;
    } else {
        output << "<!MISSING ON_TRUE!><!>";
    }

    if (details.on_else != nullptr) {
        output << " else " << *details.on_else;
    }

    return output;
}


__PRINT_NODE__(WhileStatementNode) {
    output << "while ";

    if (details.condition != nullptr) {
        output << *details.condition << ' ';
    } else {
        output << "<!MISSING CONDITION!><!>";
    }

    if (details.on_true != nullptr) {
        output << *details.on_true;
    } else {
        output << "<!MISSING ON_TRUE!><!>";
    }

    return output;
}
