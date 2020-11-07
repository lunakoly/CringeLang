#include "std_stream.hpp"

#include <istream>


orders::StdStream::StdStream(std::istream & backend, bool insert_newline) : backend(backend) {
    if (insert_newline) {
        backend.putback('\n');
    }
}

int orders::StdStream::get_end_value() const {
    return EOF;
}

int orders::StdStream::peek() {
    return backend.peek();
}

void orders::StdStream::step() {
    backend.get();
    offset++;
}

size_t orders::StdStream::get_offset() const {
    return offset;
}
