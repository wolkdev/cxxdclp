#ifndef CXXDCLP__HPP
#define CXXDCLP__HPP

#include "externs/tokenizer/tokenizer.hpp"
#include "tools.hpp"

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

class cxxdclp
{
    private:
    static tokenizer stokenizer;
    
    private:
    std::vector<std::string> context;

    std::vector<std::string> instruction;
    bool preprocessor = false;

    private:
    void parse_instruction(const std::vector<std::string>& _tokens)
    {
        size_t size = _tokens.size();

        size_t equals = find_token_pos(_tokens, "=");
        size_t bracket = find_token_pos(_tokens, "(");

        if (equals != std::string::npos)
        {
            // pure function
            if (bracket != std::string::npos
                && bracket < equals
                && _tokens[equals + 1] == "0")
            {

            }
            // variable
            else
            {
                const std::string& name = _tokens[equals - 1];
                std::vector<std::string> type;

                for (size_t i = 0; i < equals - 1; i++)
                {
                    type.push_back(_tokens[i]);
                }
            }
        }
    }

    void parse_preprocessor_instruction(const std::vector<std::string>& _tokens)
    {
        if (_tokens[0] == "include")
        {

        }
        else if (_tokens[0] == "define")
        {

        }
        else if (_tokens[0] == "if")
        {

        }
        // ...
    }

    void parse_file(const char* _filePath)
    {
        char* content = file_read_all_text(_filePath);

        if (content != nullptr)
        {
            stokenizer.start(content);

            while (!stokenizer.finished())
            {
                const std::string& token = stokenizer.next();

                if (preprocessor)
                {
                    if (token == "\n") // TODO : keep end line char ?
                    {
                        parse_preprocessor_instruction(instruction);
                        instruction.clear();
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
                        if (token == "{")
                        {
                            skip_until(stokenizer, "}");
                        }

                        parse_instruction(instruction);
                        instruction.clear();
                    }
                    else
                    {
                        instruction.push_back(token);
                    }
                }

                std::cout << token << "\n";
            }
        }
    }

    public:
    static void parse(const char* _startupPath)
    {
        cxxdclp parser;
        parser.parse_file(_startupPath);
    }
};

tokenizer cxxdclp::stokenizer = tokenizer
(
    " \t\r\n",                  // Dropped Delimiters

    std::vector<std::string>    // Kept Delimiters
    {
        "template",
        "struct",
        "class",
        "enum",
        "typedef",
        "typename",

        "mutable",
        "default",

        "const",
        "if",
        "else",
        "for",
        "while",
        "do",
        "public",
        "protected",
        "private",
        "nullptr",
        "auto",
        "new",
        "delete",
        "static",
        "virtual",
        "override",

        "include",
        "define",
        "ifndef",
        "ifdef",
        "endif",
        "elif",
        "undef",

        "unsigned",
        "void",
        "int",
        "float",
        "double",
        "char",

        ";",
        "{",
        "}",

        "::",
        "&&",
        "->",
        "//",
        "/*",
        "*/",
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
    }
);

#endif // !CXXDCLP__HPP