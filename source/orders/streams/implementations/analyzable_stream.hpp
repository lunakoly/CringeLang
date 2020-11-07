// Copyright (C) 2020 luna_koly
//
// Some handy stream implementation.


#pragma once

#include "simple_text_stream.hpp"
#include "../accumulator_stream.hpp"

#include <sstream>


namespace orders {
    class AnalyzableStream : public SimpleTextStream, public virtual AccumulatorStream {
    public:
        /**
         * Accepts the size of internal buffer
         * storage.
         */
        AnalyzableStream(Stream<int> & backend, size_t buffer_size = 16, size_t buffer_indent = 5);

        virtual void clear() override;

        virtual std::string revise(size_t position) const override;

        virtual std::string revise_all() const override;

        // getting rid of the `inherits via dominance` warning

        virtual int get_end_value() const override;

        virtual int peek() override;

        virtual bool has_next() override;

        virtual void step() override;

        virtual void step(size_t count) override;

        virtual size_t get_offset() const override;

    protected:
        /**
         * Accumulates the characters.
         */
        std::stringstream accumulator;
    };
}
