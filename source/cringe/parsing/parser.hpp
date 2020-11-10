// Copyright (C) 2020 luna_koly
//
// The parser for the cringe language.


#pragma once


#include "../session.hpp"
#include "../ast/nodes.hpp"


namespace cringe {
    /**
     * Builds an abstract syntax tree for the single file.
     */
    AST::DetailedNode<AST::FileNode> * parse_file(Session & session, const std::string & filename);
    /**
     * Builds an abstract syntax tree for all the files.
     */
    AST::DetailedNode<AST::GlobalNode> * parse_files(Session & session, const std::vector<std::string> & filenames);
}
