#include "parser.hpp"

#include "diagnostics.hpp"
#include "ast/probably.hpp"

#include <fstream>

#include <orders/streams/implementations/std_stream.hpp>
#include <orders/streams/implementations/analyzable_stream.hpp>


using namespace cringe;
using namespace cringe::AST;


/**
 * Removes the first and the last characters.
 */
std::string remove_quotes(const std::string & source) {
    if (source.length() >= 2) {
        return source.substr(1, source.size() - 2);
    }

    return std::string();
}


/**
 * Helps to convert individual T's
 * to their corresponding DetailedNode's.
 */
struct DetailsConverter {};

/**
 * Use `DETAILS << MyNode{}`.
 */
static DetailsConverter DETAILS;

/**
 * Even shorter variant...
 */
#define $ DETAILS <<
// or the more readable one...
#define detailed DETAILS <<

template <typename T>
DetailedNode<T> * operator << (const DetailsConverter & converter, T it) {
    return new DetailedNode{it};
}


/**
 * The actual parser.
 */
struct ParsingContextBackend {
    /**
     * The single place where all
     * available compilation information
     * lives.
     */
    Session & session;

    /**
     * Full path to the input file.
     */
    std::string filename;

    /**
     * A single file input.
     */
    orders::AnalyzableStream & input;

    /**
     * The line the token has
     * been met at.
     */
    size_t line_number = 0;

    /**
    * The current indent `depth`.
    */
    int indent_level = 0;

    /**
     * 0 - no indent changes
     * 1 - INDENT
     * 2 - DEDENT
     */
    int indent_index = 0;


    static bool is_non_operator(char next) {
        return
            next >= 'a' && next <= 'z' ||
            next >= 'A' && next <= 'Z' ||
            next >= '0' && next <= '9' ||
            next == '_';
    }

    static bool is_special(char next) {
        return
            next == '%'  ||
            next == '$'  ||
            next == '#'  ||
            next == '.'  ||
            next == '\"' ||
            next == '\'';
    }

    static bool is_blank(char next) {
        return
            next == '\t' ||
            next == '\r' ||
            next == '\n' ||
            next == ' ';
    }

    static bool is_space(char next) {
        return
            next == '\t' ||
            next == '\r' ||
            next == ' ';
    }

    static bool is_identifier_start(char next) {
        return
            next >= 'a' && next <= 'z' ||
            next >= 'A' && next <= 'Z' ||
            next == '_';
    }

    static bool is_binary_digit(char next) {
        return next >= '0' && next <= '1';
    }

    static bool is_octal_digit(char next) {
        return next >= '0' && next <= '7';
    }

    static bool is_decimal_digit(char next) {
        return next >= '0' && next <= '9';
    }

    static bool is_hexadecimal_uppercase_digit(char next) {
        return next >= 'A' && next <= 'F';
    }

    static bool is_hexadecimal_lowercase_digit(char next) {
        return next >= 'a' && next <= 'f';
    }


    void read_space() {
        while (
            input.has_next() &&
            is_space(input.peek())
        ) {
            input.step();
        }
    }

    int read_space_and_count() {
        int count = 0;

        while (
            input.has_next() &&
            is_space(input.peek())
        ) {
            if (input.peek() == ' ') {
                count++;
            } else if (input.peek() == '\t') {
                count += session.options.tab_size;
            }

            input.step();
        }

        return count;
    }

    void prepare() {
        read_space();

        auto new_level = indent_level;
        bool newline_found = false;

        while (input.peek() == '\n') {
            input.step();
            line_number++;
            newline_found = true;
            new_level = read_space_and_count();
        }

        if (newline_found) {
            if (new_level == indent_level + session.options.tab_size) {
                indent_index = 1;
                indent_level = new_level;
            } else if (new_level == indent_level - session.options.tab_size) {
                indent_index = 2;
                indent_level = new_level;
            } else {
                indent_index = 0;
            }
        }

        input.clear();
    }

