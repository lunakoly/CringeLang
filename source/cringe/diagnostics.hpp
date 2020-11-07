// Copyright (C) 2020 luna_koly
//
// Declares cringe-specific diagnostics.


#pragma once

#include <orders/util/printable.hpp>
#include <orders/parsing/diagnostics.hpp>

namespace cringe {
    /**
     * 'dawd instead of 'd'.
     */
    struct SingleQuoteExpectedDiagnostic {
        __DIAGNOSTIC__

        /**
         * The character instead of `'`.
         */
        char found;
        /**
         * The whole error token
         * the user entered.
         */
        std::string whole_token;
    };

    /**
     * For weird tokens.
     */
    struct BadTokenDiagnostic {
        __DIAGNOSTIC__

        /**
         * The error token
         * the user entered.
         */
        std::string found;
    };

    /**
     * For weird indents.
     */
    struct InvalidIndentDiagnostic {
        __DIAGNOSTIC__

        /**
         * The error token
         * the user entered.
         */
        std::string found;
        /**
         * The new (bad) indent.
         */
        size_t indent_level;
        /**
         * The old (vaild) one.
         */
        size_t old_indent;
    };

    /**
     * For other but still weird indents.
     */
    struct UnexpectedIndentDiagnostic {
        __DIAGNOSTIC__

        /**
         * The error token
         * the user entered.
         */
        std::string found;
        /**
         * Actual.
         */
        std::string type;
    };

    /**
     * Should've been identifier but something
     * weird found.
     */
    struct AnotherTokenTypeExpectedDiagnostic {
        __DIAGNOSTIC__

        /**
         * Allows to show additional info.
         */
        enum class Hint {
            VARIABLE_DECLARATION, TYPE_DECLARATION, ASSIGNMENT,
            TERMINAL, CONSTANT_DECLARATION, FUNCTION_NAME
        };

        /**
         * The desired TokenType.
         */
        std::string expected;
        /**
         * The error token
         * the user entered.
         */
        std::string token;
        /**
         * A little more accurate description.
         */
        Hint hint;
    };

    /**
     * Unmatched brackets, etc.
     */
    struct OperatorExpectedDiagnostic {
        __DIAGNOSTIC__

        /**
         * Allows for more descriptive messages.
         */
        enum class Hint {
            NESTED_EXPRESSION, TYPE_PAREMETERS, COMPOUND_TYPE_LIST,
            TYPE_EQUALS, ASSIGNMENT, CLOSING_PARAMETERS
        };

        /**
         * The missing operator.
         */
        std::string operator_token;
        /**
         * More accurate description of
         * what happened.
         */
        Hint hint;
    };

    /**
     * Expression expected.
     */
    struct ExpressionExpectedDiagnostic {
        __DIAGNOSTIC__

        /**
         * The error token
         * the user entered.
         */
        std::string found;
    };

    /**
     * User attempts to redeclare a type.
     */
    struct TypeRedeclarationDiagnostic {
        __DIAGNOSTIC__

        /**
         * The name of the type.
         */
        std::string name;
    };
}
