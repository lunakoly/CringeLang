#include "diagnostics.hpp"


using namespace orders;
using namespace cringe;


#define __PRINT_DIAGNOSTIC__(T) __IMPLEMENT_PRINT__(DetailedDiagnostic<T>)


__PRINT_DIAGNOSTIC__(BadTokenDiagnostic) {
    return __DIAGNOSTIC_HEADER__
        << " > Something weird found: `" << details.found << "`. Is it yours?";
}


__PRINT_DIAGNOSTIC__(InvalidIndentDiagnostic) {
    return __DIAGNOSTIC_HEADER__
        << " > Invalid indent level > `" << details.indent_level
        << "` spaces indent can't follow a(n) `" << details.old_indent
        << "` spaces one at `" << details.found << "`.";
}


__PRINT_DIAGNOSTIC__(UnexpectedIndentDiagnostic) {
    return __DIAGNOSTIC_HEADER__
        << " > Unexpected indent level > `" << details.type << "` shouldn't go here.";
}


__PRINT_DIAGNOSTIC__(SingleQuoteExpectedDiagnostic) {
    return __DIAGNOSTIC_HEADER__
        << " > Expected `'`, but `"
        << details.found << "` found in `" << details.whole_token << "`. Otherwise you "
        << "have a character literal declaration without a closing `'`.";
}


__PRINT_DIAGNOSTIC__(AnotherTokenTypeExpectedDiagnostic) {
    using Hint = AnotherTokenTypeExpectedDiagnostic::Hint;

    switch (details.hint) {
        case Hint::VARIABLE_DECLARATION:
            output << "Error in the variable declaration > ";
            break;
        case Hint::TYPE_DECLARATION:
            output << "Error in the type declaration > ";
            break;
        case Hint::ASSIGNMENT:
            output << "Error in the assignment statement > ";
            break;
        case Hint::TERMINAL:
            output << "Error in the expression > ";
            break;
        case Hint::CONSTANT_DECLARATION:
            output << "Error in the constant declaration > ";
            break;
        case Hint::FUNCTION_NAME:
            output << "Error in the function name > ";
            break;
    }

    return __DIAGNOSTIC_HEADER__
        << "`" << details.expected << "` expected, but `" << details.token << "` found.";
}


__PRINT_DIAGNOSTIC__(OperatorExpectedDiagnostic) {
    using Hint = OperatorExpectedDiagnostic::Hint;

    switch (details.hint) {
        case Hint::NESTED_EXPRESSION:
            output << "Unclosed parentheses in the nested expression";
            break;
        case Hint::TYPE_PAREMETERS:
            output << "Unclosed type parameters list";
            break;
        case Hint::COMPOUND_TYPE_LIST:
            output << "Unclosed parentheses in the types list";
            break;
        case Hint::TYPE_EQUALS:
            output << "Typealias declaration must specify a value via the `=` sing";
            break;
        case Hint::ASSIGNMENT:
            output << "Assignment statement must specify its value(s) via the `=` sing";
            break;
        case Hint::CLOSING_PARAMETERS:
            output << "Unclosed parentheses in the function parameters list";
            break;
    }

    return __DIAGNOSTIC_HEADER__
        << " > `" << details.operator_token << "` expected.";
}


__PRINT_DIAGNOSTIC__(ExpressionExpectedDiagnostic) {
    return __DIAGNOSTIC_HEADER__
        << " > Expression expected but `" << details.found << "` found.";
}


__PRINT_DIAGNOSTIC__(TypeRedeclarationDiagnostic) {
    return __DIAGNOSTIC_HEADER__
        << " > Type `" << details.name << "` has already been declared.";
}
