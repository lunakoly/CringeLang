// Copyright (C) 2020 luna_koly
//
// Analyzer with some handy functions designed
// to simplify the lexical analysis.


#pragma once

#include "stream.hpp"

#include <string>


namespace orders {
    /**
     * Stream with an accumulator that saves
     * every single character it has seen.
     */
    struct AccumulatorStream : public virtual Stream<int> {
        /**
         * Clears the interal lexeme.
         */
        virtual void clear() = 0;

        /**
         * Returns the string starting from
         * the `position` and up to the current
         * position.
         */
        virtual std::string revise(size_t position) const = 0;

        /**
         * Returns the whole string.
         */
        virtual std::string revise_all() const = 0;

        /**
         * Steps if `peek()` equals `next`.
         */
        bool accept(char next) {
            if (peek() == next) {
                step();
            }
        }
    };
}
