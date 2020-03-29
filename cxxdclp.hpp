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

    std::vector<std::string> defines;

    std::vector<std::string> instruction;
    bool preprocessor = false;

    private:
    void parse_variable(const std::vector<std::string>& _tokens)
    {
        std::string name;
        std::vector<std::string> type;
        std::vector<std::string> value;

        for (size_t i = 0; i < _tokens.size() - 1; i++)
        {
            if (!name.empty())
            {
                value.push_back(_tokens[i]);
            }
            else
            {
                if (_tokens[i + 1] == "=" || _tokens[i + 1] == ";")
                {
                    name = _tokens[i];
                    i++; // skip "=" or ";"
                }
                else
                {
                    type.push_back(_tokens[i]);
                }
            }
        }
    }

    void parse_function(const std::vector<std::string>& _tokens)
    {
        
    }

    void parse_instruction(const std::vector<std::string>& _tokens)
    {
        size_t size = _tokens.size();

        if (size == 0)
        {
            return;
        }
        else if (_tokens[0] == "class")
        {

        }
        else if (_tokens[0] == "struct")
        {

        }
        else if (_tokens[0] == "namespace")
        {

        }
        else
        {
            size_t equals = find_token_pos(_tokens, "=");
            size_t bracket = find_token_pos(_tokens, "(");

            if (equals != std::string::npos)
            {
                // pure function
                if (bracket != std::string::npos && bracket < equals)
                {
                    parse_function(_tokens);
                }
                // variable
                else
                {
                    parse_variable(_tokens);
                }
            }
            // function
            else if (bracket != std::string::npos)
            {
                parse_function(_tokens);
            }
            // variable
            else
            {
                parse_variable(_tokens);
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

                if (token == "\"")
                {
                    // TODO : Get 'raw' string
                }

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

                        instruction.push_back(token);
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
    }
);

#endif // !CXXDCLP__HPP