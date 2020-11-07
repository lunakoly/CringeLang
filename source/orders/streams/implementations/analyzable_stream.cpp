#include "analyzable_stream.hpp"


orders::AnalyzableStream::AnalyzableStream(Stream<int> & backend, size_t buffer_size, size_t buffer_indent)
    : SimpleTextStream(backend, buffer_size, buffer_indent)
    {}

void orders::AnalyzableStream::clear() {
    accumulator.str(std::string());
}

std::string orders::AnalyzableStream::revise(size_t position) const {
    auto contents = accumulator.str();
    auto distance = /*SimpleBufferedStream<int>::*/get_offset() - position;
    return contents.substr(contents.size() - distance, distance);
}

std::string orders::AnalyzableStream::revise_all() const {
    return accumulator.str();
}


// getting rid of the `inherits via dominance` warning


int orders::AnalyzableStream::get_end_value() const {
    return SimpleTextStream::get_end_value();
}

int orders::AnalyzableStream::peek() {
    return SimpleTextStream::peek();
}

bool orders::AnalyzableStream::has_next() {
    return SimpleTextStream::has_next();
}

void orders::AnalyzableStream::step() {
    accumulator << (char) peek();
    return SimpleTextStream::step();
}

void orders::AnalyzableStream::step(size_t count) {
    return Stream::step(count);
}

size_t orders::AnalyzableStream::get_offset() const {
    return SimpleTextStream::get_offset();
}
