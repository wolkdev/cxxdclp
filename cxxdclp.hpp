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

    private:
    
    std::string token;
    std::vector<std::string> instruction;

    int index;

    std::string name;
    std::vector<std::string> strs_type;
    std::vector<std::string> strs_specifiers;
    PROTECTION protection;
    bool typelink;

    bool isStatic;
    bool isVirtual;
    bool isInline;
    bool isExtern;

    bool forwardDcl;

    public:
    std::vector<type> types;
    std::vector<variable> variables;
    std::vector<function> functions;

    private:

    bool try_parse_template()
    {
        if (instruction[index] == "template")
        {

        }
        else
        {
            return false;
        }
        
        return true;
    }

    bool try_parse_type()
    {
        if (instruction[index] == "class")
        {

        }
        else if (instruction[index] == "struct")
        {

        }
        else if (instruction[index] == "typedef")
        {
            
        }
        else
        {
            return false;
        }

        if (instruction[index + 1] == ";")
        {
            forwardDcl = true;
        }

        return true;
    }

    bool try_parse_protection()
    {
        if (instruction[index] == "public"
            && instruction[index + 1] == ":")
        {
            protection = PROTECTION::PUBLIC;
        }
        else if (instruction[index] == "private"
            && instruction[index + 1] == ":")
        {
            protection = PROTECTION::PRIVATE;
        }
        else if (instruction[index] == "protected"
            && instruction[index + 1] == ":")
        {
            protection = PROTECTION::PROTECTED;
        }
        else
        {
            return false;
        }

        index += 2;

        return true;
    }

    bool try_parse_specifier()
    {
        if (instruction[index] == "static")
        {
            isStatic = true;
        }
        else if (instruction[index] == "virtual")
        {
            isVirtual = true;
        }
        else if (instruction[index] == "inline")
        {
            isInline = true;
        }
        else if (instruction[index] == "extern")
        {
            isExtern = true;
        }
        else
        {
            return false;
        }
        
        return true;
    }

    bool try_parse_array(int& _arraySize)
    {
        if (instruction[index] == "[")
        {
            if (is_digit(instruction[++index]))
            {
                _arraySize = std::stoi(instruction[index]);
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    bool try_parse_value(std::string& _value)
    {
        if (instruction[index] == "=")
        {
            while (instruction[++index] != ";")
            {
                _value += instruction[index];
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    bool is_pointer()
    {
        return instruction[index] == "*";
    }

    bool is_reference()
    {
        return instruction[index] == "&";
    }

    bool is_move_reference()
    {
        return instruction[index] == "&&";
    }

    bool is_const()
    {
        return instruction[index] == "const";
    }

    bool is_type_extension()
    {
        return instruction[index] == "unsigned"
            || instruction[index] == "long"
            || instruction[index] == "short";
    }

    void parse_member(member& _member)
    {
        int arraySize;
        
        while (instruction[index] != ";")
        {
            if (is_const())
            {
                _member.isConst;
            }
            else if (try_parse_array(arraySize))
            {
                _member.arraySizes.push_back(arraySize);
            }
            else if (is_type_extension())
            {
                _member.typeName += instruction[index];
            }
            else if (try_parse_value(_member.value))
            {

            }
        }
    }

    void parse_instruction_token()
    {
        if (try_parse_template())
        {

        }
        else if (try_parse_type())
        {
            
        }
        // ...
    }

    INSTRUCTION_TYPE parse_instruction()
    {
        for (index = 0; index < instruction.size(); index++)
        {
            parse_instruction_token();
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

#endif // !CXXDCLP__HPP