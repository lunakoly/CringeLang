// Copyright (C) 2020 luna_koly
//
// Orders-specific parsing data.


#pragma once

#include <orders/parsing/diagnostics.hpp>
#include <orders/streams/implementations/analyzable_stream.hpp>

#include <threading/thread_pool.hpp>


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
             * The language version to use.
             */
            const std::string std;
            /**
             * Number of spaces per tabulation.
             */
            const int tab_size = 4;
            /**
             * Don't use thread pool for various stages.
             */
            const bool no_parallel = false;
        } options;

        /**
         * Collects diagnostics.
         */
        orders::DiagnosticReporter reporter;
        /**
         * If parallel compilation is allowed,
         * this is where the pool lives.
         */
        threading::ThreadPool * pool = nullptr;
    };
}