    char read_character_representation() {
        if (input.peek() == '\n') {
            line_number += 1;
        }

        if (input.peek() == '\\') {
            input.step();

            if (input.peek() == 'n') {
                return '\n';
            } else if (input.peek() == 't') {
                return '\t';
            }
        }

        return input.grab();
    }


    std::string visualize() {
        std::stringstream result;
        std::stringstream highlight;
        auto contents = input.get_text();

        result << ' ' << line_number << " | ...";
        highlight << std::string(result.str().length(), ' ');

        for (size_t it = 0; it < input.buffer_indent; it++) {
            if (contents[it] == '\n') {
                result    << "<newline>";
                highlight << "         ";
            } else {
                result << contents[it];
                highlight << '~';
            }
        }

        result << contents[input.buffer_indent];
        highlight << '^';

        for (size_t it = input.buffer_indent + 1; it < input.buffer_size; it++) {
            if (contents[it] == '\n') {
                result    << "<newline>";
                highlight << "         ";
            } else {
                result << contents[it];
                highlight << '~';
            }
        }

        // just because I trim whitespaces
        // at the end of a string in my text editor
        // and I use this buffer visualization
        // to compare tests.
        auto highlight_result = highlight.str();
        size_t trim_index = highlight_result.size() - 1;

        while (
            trim_index >= 0 &&
            highlight_result[trim_index] == ' '
        ) {
            trim_index--;
        }

        result << "...\n" << highlight_result.substr(0, trim_index + 1);
        return result.str();
    }

    DetailedNode<ErrorNode> * read_error_end() {
        auto start = input.get_offset();
        auto visualization = visualize();

        while (is_non_operator(input.peek())) {
            input.step();
        }

        session.reporter << BadTokenDiagnostic{
            .filename = filename,
            .line_number = line_number,
            .range = {start, input.get_offset()},
            .visualization = visualization,
            .found = input.revise_all()
        };

        indent_index = 0;

        return $ ErrorNode{
            .value = input.revise_all()
        };
    }

    DetailedNode<ErrorNode> * read_error() {
        prepare();
        auto start = input.get_offset();
        auto visualization = visualize();

        while (
            input.has_next() &&
            !is_space(input.peek())
        ) {
            input.step();
        }

        session.reporter << BadTokenDiagnostic{
            .filename = filename,
            .line_number = line_number,
            .range = {start, input.get_offset()},
            .visualization = visualization,
            .found = input.revise_all()
        };

        indent_index = 0;

        return $ ErrorNode{
            .value = input.revise_all()
        };
    }


    int read_sign() {
        if (input.peek() == '-') {
            return -1;
        }

        if (input.peek() == '+') {
            return 1;
        }

        return 1;
    }

    struct Binary {
        static const int base = 2;

        static int decode(char it) {
            return it - '0';
        }

        static bool test(char it) {
            return is_binary_digit(it);
        }
    };

    struct Octal {
        static const int base = 8;

        static int decode(char it) {
            return it - '0';
        }

        static bool test(char it) {
            return is_octal_digit(it);
        }
    };

    struct Decimal {
        static const int base = 10;

        static int decode(char it) {
            return it - '0';
        }

        static bool test(char it) {
            return is_decimal_digit(it);
        }
    };

    struct Hexadecimal {
        static const int base = 16;

        static int decode(char it) {
            if (is_hexadecimal_uppercase_digit(it)) {
                return (it - 'A') + 10;
            } else if (is_hexadecimal_lowercase_digit(it)) {
                return (it - 'a') + 10;
            }

            return it - '0';
        }

        static bool test(char it) {
            return is_decimal_digit(it) || is_hexadecimal_lowercase_digit(it) || is_hexadecimal_uppercase_digit(it);
        }
    };

    template <typename T>
    int32_t read_integer() {
        int32_t value = 0;

        while (T::test(input.peek())) {
            value = (value * T::base) + T::decode(input.peek());
            input.step();
        }

        return value;
    }

    template <typename T>
    int32_t read_signed_integer() {
        return read_sign() * read_integer<T>();
    }

