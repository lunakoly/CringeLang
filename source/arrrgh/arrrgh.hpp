// Copyright (C) 2020 luna_koly
//
// This code simplifies input arguments
// management.


#pragma once

#include <any>
#include <vector>
#include <string>
#include <charconv>
#include <unordered_map>

#include <iostream>


/*
    Argument parsing manager
*/
namespace arrrgh {
    // Code starts to look VERY complicated without
    // these aliases

    /*
        Alias for std::string_view
    */
    using StringLike = std::string_view;

    /*
        Alias for std::unordered_map<T, K>
    */
    template <typename T, typename K>
    using Map = std::unordered_map<T, K>;

    /*
        Alias for std::vector<std::string_view>
    */
    using List = std::vector<StringLike>;


    /*
        Represents a sequence of StringLike. Actually just a bit
        different form of passing iterators around
        (in and out of the functions) in a way that
        any inner function 'modifies the pointers
        of the outer function'
    */
    struct Arguments {
        virtual ~Arguments() {};

        /*
            Returns true if there other
            values left unread
        */
        virtual bool has_next() const = 0;

        /*
            Returns a StringLike of the
            next value of the stream.
            This helps to generalize
            const char *[] and std::string streams
        */
        virtual StringLike next() const = 0;
    };

    /*
        Implements Arguments based on two iterators
    */
    template <typename Iterator>
    class Sequence : public Arguments {
    public:
        /*
            Creates a sequence starting
            with current and ending with end
        */
        Sequence(Iterator current, Iterator end)
            : current(current), end(end) {}

        bool has_next() const override {
            return current != end;
        }

        StringLike next() const override {
            return *(current++);
        }

    private:
        mutable Iterator current;
        const Iterator end;
    };


    /*
        A function that modifies an option's value
        based on some next arguments and the previous
        value
    */
    using Processor = void (*)(StringLike, const Arguments &);


    /*
        Maps options to processors that
        work with them
    */
    Map<StringLike, Processor> processors;

    /*
        Maps options to their values
    */
    template <typename T>
    Map<StringLike, T> options;

    /*
        Maps aliases to their options
    */
    Map<char, StringLike> aliases;

    /*
        Collects 'free' parameters
    */
    List parameters;


    /*
        Assigns True to an option
    */
    void flag_processor(
        StringLike option_name,
        const Arguments & arguments
    ) {
        options<bool>[option_name] = true;
    }

    /*
        Attempts to assign the next
        argument value to an option
    */
    void single_argument_processor(
        StringLike option_name,
        const Arguments & arguments
    ) {
        if (arguments.has_next()) {
            options<StringLike>[option_name] = arguments.next();
        }
    }

    /*
        Collects all values of the same option
        that is being called multiple times
        together into a list
    */
    void list_processor(
        StringLike option_name,
        const Arguments & arguments
    ) {
        if (arguments.has_next()) {
            auto & list = options<List>[option_name];
            list.push_back(arguments.next());
        }
    }

    /*
        Attempts to assign the next
        argument value to an option
        if it's an integer. Otherwise
        leaves the default value as is
    */
    void integer_processor(
        StringLike option_name,
        const Arguments & arguments
    ) {
        if (arguments.has_next()) {
            auto value = arguments.next();

            auto result = std::from_chars(
                value.data(),
                value.data() + value.size(),
                options<int>[option_name]
            );

            if (
                result.ec == std::errc::invalid_argument ||
                result.ec == std::errc::result_out_of_range
            ) {
                std::cout << "Warning > Ignoring value '" << value << "' because it's not an integer" << std::endl;
            }
        }
    }

    /*
        Attempts to assign the next
        argument value to an option
        if it's an integer. Otherwise
        leaves the default value as is
    */
    void float_processor(
        StringLike option_name,
        const Arguments & arguments
    ) {
        if (arguments.has_next()) {
            auto value = arguments.next();
            errno = 0;
            char * end;

            options<double>[option_name] = std::strtod(value.data(), &end);

            if (
                *end != '\0' ||
                errno != 0 ||
                end == value.data()
            ) {
                std::cout << "Warning > Ignoring value '" << value << "' because it's not a float" << std::endl;
            }
        }
    }


    /*
        Registers a new option.
        Make sure processor is suitable
        for it's default_value type if you
        provide them manually
    */
    template <typename Value = StringLike>
    void add_option(
        StringLike option_name,
        Value default_value = StringLike(),
        Processor processor = single_argument_processor
    ) {
        processors[option_name] = processor;
        using Type = decltype(default_value);
        options<Type>[option_name] = default_value;
    }

    /*
        Registers a new alias.
        Requires the option to exist
    */
    void add_alias(
        char alias_name,
        StringLike option_name
    ) {
        auto it = processors.find(option_name);

        if (it != processors.end()) {
            aliases[alias_name] = option_name;
        } else {
            std::cout << "Error > Can\'t create alias for non-existing option > " << option_name << std::endl;
        }
    }

    /*
        Registers a new boolean option
    */
    void add_flag(
        StringLike option_name
    ) {
        add_option(option_name, false, flag_processor);
    }

    /*
        Registers a new list option
    */
    void add_list(
        StringLike option_name
    ) {
        add_option(option_name, List(), list_processor);
    }

    /*
        Registers a new integer option
    */
    void add_integer(
        StringLike option_name,
        int default_value = 0
    ) {
        add_option(option_name, default_value, integer_processor);
    }

    /*
        Registers a new float option
    */
    void add_float(
        StringLike option_name,
        double default_value = 0.0
    ) {
        add_option(option_name, default_value, float_processor);
    }


    /*
        Calls registered Processor for an
        option
    */
    void parse_option(
        StringLike option_name,
        const Arguments & arguments
    ) {
        auto it = processors.find(option_name);

        if (it != processors.end()) {
            it->second(option_name, arguments);
        } else {
            std::cout << "Error > Unspecified option met > " << option_name << std::endl;
        }
    }

    /*
        Calls registered Processor for an
        alias
    */
    void parse_alias(
        char alias_name,
        const Arguments & arguments
    ) {
        auto it = aliases.find(alias_name);

        if (it != aliases.end()) {
            parse_option(it->second, arguments);
        } else {
            std::cout << "Error > Unspecified alias met > " << alias_name << std::endl;
        }
    }

    /*
        Returns true if the string
        starts with the prefix
    */
    bool starts_with(StringLike text, StringLike prefix) {
        return text.rfind(prefix, 0) != text.npos;
    }

    /*
        Analyses the arguments
    */
    template <typename Iterator>
    void parse(Iterator begin, Iterator end) {
        const Sequence arguments {begin, end};

        while (arguments.has_next()) {
            auto next = arguments.next();

            if (starts_with(next, "--")) {
                parse_option(next.substr(2), arguments);
            }

            else if (starts_with(next, "-")) {
                for (int it = 1; it < next.length(); it++) {
                    parse_alias(next[it], arguments);
                }
            }

            else {
                parameters.emplace_back(next);
            }
        }
    }
}
