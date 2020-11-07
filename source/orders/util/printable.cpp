#include "printable.hpp"


std::ostream & operator << (std::ostream & output, const orders::Printable & self) {
    return self.print(output);
}