    template <typename T>
    double read_optional_fraction() {
        double value = 0.0;
        double scale = 1.0;

        if (input.peek() == '.') {
            input.step();

            while (T::test(input.peek())) {
                scale /= T::base;
                value += scale * T::decode(input.peek());
                input.step();
            }
        }

        return value;
    }

    template <typename T>
    int32_t read_optional_exponent() {
        if (
            input.peek() == 'e' ||
            input.peek() == 'E'
        ) {
            input.step();
            return read_signed_integer<T>();
        }

        return 0;
    }

    template <typename T>
    Probably<NumberLiteralNode> read_number() {
        int sign = read_sign();
        int32_t integer = read_integer<T>();
        double fraction = read_optional_fraction<T>();
        int32_t exponent = read_optional_exponent<T>();

        if (is_non_operator(input.peek())) {
            return read_error_end();
        }

        if (fraction == 0.0 && exponent == 0) {
            indent_index = 0;

            return $ NumberLiteralNode{
                .value = input.revise_all(),
                .calculated = integer
            };
        }

        indent_index = 0;

        return $ NumberLiteralNode{
            .value = input.revise_all(),
            .calculated = sign * (integer + fraction) * pow(T::base, (double) exponent)
        };
    }


    bool read_operator(const std::string & lexeme) {
        prepare();

        if (input.match(lexeme.c_str())) {
            input.step(lexeme.size());
            indent_index = 0;
            return true;
        }

        return false;
    }

    bool read_keyword(const std::string & lexeme) {
        prepare();

        auto tail = input.lookahead(lexeme.size());
        bool is_valid_tail = !is_non_operator(tail) || is_blank(tail) || is_special(tail);

        if (input.match(lexeme.c_str()) && is_valid_tail) {
            input.step(lexeme.size());
            indent_index = 0;
            return true;
        }

        return false;
    }

    bool is_identifier() {
        prepare();
        return is_identifier_start(input.peek());
    }

    Probably<IdentifierNode> read_identifier() {
        input.step();

        while (is_non_operator(input.peek())) {
            input.step();
        }

        indent_index = 0;

        return IdentifierNode{
            .value = input.revise_all()
        };
    }

    bool is_string() {
        prepare();
        return input.peek() == '\"';
    }

    Probably<StringLiteralNode> read_string() {
        input.step();

        while (
            input.has_next() &&
            input.peek() != '"'
        ) {
            read_character_representation();
        }

        input.step();

        if (is_non_operator(input.peek())) {
            return read_error_end();
        }

        indent_index = 0;

        return StringLiteralNode{
            .value = remove_quotes(input.revise_all())
        };
    }

    bool is_character() {
        prepare();
        return input.peek() == '\'';
    }

    Probably<CharacterLiteralNode> read_character() {
        input.step();

        read_character_representation();

        if (input.peek() != '\'') {
            auto it = input.peek();

            session.reporter << SingleQuoteExpectedDiagnostic{
                .filename = filename,
                .line_number = line_number,
                .range = {input.get_offset(), input.get_offset() + 1},
                .visualization = visualize(),
                .found = (char) it,
                .whole_token = '\'' + input.revise_all()
            };

            input.step();
            indent_index = 0;

            return ErrorNode{
                .value = input.revise_all()
            };
        }

        input.step();

        if (is_non_operator(input.peek())) {
            return read_error_end();
        }

        indent_index = 0;

        return CharacterLiteralNode{
            .value = remove_quotes(input.revise_all())
        };
    }

    bool is_binary() {
        prepare();
        return input.peek() == '%';
    }

    Probably<NumberLiteralNode> read_binary() {
        input.step();
        return read_number<Binary>();
    }

    bool is_octal() {
        prepare();
        return input.peek() == '$';
    }

    Probably<NumberLiteralNode> read_octal() {
        input.step();
        return read_number<Octal>();
    }

    bool is_decimal() {
        prepare();
        return is_decimal_digit(input.peek()) || input.peek() == '-' || input.peek() == '.';
    }

