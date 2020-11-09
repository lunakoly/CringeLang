#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include <arrrgh/arrrgh.hpp>

#include <orders/streams/implementations/std_stream.hpp>

#include <cringe/about.hpp>
#include <cringe/parser.hpp>
#include <cringe/scope_resolver.hpp>
#include <cringe/global_declaration_resolver.hpp>
#include <cringe/deep_declaration_resolver.hpp>


cringe::AST::Node * parse_file(
    cringe::Session & session,
    const std::string & filename
) {
    cringe::ParsingContext context{
        .session = session,
        .filename = filename
    };

    return context.to_ast();
}


int run() {
    if (arrrgh::options<int>["tab-size"] <= 1) {
        std::cout << "Wait > Tab size `" << arrrgh::options<int>["tab-size"] << "` is invalid. It must be > 1";
        return 1;
    }

    cringe::Session session{
        .options = {
            .tab_size = arrrgh::options<int>["tab-size"]
        }
    };

    auto global = new cringe::AST::DetailedNode<cringe::AST::GlobalNode>({
        .files = new cringe::AST::DetailedNode<cringe::AST::NodeList>({})
    });

    for (size_t that = 1; that < arrrgh::parameters.size(); that++) {
        std::string filename{arrrgh::parameters[that]};

        auto it = parse_file(session, filename);
        global->details.files->details.values.push_back(it);

        if (it != nullptr) {
            std::cout << "==== Diagnostics ====" << std::endl;
            for (auto that : session.reporter.diagnostics) {
                std::cout << *that << std::endl;

                if (arrrgh::options<bool>["no-links"] == false) {
                    std::cout << "Quick Link > " << std::filesystem::absolute(filename).string() << "(" << that->get_line_number() << ",1)" << std::endl;
                }
            }

            std::cout << std::endl;
            std::cout << "==== AST ====" << std::endl;
            std::cout << *it << std::endl;
            std::cout << std::endl;
        }
    }

    cringe::resolve_scopes(global);
    cringe::resolve_global_declarations(global);
    cringe::resolve_deep_declarations(session, global);

    auto files = global->details.files->details.values;

    std::cout << "==== RESOLVED ====" << std::endl;
    std::cout << *global << std::endl;
    std::cout << std::endl;

    std::cout << "==== Done ====" << std::endl;
    return 0;
}


static const char * HELP_TEXT =
    "Usage > cringe [options...] file1 [file2...]\n"
    "    --std [<int> | latest]\n"
    "        Specifies the language version.\n"
    "    -t, --tab-size <int>\n"
    "        Sets the tab size for the lexer.\n"
    "    -v, --version\n"
    "        Prints general information.\n"
    "    --no-links\n"
    "        Don't print quick links.\n"
    "    -h, --help\n"
    "        Prints this text.\n"
;


int main(int argc, char * argv[]) {
    arrrgh::add_flag("help");
    arrrgh::add_flag("version");
    arrrgh::add_integer("tab-size", 4);
    arrrgh::add_flag("no-links");
    arrrgh::add_option<arrrgh::StringLike>("std", "undefined");

    arrrgh::add_alias('h', "help");
    arrrgh::add_alias('v', "version");
    arrrgh::add_alias('t', "tab-size");

    arrrgh::parse(argv, argv + argc);

    if (arrrgh::options<bool>["help"] || arrrgh::parameters.size() < 2) {
        // parameters[0] is the path to the command
        std::cout << HELP_TEXT << std::endl;
    }

    else if (arrrgh::options<bool>["version"]) {
        std::cout << "Cringe Compiler v" << __CRINGE_VERSION_MAJOR__ << '.' << __CRINGE_VERSION_MINOR__ << std::endl;
    }

    else if (arrrgh::options<arrrgh::StringLike>["std"] == "undefined") {
        std::cout << "Error > It's mandatory to specify the `--std` option with the appropriate language version. Use `--std latest` to use the newest." << std::endl;
    }

    else if (
        arrrgh::options<arrrgh::StringLike>["std"] == "latest" ||
        arrrgh::options<arrrgh::StringLike>["std"] == "1"
    ) {
        return run();
    }

    else {
        std::cout << "Error > Unsupported language version > `" << arrrgh::options<arrrgh::StringLike>["std"] << '`' << std::endl;
    }
}
