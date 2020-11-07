// Copyright (C) 2020 luna_koly
//
// When reading from a stream we might want
// to look ahead some values. The idea behind
// BufferedStream is pre-loading several
// values ahead.


#pragma once

#include <memory>

#include "stream.hpp"
#include "../util/printable.hpp"


namespace orders {
    /**
     * Stream with lookaheads. Reads
     * a bunch of extra values beforehand.
     */
    template <typename T>
    struct BufferedStream : public virtual Stream<T>, public virtual Printable {
        __WITH_PURE_PRINT__

        /**
         * Allows to look at the specified
         * position in the buffer.
         */
        virtual T lookahead(size_t position) const = 0;
    };

    /**
     * Buffered stream that works with
     * characters.
     */
    struct TextStream : public virtual BufferedStream<int> {
        /**
         * Builds a string from the buffer contents.
         */
        virtual std::string get_text() const = 0;

        /**
         * Returns the number of matching chars
         * if contents of `next` match contents
         * of the inner buffer or 0 othersize.
         * Length of `next` must be <= buffer size.
         */
        virtual size_t match(const char * next) const = 0;
    };
}