    Probably<NumberLiteralNode> read_decimal() {
        return read_number<Decimal>();
    }

    bool is_hexadecimal() {
        prepare();
        return input.peek() == '#';
    }

    Probably<NumberLiteralNode> read_hexadecimal() {
        input.step();
        return read_number<Hexadecimal>();
    }

    bool read_indent() {
        prepare();

        if (indent_index == 1) {
            indent_index = 0;
            return true;
        }

        return false;
    }

    bool read_dedent() {
        prepare();

        if (indent_index == 2) {
            indent_index = 0;
            return true;
        }

        return false;
    }

    bool read_end() {
        prepare();
        return !input.has_next();
    }


    void match(const char * operator_token, OperatorExpectedDiagnostic::Hint hint) {
        if (!read_operator(operator_token)) {
            auto start = input.get_offset();
            auto visualization = visualize();
            auto it = read_error();

            session.reporter << OperatorExpectedDiagnostic{
                .filename = filename,
                .line_number = line_number,
                .range = {start, input.get_offset()},
                .visualization = visualization,
                .operator_token = operator_token,
                .hint = hint
            };
        }
    }


    Probably<IdentifierNode> parse_identifier(AnotherTokenTypeExpectedDiagnostic::Hint error_hint) {
        if (is_identifier()) {
            return read_identifier();
        }

        auto start = input.get_offset();
        auto visualization = visualize();
        auto error = read_error();

        session.reporter << AnotherTokenTypeExpectedDiagnostic{
            .filename = filename,
            .line_number = line_number,
            .range = {start, input.get_offset()},
            .visualization = visualization,
            .expected = "IDENTIFIER",
            .token = error->details.value,
            .hint = error_hint
        };

        return error;
    }


    DetailedNode<NodeList> * parse_identifier_list(AnotherTokenTypeExpectedDiagnostic::Hint error_hint) {
        auto it = NodeList();
        it.values.push_back(parse_identifier(error_hint).any);

        while (read_operator(",")) {
            it.values.push_back(parse_identifier(error_hint).any);
        }

        return $ it;
    }

    Node * parse_qualified_access(AnotherTokenTypeExpectedDiagnostic::Hint error_hint) {
        Node * it = parse_identifier(error_hint).any;

        if (read_operator(".")) {
            auto that = QualifiedAccessNode{ .identifiers = $ NodeList() };
            that.identifiers->details.values.push_back(it);
            that.identifiers->details.values.push_back(parse_identifier(error_hint).any);

            while (read_operator(".")) {
                that.identifiers->details.values.push_back(parse_identifier(error_hint).any);
            }

            return $ that;
        }

        return it;
    }

    Node * parse_terminal() {
        if (read_operator("(")) {
            auto it = parse_expression();
            match(")", OperatorExpectedDiagnostic::Hint::NESTED_EXPRESSION);
            return it;
        }

        if (is_identifier()) {
            return parse_qualified_access(AnotherTokenTypeExpectedDiagnostic::Hint::TERMINAL);
        }

        if (is_string()) {
            return read_string().any;
        }

        if (is_character()) {
            return read_character().any;
        }

        if (is_binary()) {
            return read_binary().any;
        }

        if (is_octal()) {
            return read_octal().any;
        }

        if (is_hexadecimal()) {
            return read_hexadecimal().any;
        }

        if (is_decimal()) {
            return read_decimal().any;
        }

        auto start = input.get_offset();
        auto visualization = visualize();
        auto error = read_error();

        session.reporter << ExpressionExpectedDiagnostic{
            .filename = filename,
            .line_number = line_number,
            .range = {start, input.get_offset()},
            .visualization = visualization,
            .found = error->details.value
        };

        return error;
    }

    Node * parse_unary_minus() {
        if (read_operator("-")) {
            return $ UnaryExpressionNode{
                .target = parse_terminal(),
                .operator_token = "-"
            };
        }

        return parse_terminal();
    }

