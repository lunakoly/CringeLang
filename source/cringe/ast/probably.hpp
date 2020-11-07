// Copyright (C) 2020 luna_koly
//
// A simple wrapper that simplifies
// returning either a valid Node or
// an ErrorNode.


#pragma once

#include "visitor.hpp"

#include <variant>


namespace cringe {
    namespace AST {
        /**
         * A wrapper that either contains a
         * T node or an ErrorNode.
         */
        template <typename T>
        class Probably {
        public:
            /**
             * Just get whatever is stored here.
             */
            Node * const any;
            /**
             * True if any is T *.
             */
            const bool is_ok;

            Probably(DetailedNode<T> * node) : is_ok(true), safe_accessor(node), any(node) {}

            Probably(DetailedNode<ErrorNode> * node) : is_ok(false), safe_accessor(node), any(node) {}

            Probably(T && it) : any(new DetailedNode<T>(it)), is_ok(true), safe_accessor((DetailedNode<T> *) any) {}

            Probably(ErrorNode && it) : any(new DetailedNode<ErrorNode>(it)), is_ok(true), safe_accessor((DetailedNode<ErrorNode> *) any) {}

            /**
             * Returns the T node if `is_ok`.
             */
            DetailedNode<T> * get_node() const {
                return std::get<DetailedNode<T> *>(safe_accessor);
            }

            /**
             * Returns the ErrorNode node if not `is_ok`.
             */
            DetailedNode<ErrorNode> * get_error() const {
                return std::get<DetailedNode<ErrorNode> *>(safe_accessor);
            }

        private:
            /**
             * Handles access to the wrong node.
             */
            const std::variant<DetailedNode<T> *, DetailedNode<ErrorNode> *> safe_accessor;
        };
    }
}
