// Copyright (C) 2020 luna_koly
//
// Orders-specific parsing data.


#pragma once

#include <orders/parsing/diagnostics.hpp>
#include <orders/streams/implementations/analyzable_stream.hpp>


namespace cringe {
    /**
     * Stores the data nessesary for
     * the whole compilation process.
     */
    struct Session {
        /**
         * CLI-parameters, etc.
         */
        struct Options {
            /**
             * Number of spaces per tabulation.
             */
            const int tab_size = 4;
        } options;

        /**
         * Collects diagnostics.
         */
        orders::DiagnosticReporter reporter;
    };
}