    Node * parse_multiply() {
        auto it = parse_unary_minus();

        while (
            read_operator("*") ||
            read_operator("/")
        ) {
            auto operator_lexeme = input.revise_all();
            auto that = parse_unary_minus();
            it = $ BinaryExpressionNode{
                .left = it,
                .right = that,
                .operator_token = operator_lexeme
            };
        }

        return it;
    }

    Node * parse_add() {
        auto it = parse_multiply();

        while (
            read_operator("+") ||
            read_operator("-")
        ) {
            auto operator_lexeme = input.revise_all();
            auto that = parse_multiply();
            it = $ BinaryExpressionNode{
                .left = it,
                .right = that,
                .operator_token = operator_lexeme
            };
        }

        return it;
    }

    Node * parse_expression() {
        return parse_add();
    }

    DetailedNode<NodeList> * parse_expression_list() {
        auto it = NodeList();
        it.values.push_back(parse_expression());

        while (read_operator(",")) {
            it.values.push_back(parse_expression());
        }

        return $ it;
    }

    Node * parse_assignment() {
        Node * it = parse_expression_list();

        while (read_operator("=")) {
            auto that = parse_expression_list();
            it = $ BinaryExpressionNode{
                .left = it,
                .right = that,
                .operator_token = "="
            };
        }

        return it;
    }

    DetailedNode<NodeList> * parse_type_list() {
        auto it = NodeList();
        it.values.push_back(parse_type());

        while (read_operator(",")) {
            it.values.push_back(parse_type());
        }

        return $ it;
    }

    DetailedNode<NodeList> * parse_type_parameters() {
        if (read_operator("<")) {
            auto that = parse_type_list();
            match(">", OperatorExpectedDiagnostic::Hint::TYPE_PAREMETERS);
            return that;
        }

        return $ NodeList();
    }

    Node * parse_simple_type() {
        auto it = parse_qualified_access(AnotherTokenTypeExpectedDiagnostic::Hint::TYPE_DECLARATION);
        auto that = parse_type_parameters();
        return $ TypeNode{
            .identifier = it,
            .subtypes = that
        };
    }

    Node * parse_coumpound_type() {
        if (read_operator("(")) {
            auto compound = parse_type_list();
            match(")", OperatorExpectedDiagnostic::Hint::COMPOUND_TYPE_LIST);
            return compound;
        }

        return parse_simple_type();
    }

    Node * parse_type() {
        auto it = parse_coumpound_type();

        if (read_operator("->")) {
            it = $ BinaryExpressionNode{
                .left = it,
                .right = parse_type(),
                .operator_token = "->"
            };
        }

        return it;
    }

    Node * parse_variable_declaration() {
        auto declaration = VariableDeclarationNode();
        declaration.variables = parse_identifier_list(AnotherTokenTypeExpectedDiagnostic::Hint::VARIABLE_DECLARATION);

        if (read_operator(":")) {
            declaration.type = parse_type();
        }

        if (read_operator("=")) {
            declaration.values = parse_expression_list();
        }

        return $ declaration;
    }

    Node * parse_constant_declaration() {
        auto declaration = ConstantDeclarationNode();
        declaration.constants = parse_identifier_list(AnotherTokenTypeExpectedDiagnostic::Hint::CONSTANT_DECLARATION);

        if (read_operator(":")) {
            declaration.type = parse_type();
        }

        if (read_operator("=")) {
            declaration.values = parse_expression_list();
        }

        return $ declaration;
    }

    Node * parse_typealias_declaration() {
        auto declaration = TypealiasDeclarationNode();
        declaration.type = parse_type();
        match("=", OperatorExpectedDiagnostic::Hint::TYPE_EQUALS);
        declaration.value = parse_type();
        return $ declaration;
    }

    Node * parse_if() {
        auto declaration = IfStatementNode();
        declaration.condition = parse_expression();
        declaration.on_true = parse_command_as_list();

        if (read_keyword("else")) {
            declaration.on_else = parse_command_as_list();
        }

        return $ declaration;
    }

