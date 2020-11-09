// Copyright (C) 2020 luna_koly
//
// Some abstraction for representing analysis
// errors. We have 'diagnostics' that may be
// 'reported' by some 'reporter'.


#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "../util/printable.hpp"

#define __DIAGNOSTIC__                                      \
    /**                                                     \
     * Full path to the input file.                         \
     */                                                     \
    std::string filename;                                   \
    /**                                                     \
     * The line the error was found at.                     \
     */                                                     \
    size_t line_number;                                     \
    /**                                                     \
     * Some place.                                          \
     */                                                     \
    orders::Range range;                              \
    /**                                                     \
     * Well, maybe range is fine enough                     \
     * and we could run through the file once               \
     * more to grab parts of text described by it,          \
     * but because there's no such an infrastructure for    \
     * now, I'll go with this approach.                     \
     */                                                     \
    std::string visualization;

#define __DIAGNOSTIC_HEADER__                                                                 \
    output                                                                                    \
        << details.visualization << '\n'                                                      \
        << "Quick Link > " << details.filename << "(" << details.line_number << ",1)" << '\n' \
        << "Error > "


namespace orders {
    /**
     * A part of the input source
     * code.
     */
    struct Range {
        size_t start;
        size_t stop;
    };

    /**
     * Represents a diagnostic reported
     * by lexer/parser/etc.
     */
    struct Diagnostic : public Printable {
        __WITH_PURE_PRINT__

        virtual ~Diagnostic() {}

        /**
         * Returns the line the diagnostic was
         * reported at.
         */
        virtual size_t get_line_number() = 0;

        /**
         * Returns the character range the
         * diagnostic was reported at.
         */
        virtual Range get_range() = 0;
    };

    /**
     * Represents a diagnostic reported
     * by lexer/parser/etc.
     */
    template <typename T>
    struct DetailedDiagnostic : public Diagnostic {
        // there's no definition for abstract T
        // but rather there're definitions for each
        // particular type variant
        __WITH_CUSTOM_PRINT__

        /**
         * More useful data.
         */
        T details;

        DetailedDiagnostic(T && details) : details(details) {}

        virtual size_t get_line_number() override {
            return details.line_number;
        }

        virtual Range get_range() override {
            return details.range;
        }
    };

    /**
     * Collects diagnostics.
     */
    struct DiagnosticReporter {
        /**
         * Diagnostics indicating
         * some warnings/errors.
         */
        std::vector<Diagnostic *> diagnostics;

        /**
         * Adds the diagnostic to the inner
         * list of diagnostics.
         */
        template <typename D>
        void report(D && diagnostic) {
            auto it = new DetailedDiagnostic<D>{std::move(diagnostic)};
            diagnostics.push_back(it);
        }

        /**
         * A handy shortcut.
         */
        template <typename D>
        void operator << (D && diagnostic) {
            report(std::move(diagnostic));
        }
    };
}


std::ostream & operator << (std::ostream & output, const orders::Range & self);
