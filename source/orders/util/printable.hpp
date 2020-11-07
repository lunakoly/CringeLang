// Copyright (C) 2020 luna_koly
//
// A level of indirection that simplifies
// printing custom objects.


#pragma once

#include <ostream>


namespace orders {
    /**
     * Forces the class to implement
     * a print() method that is used
     * under the hood for printing via
     * operator<< or other mechanisms.
     */
    struct Printable {
        virtual ~Printable() {}

        /**
         * Prints the object representation
         * to the std::ostream.
         */
        virtual std::ostream & print(std::ostream & output) const = 0;
    };
}


std::ostream & operator << (std::ostream & output, const orders::Printable & self);

#define __WITH_CUSTOM_PRINT__ virtual std::ostream & print(std::ostream & output) const override;
#define __WITH_PURE_PRINT__ virtual std::ostream & print(std::ostream & output) const = 0;
#define __IMPLEMENT_PRINT__(T) std::ostream & T::print(std::ostream & output) const
