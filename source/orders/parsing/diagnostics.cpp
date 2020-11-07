#include "diagnostics.hpp"


std::ostream & operator << (std::ostream & output, const orders::Range & self) {
    return output << '[' << self.start << ", " << self.stop << ')';
}
