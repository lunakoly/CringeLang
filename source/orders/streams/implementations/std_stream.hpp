// Copyright (C) 2020 luna_koly
//
// A trivial Stream<int> implementation based
// on an std::istream instance. Why not Stream<char>?
// because EOF is (-1) and char may be the unsigned char.


#pragma once

#include <istream>

#include "../stream.hpp"


namespace orders {
    /**
     * Trivial Stream<int> implementation that uses
     * the std::istream as the backend.
     */
    class StdStream : public virtual Stream<int> {
    public:
        /**
         * Accepts a std::istream used as the backend.
         * If insert_newline is true, puts additional
         * `\n` character into the stream.
         */
        StdStream(std::istream & backend, bool insert_newline = false);

        virtual int get_end_value() const override;

        virtual int peek() override;

        virtual void step() override;

        virtual size_t get_offset() const override;

    private:
        /**
         * Number of read values.
         */
        size_t offset = 0;
        /*
        * The stream that provides the data.
        */
        std::istream & backend;
    };
}