    Node * parse_while() {
        auto declaration = WhileStatementNode();
        declaration.condition = parse_expression();
        declaration.on_true = parse_command_as_list();
        return $ declaration;
    }

    Node * parse_value_parameter_declaration() {
        auto declaration = VariableDeclarationNode();
        declaration.variables = $ NodeList();
        declaration.variables->details.values.push_back(parse_identifier(AnotherTokenTypeExpectedDiagnostic::Hint::VARIABLE_DECLARATION).any);

        if (read_operator(":")) {
            declaration.type = parse_type();
        }

        if (read_operator("=")) {
            declaration.values = $ NodeList();
            declaration.values->details.values.push_back(parse_expression());
        }

        return $ declaration;
    }

    DetailedNode<NodeList> * parse_value_parameter_declaration_list() {
        auto it = NodeList();
        it.values.push_back(parse_value_parameter_declaration());

        while (read_operator(",")) {
            it.values.push_back(parse_value_parameter_declaration());
        }

        return $ it;
    }

    Node * parse_function() {
        auto declaration = FunctionStatementNode();
        declaration.name = parse_identifier(AnotherTokenTypeExpectedDiagnostic::Hint::FUNCTION_NAME).any;

        if (read_operator("(")) {
            if (!read_operator(")")) {
                declaration.value_parameters = parse_value_parameter_declaration_list();
                match(")", OperatorExpectedDiagnostic::Hint::CLOSING_PARAMETERS);
            } else {
                declaration.value_parameters = $ NodeList();
            }
        } else {
            declaration.value_parameters = $ NodeList();
        }

        if (read_operator(":")) {
            declaration.return_type = parse_type();
        } else {
            declaration.return_type = nullptr;
        }

        declaration.body = parse_command_as_list();

        return $ declaration;
    }

    void parse_command(DetailedNode<NodeList> * commands) {
        if (read_indent()) {
            commands->details.values.push_back(parse_commands());
        }

        else if (read_keyword("var")) {
            commands->details.values.push_back(parse_variable_declaration());
        }

        else if (read_keyword("let")) {
            commands->details.values.push_back(parse_constant_declaration());
        }

        else if (read_keyword("typealias")) {
            commands->details.values.push_back(parse_typealias_declaration());
        }

        else if (read_keyword("if")) {
            commands->details.values.push_back(parse_if());
        }

        else if (read_keyword("while")) {
            commands->details.values.push_back(parse_while());
        }

        else if (read_keyword("fun")) {
            commands->details.values.push_back(parse_function());
        }

        else if (read_end()) {}

        else {
            commands->details.values.push_back(parse_assignment());
        }
    }

    void parse_command_without_indent(DetailedNode<NodeList> * commands) {
        if (read_indent()) {
            session.reporter << UnexpectedIndentDiagnostic{
                .filename = filename,
                .line_number = line_number,
                .range = {input.get_offset(), input.get_offset() + 1},
                .visualization = visualize(),
                .found = input.revise_all(),
                .type = "INDENT"
            };
            commands->details.values.push_back(parse_commands());
        }

        parse_command(commands);
    }

    Node * parse_commands() {
        auto commands = $ NodeList();
        parse_command(commands);

        while (!read_end()) {
            if (read_dedent()) {
                break;
            }

            parse_command_without_indent(commands);
        }

        return commands;
    }

    Node * parse_command_as_list() {
        auto commands = $ NodeList();
        parse_command(commands);
        return commands;
    }

    Node * parse() {
        return $ FileNode {
            .filename = filename,
            .root = parse_commands()
        };
    }
};


Node * cringe::ParsingContext::to_ast() {
    std::fstream file{filename};

    if (file.fail()) {
        std::cout << "Error > File `" << filename << "` could not be found." << std::endl;
        return nullptr;
    }

    orders::StdStream input{file};
    orders::AnalyzableStream analyzable_input{input};

    return ParsingContextBackend{
        .session = session,
        .filename = filename,
        .input = analyzable_input
    }.parse();
}
