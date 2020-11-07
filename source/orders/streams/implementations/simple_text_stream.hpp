// Copyright (C) 2020 luna_koly
//
// Some handy stream implementation.


#pragma once

#include "simple_buffered_stream.hpp"

#include <sstream>


namespace orders {
    /**
     * A trivial implementation of a TextStream.
     */
    class SimpleTextStream : public SimpleBufferedStream<int>, public virtual TextStream {
    public:
        /**
         * Accepts the size of internal buffer
         * storage.
         */
        SimpleTextStream(Stream<int> & backend, size_t buffer_size = 16, size_t buffer_indent = 5);

        virtual std::string get_text() const override;

        virtual size_t match(const char * next) const override;

        // getting rid of the `inherits via dominance` warning

        virtual int get_end_value() const override;

        virtual int peek() override;

        virtual bool has_next() override;

        virtual void step() override;

        virtual size_t get_offset() const override;

        virtual int lookahead(size_t position) const override;

        __WITH_CUSTOM_PRINT__
    };
}
