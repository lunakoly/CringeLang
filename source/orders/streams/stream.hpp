// Copyright (C) 2020 luna_koly
//
// This file declares common a interface
// for stream-like data. For example, when
// dealing with a file we access it's contents
// via character stream. Lexer can also be
// treaded as a token stream but this is not
// the way they are used here.


#pragma once


namespace orders {
    /**
     * A sequence of some values of
     * type T.
     */
    template <typename T>
    struct Stream {
        virtual ~Stream() {}

        /**
         * Returns the value meaning
         * the end of the stream.
         */
        virtual T get_end_value() const = 0;

        /**
         * Returns the next unread
         * item without going forward.
         * Should return a special value
         * meaning "the end" if has_next()
         * is false.
         */
        virtual T peek() = 0;

        /**
         * Returns true if there're some
         * other values left.
         */
        virtual bool has_next() {
            return peek() == get_end_value();
        };

        /**
         * Skips the current item.
         */
        virtual void step() = 0;

        /**
         * Skips several items.
         */
        virtual void step(size_t count) {
            for (size_t it = 0; it < count; it++) {
                step();
            }
        }

        /**
         * Returns the next unread
         * item and goes forward
         * (peek() && step()).
         */
        virtual T grab() {
            auto it = peek();
            step();
            return it;
        }

        /**
         * Returns the number of read values.
         */
        virtual size_t get_offset() const = 0;
    };
}
