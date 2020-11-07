// Copyright (C) 2020 luna_koly
//
// Some handy stream implementation.


#pragma once

#include "../buffered_stream.hpp"

#include <sstream>


namespace orders {
    /**
     * A trivial implementation of a BufferedStream<T>.
     */
    template <typename T>
    class SimpleBufferedStream : public virtual BufferedStream<T> {
    public:
        __WITH_CUSTOM_PRINT__

        /**
         * Size of internal storage.
         */
        const size_t buffer_size;
        /**
         * The amount of positions peek_index
         * is ahead of fill_index.
         */
        const size_t buffer_indent;

        /**
         * Accepts the size of internal buffer
         * storage.
         */
        SimpleBufferedStream(Stream<T> & backend, size_t buffer_size = 16, size_t buffer_indent = 5);

        virtual T get_end_value() const override;

        virtual T peek() override;

        virtual bool has_next() override;

        virtual void step() override;

        virtual size_t get_offset() const override;

        virtual T lookahead(size_t position) const override;

    protected:
        /**
         * The stream used as a backend.
         */
        Stream<T> & backend;
        /**
         * Points to the element that
         * will be returned at the next
         * peek() call. peek_index should
         * cycle over the buffer.
         */
        size_t peek_index;
        /**
         * Points to the element that
         * will be updated after the next step.
         * fill_index should cycle over the buffer.
         */
        size_t fill_index = 0;
        /**
         * Internal storage for collecting
         * lookahead values.
         */
        std::unique_ptr<T[]> buffer;
    };
}


template <typename T>
orders::SimpleBufferedStream<T>::SimpleBufferedStream(Stream<T> & backend, size_t buffer_size, size_t buffer_indent)
    : backend(backend)
    , buffer_size(buffer_size)
    , peek_index(buffer_indent)
    , buffer_indent(buffer_indent)
    , buffer(std::make_unique<T[]>(buffer_size)) {
    for (auto it = peek_index; it < buffer_size; it++) {
        buffer[it] = backend.peek();
        backend.step();
    }
}

template <typename T>
T orders::SimpleBufferedStream<T>::get_end_value() const {
    return backend.get_end_value();
}

template <typename T>
T orders::SimpleBufferedStream<T>::peek() {
    return buffer[peek_index];
}

template <typename T>
bool orders::SimpleBufferedStream<T>::has_next() {
    return buffer[peek_index] != backend.get_end_value();
}

template <typename T>
void orders::SimpleBufferedStream<T>::step() {
    buffer[fill_index] = backend.peek();
    backend.step();

    if (peek_index != buffer_size - 1) {
        peek_index++;
    } else {
        peek_index = 0;
    }

    if (fill_index != buffer_size - 1) {
        fill_index++;
    } else {
        fill_index = 0;
    }
}

template <typename T>
size_t orders::SimpleBufferedStream<T>::get_offset() const {
    return backend.get_offset() - buffer_size;
}

template <typename T>
__IMPLEMENT_PRINT__(orders::SimpleBufferedStream<T>) {
    output << "SimpleBufferedStream [";

    for (size_t it = fill_index; it < buffer_size; it++) {
        output << buffer[it] << ", ";
    }

    for (size_t it = 0; it < fill_index; it++) {
        output << buffer[it] << ", ";
    }

    return output << "]";
}

template <typename T>
T orders::SimpleBufferedStream<T>::lookahead(size_t position) const {
    auto index = position + peek_index;

    if (index >= buffer_size) {
        index -= buffer_size;
    }

    return buffer[index];
}
