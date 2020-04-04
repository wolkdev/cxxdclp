#ifndef CXXDCLP__HPP
#define CXXDCLP__HPP

#include "externs/tokenizer/tokenizer.hpp"

#include "tools.hpp"

#include "data.hpp"

#include <cstdio>
#include <vector>
#include <string>

class cxxdclp
{
    private:
    static tokenizer stokenizer;

    private:
    std::vector<std::string> context;

    std::vector<std::string> defines;

    std::vector<std::string> instruction;
    bool preprocessor = false;

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

    INSTRUCTION_TYPE get_instruction_type()
    {
        if (instruction.size() == 0)
        {
            return INSTRUCTION_TYPE::UNKNOWN;
        }
        else if (instruction[0] == "namespace")
        {
            return INSTRUCTION_TYPE::NAMESPACE;
        }
        else if (have_token(instruction, "class"))
        {
            return INSTRUCTION_TYPE::CLASS;
        }
        else if (have_token(instruction, "struct"))
        {
            return INSTRUCTION_TYPE::STRUCT;
        }
        else if (have_token(instruction, "typedef"))
        {
            return INSTRUCTION_TYPE::TYPEDEF;
        }
        else
        {
            size_t equals = find_token_pos(instruction, "=");
            size_t bracket = find_token_pos(instruction, "(");

            if (equals != std::string::npos)
            {
                // pure function
                if (bracket != std::string::npos && bracket < equals)
                {
                    return INSTRUCTION_TYPE::FUNCTION;
                }
                // variable
                else
                {
                    return INSTRUCTION_TYPE::VARIABLE;
                }
            }
            // function
            else if (bracket != std::string::npos)
            {
                return INSTRUCTION_TYPE::FUNCTION;
            }
            // variable
            else
            {
                return INSTRUCTION_TYPE::VARIABLE;
            }
        }

        return INSTRUCTION_TYPE::UNKNOWN;
    }

    INSTRUCTION_TYPE parse_instruction()
    {
        INSTRUCTION_TYPE type = get_instruction_type();

        switch (type)
        {
            case INSTRUCTION_TYPE::VARIABLE:
            {
                variables.push_back(parse_variable(0, instruction.size() - 1));
                break;
            }
            case INSTRUCTION_TYPE::FUNCTION:
            {
                functions.push_back(parse_function(0, instruction.size() - 1));
                break;
            }
            
            default: break;
        }

        return type;
    }

    void parse_preprocessor_instruction()
    {
        if (instruction[0] == "include")
        {

        }
        else if (instruction[0] == "define")
        {

        }
        else if (instruction[0] == "if")
        {

        }
        // ...
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
                const std::string& token = stokenizer.next();

                if (token == "\"")
                {
                    // TODO : Get 'raw' string
                }

                if (preprocessor)
                {
                    if (token == "\n")
                    {
                        parse_preprocessor_instruction();
                        instruction.clear();
                        preprocessor = false;
                    }
                    else
                    {
                        instruction.push_back(token);
                    }
                }
                else
                {
                    if (token == "#")
                    {
                        preprocessor = true;
                    }
                    else if (token == ";" || token == "{")
                    {
                        instruction.push_back(token);
                        type = parse_instruction();
                        instruction.clear();

                        if (type == INSTRUCTION_TYPE::FUNCTION && token == "{")
                        {
                            skip_until(stokenizer, "}");
                        }
                    }
                    else if (token != "\n")
                    {
                        instruction.push_back(token);
                    }
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
    " \t\r",                  // Dropped Delimiters

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

#endif // !CXXDCLP__HPP