#include "simple_text_stream.hpp"

#include <sstream>


orders::SimpleTextStream::SimpleTextStream(Stream<int> & backend, size_t buffer_size, size_t buffer_indent)
    : SimpleBufferedStream(backend, buffer_size, buffer_indent)
    {}

size_t orders::SimpleTextStream::match(const char * next) const {
    for (size_t it = peek_index; it < buffer_size; it++) {
        if (*next != '\0') {
            if (*next == buffer[it]) {
                next += 1;
            } else {
                return 0;
            }
        } else {
            return it - peek_index;
        }
    }

    for (size_t it = 0; it < peek_index; it++) {
        if (*next != '\0') {
            if (*next == buffer[it]) {
                next += 1;
            } else {
                return false;
            }
        } else {
            return it + (buffer_size - peek_index);
        }
    }

    return buffer_size;
}

std::string orders::SimpleTextStream::get_text() const {
    std::stringstream result;

    for (size_t it = fill_index; it < buffer_size; it++) {
        result << (char) buffer[it];
    }

    for (size_t it = 0; it < fill_index; it++) {
        result << (char) buffer[it];
    }

    return result.str();
}


// getting rid of the `inherits via dominance` warning


int orders::SimpleTextStream::get_end_value() const {
    return SimpleBufferedStream<int>::get_end_value();
}

int orders::SimpleTextStream::peek() {
    return SimpleBufferedStream<int>::peek();
}

bool orders::SimpleTextStream::has_next() {
    return SimpleBufferedStream<int>::has_next();
}

void orders::SimpleTextStream::step() {
    return SimpleBufferedStream<int>::step();
}

size_t orders::SimpleTextStream::get_offset() const {
    return SimpleBufferedStream<int>::get_offset();
}

int orders::SimpleTextStream::lookahead(size_t position) const {
    return SimpleBufferedStream<int>::lookahead(position);
}

__IMPLEMENT_PRINT__(orders::SimpleTextStream) {
    return SimpleBufferedStream<int>::print(output);
}
