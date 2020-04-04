#ifndef CXXDCLP__HPP
#define CXXDCLP__HPP

#include "externs/tokenizer/tokenizer.hpp"

#include "tools.hpp"

#include "data.hpp"
#include "category.hpp"

#include <cstdio>
#include <vector>
#include <string>

class cxxdclp
{
    private:
    static tokenizer stokenizer;
    static category s_Protection;
    static category s_NativeType;
    static category s_Specifier;
    static category s_NotSupported;

    private:
    
    std::string token;
    std::vector<std::string> instruction;

    std::string name;
    std::vector<std::string> strs_type;
    std::vector<std::string> strs_specifiers;
    std::string str_protection;

    public:
    std::vector<type> types;
    std::vector<variable> variables;
    std::vector<function> functions;

    private:
    variable parse_variable(size_t _start, size_t _end)
    {
        variable var;

        bool value = false;

        for (size_t i = _start; i <= _end; i++)
        {
            if (value)
            {
                var.value.push_back(instruction[i]);
            }
            else
            {
                if (instruction[i] == "="
                    || instruction[i] == ";"
                    || instruction[i] == ","
                    || instruction[i] == ")"
                    || instruction[i] == "[")
                {
                    if (var.name.empty())
                    {
                        var.name = var.type.back();
                        var.type.pop_back();
                    }

                    if (instruction[i] == "=")
                    {
                        value = true;
                    }
                    else if (instruction[i] == "[")
                    {
                        var.type.push_back(instruction[i]);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    var.type.push_back(instruction[i]);
                }
            }
        }

        return var;
    }

    function parse_function(size_t _start, size_t _end)
    {
        function func;

        size_t arg = 0;

        for (size_t i = _start; i <= _end; i++)
        {
            if (!func.name.empty())
            {
                if (instruction[i] == "," || instruction[i] == ")" && arg < i)
                {
                    func.args.push_back(parse_variable(arg, i));
                    arg = i + 1;
                }
            }
            else
            {
                const std::string& next = instruction[i + 1];

                if (next == "(")
                {
                    func.name = instruction[i];
                    i++; // skip "("
                    arg = i + 1;
                }
                else
                {
                    func.type.push_back(instruction[i]);
                }
            }
        }

        return func;
    }

    void parse_instruction_token(const std::string& _token)
    {
        if (_token == s_Protection)
        {
            str_protection = token;
        }
    }

    INSTRUCTION_TYPE parse_instruction()
    {
        for (size_t i = 0; i < instruction.size(); i++)
        {
            parse_instruction_token(instruction[i]);
        }
        
        return INSTRUCTION_TYPE::UNKNOWN;
    }

    void get_nex_instruction()
    {
        bool preprocessor = false;

        instruction.clear();

        while (!stokenizer.finished() &&
              (preprocessor ?
              (token != "\n") :
              (token != ";" && token != "}")))
        {
            token = stokenizer.next();

            if (token == "#")
            {
                preprocessor = true;
            }

            if (token == "//")
            {
                skip_until(stokenizer, "\n");
            }
            else if (token == "/*")
            {
                skip_until(stokenizer, "*/");
            }
            else
            {
                if (token != "\n")
                {
                    instruction.push_back(token);
                }

                if (token == "\"")
                {
                    instruction.push_back(get_string(stokenizer.text - 1));
                    stokenizer.text += instruction.back().size();
                }
            }
        }
    }

    void parse_file(const char* _filePath)
    {
        char* content = file_read_all_text(_filePath);

        INSTRUCTION_TYPE type;

        if (content != nullptr)
        {
            stokenizer.start(content);

            while (!stokenizer.finished())
            {
                get_nex_instruction();
                type = parse_instruction();

                if (type == INSTRUCTION_TYPE::FUNCTION
                    && instruction.back() == "{")
                {
                    skip_until(stokenizer, "}");
                }
            }
        }
    }

    public:
    static cxxdclp parse(const char* _startupPath)
    {
        cxxdclp parser;
        parser.parse_file(_startupPath);
        return parser;
    }
};

tokenizer cxxdclp::stokenizer = tokenizer
(
    " \t\r",                    // Dropped Delimiters

    std::vector<std::string>    // Kept Delimiters
    {
        "::",
        "&&",
        "->",
        "//",
        "/*",
        "*/",
        ";",
        "{",
        "}",
        "(",
        ")",
        ",",
        ":",
        "&",
        "*",
        "=",
        "+",
        "-",
        "/",
        "<",
        ">",
        "\"",
        "'",
        ".",
        "[",
        "]",
        "#",
        "\n"
    }
);

category cxxdclp::s_Protection = category
(
    std::vector<std::string>
    {
        "public",
        "private",
        "protected"
    }
);

category cxxdclp::s_NativeType = category
(
    std::vector<std::string>
    {
        "void",
        "char",
        "short",
        "int",
        "long",
        "float",
        "double",
        "unsigned"
    }
);

category cxxdclp::s_Specifier = category
(
    std::vector<std::string>
    {
        "static",
        "inline",
        "virtual",
        "extern"
    }
);

category cxxdclp::s_NotSupported = category
(
    std::vector<std::string>
    {
        "template"
    }
);

#endif // !CXXDCLP__HPP